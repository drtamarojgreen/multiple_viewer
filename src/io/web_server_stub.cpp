#include "web_server_stub.h"
#include <iostream>

namespace io {

void WebServerStub::start(int port) {
    std::cout << "[Web] Starting stub server on port " << port << "...\n";
}

void WebServerStub::stop() {
    std::cout << "[Web] Stopping stub server...\n";
}

void WebServerStub::handleRequest(const std::string& path) {
    std::cout << "[Web] Handling request for: " << path << "\n";
}

} // namespace io
