#include "spotify/auth.hpp"
#include "spotify/client.hpp"
#include "spotify/api.hpp"
#include "env.hpp"

int main() {
    Env env = loadEnv();
    
    try {
        spotify::Auth auth(
            env.client_id,
            env.client_secret,
            env.redirect_uri
        );
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}