#include "spotify/auth.hpp"
#include "spotify/api.hpp"
#include "librespot/librespot.hpp"
#include "env.hpp"

#include <thread>
#include <chrono>

int main() {
    Env env = loadEnv();
    
    try {
        spotify::Auth auth(
            env.client_id,
            env.client_secret,
            env.redirect_uri
        );
        librespot::start();
        spotify::Api api(auth);
        
        std::string deviceId;

        while (true) {
            auto devices = api.getDevices();
            for (const auto& device : devices) {
                if (device.name == "PengPlayer") {
                    deviceId = device.id;
                    goto device_found;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        device_found:
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto tracks = api.searchTracks("Imagine Dragons Believer");
        if (!tracks.empty()) {
            api.playTrack(deviceId, tracks[0].uri);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}