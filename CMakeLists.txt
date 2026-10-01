cmake_minimum_required(VERSION 3.16)
project(OOP_Agent_25120016_25120048_25120069 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.10.0 REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(src ${CURL_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS})

# Thu vien
# Xay bang cach de quy di tim cac file .cpp de noi voi cac libs ben ngoai
# Noi chung la nhu Thay bao dao mot duong ham de noi voi server ben Kaggle (Ollama)
file(GLOB_RECURSE agentSrc "src/agent/*.cpp" "src/client/*.cpp" "src/tools/*.cpp" "src/harness/*.cpp")
# Doan nay luu y trong README la khi them 1 file moi
# thi phai load lai toan bo CMake de cap nhat
add_library(agentCore STATIC ${agentSrc})
target_link_libraries(agentCore PUBLIC ${CURL_LIBRARIES} nlohmann_json::nlohmann_json ${SQLite3_LIBRARIES})

# Benchmark
# Build OOP vao agentCore, build ra ngoai la Kaboommmm!!!
add_executable(run_eval benchmark/run_eval.cpp)
target_link_libraries(run_eval PRIVATE agentCore)
