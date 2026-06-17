#pragma once
#include "httplib.h"

#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

namespace spotify {
    extern std::string authCode;
    extern std::mutex authMutex;
    extern std::condition_variable authCV;
    extern bool authReady;
    extern httplib::Server server;

    void startServer();
    void stopServer();
}