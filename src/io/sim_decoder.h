#ifndef SIM_DECODER_H
#define SIM_DECODER_H

#include <vector>
#include <filesystem>
#include <string>

namespace io {

struct DecodedImage {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> pixels;
    bool hasAlpha = false;
    bool isProgressive = false;
    bool isAnimated = false;
};

class SimDecoder {
public:
    static bool decode(const std::filesystem::path& path, DecodedImage& out);
};

} // namespace io

#endif
