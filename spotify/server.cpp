#include "server.hpp"

namespace spotify {
    std::string authCode;
    std::mutex authMutex;
    std::condition_variable authCV;
    bool authReady = false;

    httplib::Server server;

    void startServer() {
        server.Get("/callback", [](const httplib::Request& req, httplib::Response& res) {

            if (!req.has_param("code")) {
                res.status = 400;
                res.set_content("Missing code", "text/plain");
                return;
            }

            std::lock_guard<std::mutex> lock(authMutex);
            authCode = req.get_param_value("code");
            authReady = true;

            authCV.notify_one();

            res.set_content(R"(
                <html>
                    <body>
                        <h2>Login successful</h2>
                        <p>You can close this tab.</p>
                    </body>
                </html>
            )", "text/html");
        });

        server.listen("127.0.0.1", 8888);
    };

    void stopServer() {
        server.stop();
    }
}