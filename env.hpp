#pragma once

#include <string>
#include <fstream>

struct Env {
    std::string client_id;
    std::string client_secret;
    std::string redirect_uri;
};

Env loadEnv();