#include "sim_decoder.h"
#include <fstream>
#include <iostream>

namespace io {

bool SimDecoder::decode(const std::filesystem::path& path, DecodedImage& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;

    std::string magic;
    if (!(f >> magic) || magic != "SIM_IMG") return false;

    int w, h;
    if (!(f >> w >> h)) return false;
    f.ignore();

    if (w <= 0 || h <= 0 || w > 10000 || h > 10000) return false;

    out.width = w;
    out.height = h;

    std::string body;
    std::getline(f, body);
    if (body.find("[A]") != std::string::npos) out.hasAlpha = true;
    if (body.find("[P]") != std::string::npos) out.isProgressive = true;
    if (body.find("[G]") != std::string::npos) out.isAnimated = true;

    return true;
}

} // namespace io
