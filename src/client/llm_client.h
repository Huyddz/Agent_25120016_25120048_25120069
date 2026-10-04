#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// Tool call
struct ToolCall {
    std::string id;//Ollama maybe blank, but OpenAI must have (xxx)            
    std::string functionName;
    nlohmann::json arguments;   // parse into JSON Object to pass to the tool.
};

// Chat message
struct ChatMessage {
    std::string role;           // "system", "user", "assistant", "tool"
    std::string content;
    std::string toolCallId;   // role == "tool" 
    std::vector<std::string> imagesBase64; // handle images
};

// Response
struct LLMResponse {
    std::string content;
    std::vector<ToolCall> toolCalls;
    bool hasToolCalls() const { return !toolCalls.empty(); }
};

// Abstract class LLMClient!!
class LLMClient {
public:
    virtual ~LLMClient() = default; //destructor

    virtual LLMResponse chat(const std::vector<ChatMessage>& messages, 
                             const nlohmann::json& tools_schema = nlohmann::json::array()) = 0;
};