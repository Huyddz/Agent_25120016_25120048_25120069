
#include "client/ollama_client.h"
#include <iostream>

int main() {
    std::cout << "Starting OllamaClient test..." << std::endl;

    // Use teacher's URL
    std::string ollama_url = "https://blinks-radar-grandma.ngrok-free.dev"; // ensure the Kaggle still runs.
    std::string model = "qwen3.8:27b"; 

    try {
        OllamaClient client(ollama_url, model);

        std::vector<ChatMessage> test_messages = {
            {"system", "You are a helpful assistant."},
            {"user", "Write a brief history about Vietnamese GDP growth."}
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