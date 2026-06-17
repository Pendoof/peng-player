#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace spotify {

class Client {
public:
    explicit Client(std::string access_token);

    nlohmann::json search(const std::string& query);

    void play(const std::string& uri);
    void pause();
    void next();

private:
    std::string access_token;
};

}