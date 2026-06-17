#include "env.hpp"

Env loadEnv() {
    Env env;
    std::ifstream file("../src/.env");
    std::string line;

    while (std::getline(file, line)) {
        if (line.starts_with("CLIENT_ID="))
            env.client_id = line.substr(11, line.length() - 12);
        else if (line.starts_with("CLIENT_SECRET="))
            env.client_secret = line.substr(15, line.length() - 16);
        else if (line.starts_with("REDIRECT_URI="))
            env.redirect_uri = line.substr(14, line.length() - 15);
    }

    return env;
}