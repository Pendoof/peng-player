#include "auth.hpp"

#include <iostream>

namespace spotify {
    Auth::Auth(std::string client_id, std::string client_secret, std::string redirect_uri)
        :   client_id(std::move(client_id)),
            client_secret(std::move(client_secret)),
            redirect_uri(std::move(redirect_uri)) {
        if (!loadTokens()) {
            login();
        } else {
            refreshTokens();
        }
    }

    std::string Auth::getAuthURL() const {
    std::ostringstream url;

    url << endpoints::AUTH_URL
        << "?client_id=" << client_id
        << "&response_type=code"
        << "&redirect_uri=" << redirect_uri
        << "&scope=user-read-playback-state%20user-modify-playback-state%20playlist-read-private%20playlist-read-collaborative";

    return url.str();
    }

    void Auth::login() {
        std::thread serverThread([] {
            spotify::startServer();
        });
        openAuthURL();
        std::unique_lock<std::mutex> lock(spotify::authMutex);
        spotify::authCV.wait(lock, [] {
            return spotify::authReady;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        spotify::server.stop();

        serverThread.join();
        exchangeCode(spotify::authCode);
        saveTokens();
    }

    void Auth::openAuthURL() const {
        std::string url = getAuthURL();
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }

    void Auth::exchangeCode(const std::string& code) {
        httplib::Client client("https://accounts.spotify.com");
        client.set_basic_auth(client_id, client_secret);

        std::string body = "grant_type=authorization_code&code=" + code + "&redirect_uri=" + redirect_uri;
        auto response = client.Post("/api/token", body, "application/x-www-form-urlencoded");

        if (!response || response->status != 200) {
            throw std::runtime_error("Failed to exchange Spotify code");
        }

        auto json = nlohmann::json::parse(response->body);
        access_token = json["access_token"];
        refresh_token = json["refresh_token"];
        expires_at = std::chrono::system_clock::now() + std::chrono::seconds(json["expires_in"]);
    }

    void Auth::saveTokens() const {
        nlohmann::json tokens;

        tokens["access_token"] = access_token;
        tokens["refresh_token"] = refresh_token;
        tokens["expires_at"] = std::chrono::duration_cast<std::chrono::seconds>(expires_at.time_since_epoch()).count();

        std::ofstream file("tokens.json");
        file << tokens.dump(4);
    }

    bool Auth::loadTokens() {
        std::ifstream file("tokens.json");
        if (!file.is_open()) {
            return false;
        }

        nlohmann::json tokens;
        file >> tokens;

        access_token = tokens["access_token"];
        refresh_token = tokens["refresh_token"];
        expires_at = std::chrono::system_clock::time_point(std::chrono::seconds(tokens["expires_at"].get<long long>()));
        return true;
    }

    void Auth::refreshTokens() {
        if (std::chrono::system_clock::now() + std::chrono::seconds(60) > expires_at) {
            httplib::Client client("https://accounts.spotify.com");
            client.set_basic_auth(client_id, client_secret);

            std::string body = "grant_type=refresh_token&refresh_token=" + refresh_token;
            std::cout << "Sending request to Spotify API with body: " << body << std::endl;
            auto response = client.Post("/api/token", body, "application/x-www-form-urlencoded");

            if (!response || response->status != 200) {
                std::cerr << "Status code: " << response->status << std::endl;
                std::cerr << "Response body: " << response->body << std::endl;
                throw std::runtime_error("Failed to refresh Spotify token");
            }

            auto json = nlohmann::json::parse(response->body);
            access_token = json["access_token"];
            expires_at = std::chrono::system_clock::now() + std::chrono::seconds(json["expires_in"]);
            saveTokens();
        }

    }

    std::string Auth::getAccessToken() {
        refreshTokens();
        return access_token;
    }
}