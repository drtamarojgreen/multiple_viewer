#ifndef WEB_SERVER_STUB_H
#define WEB_SERVER_STUB_H

#include <string>

namespace io {

class WebServerStub {
public:
    static void start(int port);
    static void stop();
    static void handleRequest(const std::string& path);
};

} // namespace io

#endif // WEB_SERVER_STUB_H
