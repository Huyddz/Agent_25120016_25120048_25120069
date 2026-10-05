#include "ollama_client.h"
#include <curl/curl.h>
#include <stdexcept>
#include <iostream>

OllamaClient::OllamaClient(std::string url, std::string modelName, float temp)
    : baseURL(std::move(url)), model(std::move(modelName)), temperature(temp) {
    if (!baseURL.empty() && baseURL.back() == '/') { // Remove in case mistyped
        baseURL.pop_back();
    }
}

size_t OllamaClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

std::string OllamaClient::sendHttpPost(const std::string& endpoint, const std::string& json_payload) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("cURL failed.");

    std::string response_data;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // Add ngrok header to bypass browser warning
    headers = curl_slist_append(headers, "ngrok-skip-browser-warning: true");
    // just in case

    std::string url = baseURL + endpoint;
    // Debugging output (to check the URL being used)
    std::cout << "Target URL: " << url << std::endl;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120L); // 120s (update in the next test)

    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        throw std::runtime_error("cURL Request Error: " + std::string(curl_easy_strerror(res)));
    }
    if (http_code >= 400) {
        throw std::runtime_error("Ollama HTTP Error " + std::to_string(http_code) + ": " + response_data);
    }

    return response_data;
}

LLMResponse OllamaClient::chat(const std::vector<ChatMessage>& messages, const nlohmann::json& tools_schema) {
    nlohmann::json body;
    body["model"] = model;
    body["stream"] = false;
    body["options"] = {{"temperature", temperature}};

    // messages -> format Ollama /api/chat
    nlohmann::json msgs_arr = nlohmann::json::array();
    for (const auto& msg : messages) {
        nlohmann::json m = {
            {"role", msg.role},
            {"content", msg.content}
        };
        if (!msg.imagesBase64.empty()) {
            m["images"] = msg.imagesBase64;
        }
        msgs_arr.push_back(m);
    }
    body["messages"] = msgs_arr;

    if (!tools_schema.empty()) {
        body["tools"] = tools_schema;
    }

    std::string raw_res = sendHttpPost("/api/chat", body.dump());
    auto json_res = nlohmann::json::parse(raw_res);

    LLMResponse response;
    if (json_res.contains("message")) {
        auto msg_obj = json_res["message"];
        response.content = msg_obj.value("content", "");

        // Parse tool calls
        if (msg_obj.contains("tool_calls") && msg_obj["tool_calls"].is_array()) {
            for (const auto& tc : msg_obj["tool_calls"]) {
                ToolCall call;
                call.id = "";
                call.functionName = tc["function"]["name"].get<std::string>();
                
                // Ollama returns JSON object
                call.arguments = tc["function"]["arguments"];
                response.toolCalls.push_back(call);
            }
        }
    }

    return response;
}