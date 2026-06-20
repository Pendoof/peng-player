cmake_minimum_required(VERSION 3.20)

project(
    peng-player
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(httplib REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)


add_executable(peng-player
    src/main.cpp
    src/env.cpp
    src/spotify/auth.cpp
    src/spotify/server.cpp
    src/spotify/api.cpp
)

target_compile_definitions(peng-player PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

target_include_directories(peng-player PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(peng-player PRIVATE
    CURL::libcurl
    httplib::httplib
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
)

if(MSVC)
    target_compile_options(peng-player PRIVATE /W4)
else()
    target_compile_options(peng-player PRIVATE -Wall -Wextra -Wpedantic)
endif()