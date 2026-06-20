#pragma once
#include "auth.hpp"
#include "endpoints.hpp"

#include <string>
#include <vector>

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace spotify
{
    struct Track
    {
        std::string id;
        std::string name;
        std::string artist;
        std::string uri;
    };

    struct Playlist
    {
        std::string id;
        std::string name;
        std::string uri;
    };

    struct Device
    {
        std::string id;
        std::string name;
        bool active{};
    };

    class Api
    {
    public:
        explicit Api(const Auth& auth): auth_(auth) {};

        std::vector<Track> searchTracks(
            const std::string& query,
            int limit = 5);

        std::vector<Playlist> getPlaylists(
            int limit = 5,
            int offset = 0);

        std::vector<Device> getDevices();

        bool playTrack(
            const std::string& deviceId,
            const std::string& trackUri);

        bool pause();
        bool resume();
        bool next();
        bool previous();

    private:
        Auth auth_;
    };
}