#pragma once
#include "llm_client.h"
#include <string>

class OllamaClient : public LLMClient {
private:
    std::string baseURL;
    std::string model;
    float temperature;

    static size_t writeCallback(void* contents, size_t size, size_t numberMember, void* userPointer); //dat static dum bo, khong la no crash voi phan harness
    std::string sendHttpPost(const std::string& endpoint, const std::string& json_payload);

public:
    OllamaClient(std::string url, std::string modelName, float temp = 0.2f); // modelName == Ollama || OpenAI;
    // Handle separately, due next week.
    ~OllamaClient() override = default; //destructor

    LLMResponse chat(const std::vector<ChatMessage>& messages, 
                     const nlohmann::json& tools_schema = nlohmann::json::array()) override;
};