#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include <vector>
#include <string>

class RenderBuffer {
public:
    std::vector<std::string> lines;

    void clear();
    void pushLine(const std::string& line);
    void flushToTerminal() const;
};

#endif

