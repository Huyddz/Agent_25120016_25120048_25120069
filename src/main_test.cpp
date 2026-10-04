
#include "client/ollama_client.h"
#include <iostream>

int main() {
    std::cout << "Starting OllamaClient test..." << std::endl;

    // Replace with your active ngrok/pinggy URL or local endpoint
    std::string ollama_url = "https://835c-130-211-229-253.ngrok-free.app/v1"; 
    std::string model = "qwen2.5"; 

    try {
        OllamaClient client(ollama_url, model);

        std::vector<ChatMessage> test_messages = {
            {"system", "You are a helpful assistant."},
            {"user", "Say 'Hello from Ollama' and nothing else."}
        };

        std::cout << "Sending test request..." << std::endl;
        LLMResponse response = client.chat(test_messages);

        std::cout << "Response received:" << std::endl;
        std::cout << response.content << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error encountered: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}