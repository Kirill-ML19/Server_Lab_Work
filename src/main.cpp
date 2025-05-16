#include "server.h"
#include "utils.h"
#include <thread>

int main() {
    init_logs();

    const int port = 5555;
    Server server(port);
    log("Starting Cybernetic Implant Server...", LogLevel::INFO);

    server.run();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server.stop();
    close_logs();
    return 0;
}
