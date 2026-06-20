#include "api.hpp"

#include <iostream>

namespace spotify
{
    std::vector<Track> Api::searchTracks(const std::string& query, int limit) {
        httplib::Client client(API_URL);

        httplib::Params params = {
            {"q", query},
            {"type", "track"},
            {"limit", std::to_string(limit)}
        };

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        auto response = client.Get("/v1/search", params, headers);

        if (!response) {
            throw std::runtime_error("Failed to connect to Spotify API");
        }

        auto json = nlohmann::json::parse(response->body);

        if (!json.contains("tracks") || !json["tracks"].contains("items")) {
            
            throw std::runtime_error("Invalid response from Spotify API: " + json.dump());
        }

        std::vector<Track> tracks;

        for (const auto& item : json["tracks"]["items"]) {
            tracks.push_back({
                item["id"].get<std::string>(),
                item["name"].get<std::string>(),
                item["artists"][0]["name"].get<std::string>(),
                item["uri"].get<std::string>()
            });
        }

        return tracks;
    }

    std::vector<Playlist> Api::getPlaylists(int limit, int offset) {
        httplib::Client client(API_URL);

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        auto response = client.Get("/v1/me/playlists?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset), headers);

        if (!response) {
            throw std::runtime_error("Failed to connect to Spotify API");
        }

        auto json = nlohmann::json::parse(response->body);

        if (!json.contains("items")) {
            throw std::runtime_error("Invalid response from Spotify API: " + json.dump());
        }

        std::vector<Playlist> playlists;

        for (const auto& item : json["items"]) {
            playlists.push_back({
                item["id"].get<std::string>(),
                item["name"].get<std::string>(),
                item["uri"].get<std::string>()
            });
        }

        return playlists;
    }

    std::vector<Device> Api::getDevices() {
        httplib::Client client(API_URL);

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        auto response = client.Get("/v1/me/player/devices", headers);

        if (!response) {
            throw std::runtime_error("Failed to connect to Spotify API");
        }

        auto json = nlohmann::json::parse(response->body);

        if (!json.contains("devices")) {
            throw std::runtime_error("Invalid response from Spotify API: " + json.dump());
        }
        std::vector<Device> devices;

        for (const auto& item : json["devices"]) {
            devices.push_back({
                item["id"].get<std::string>(),
                item["name"].get<std::string>(),
                item["is_active"].get<bool>()
            });
        }

        return devices;
    }

    bool Api::playTrack(const std::string& deviceId, const std::string& trackUri) {
        httplib::Client client(API_URL);

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        nlohmann::json body = {
            {"uris", {trackUri}}
        };
        auto response = client.Put("/v1/me/player/play?device_id=" + deviceId, headers, body.dump(), "application/json");

        return response && response->status == 204;
    }

    bool Api::pause() {
        httplib::Client client(API_URL);

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        auto response = client.Put("/v1/me/player/pause", headers);
        return response && response->status == 204;
    }

    bool Api::resume() {
        httplib::Client client(API_URL);

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        auto response = client.Put("/v1/me/player/play", headers);
        return response && response->status == 204;
    }

    bool Api::next() {
        httplib::Client client(API_URL);

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        auto response = client.Post("/v1/me/player/next", headers);
        return response && response->status == 204;
    }

    bool Api::previous() {
        httplib::Client client(API_URL);

        httplib::Headers headers = {{"Authorization", "Bearer " + auth_.getAccessToken()}};
        auto response = client.Post("/v1/me/player/previous", headers);
        return response && response->status == 204;
    }
}