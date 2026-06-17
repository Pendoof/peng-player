#pragma once
#include "server.hpp"
#include "endpoints.hpp"

#include "httplib.h"
#include <nlohmann/json.hpp>

#include <string>
#include <sstream>
#include <windows.h>
#include <chrono>

namespace spotify {

class Auth {
public:
    Auth(std::string client_id, std::string client_secret, std::string redirect_uri);

    std::string getAuthURL() const;
    void login();
    void openAuthURL() const;
    void exchangeCode(const std::string& code);
    void saveTokens() const;
    bool loadTokens();
    void refreshTokens();
    std::string getAccessToken();

private:
    std::string client_id;
    std::string redirect_uri;
    std::string client_secret;
    std::string access_token;
    std::string refresh_token;
    std::chrono::system_clock::time_point expires_at;
};

}