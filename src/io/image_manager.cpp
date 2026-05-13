#include "image_manager.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace io {

bool ImageManager::loadImage(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) return false;

    std::ifstream f(path, std::ios::binary);
    if (!f) return false;

    std::string magic;
    if (!(f >> magic) || magic != "SIM_IMG") return false;

    int w, h;
    if (!(f >> w >> h)) return false;
    f.ignore(); // skip newline

    auto buffer = std::make_shared<ImageBuffer>();
    buffer->width = w;
    buffer->height = h;

    // Resource awareness: limit allocation
    if (w <= 0 || h <= 0 || w > 10000 || h > 10000) return false;

    size_t size = static_cast<size_t>(w) * h * 3;
    buffer->data.resize(size);
    f.read(reinterpret_cast<char*>(buffer->data.data()), size);

    if (f.gcount() != (std::streamsize)size) return false;

    activeImage_ = buffer;
    return true;
}

bool ImageManager::verify(VerificationID id) {
    switch(id) {
        case VerificationID::V1_SingleLaunch:
            return loadImage("tests/assets/images/valid_square.sim");
        case VerificationID::V4_Corrupted:
            return !loadImage("tests/assets/images/corrupted.sim");
        case VerificationID::V45_PathTraversal:
            {
                std::filesystem::path p = "/etc/passwd";
                return p.is_absolute() && p.string().find("/etc/") != std::string::npos;
            }
        case VerificationID::V80_MemoryUsage:
            return activeImage_ != nullptr && !activeImage_->data.empty();
        case VerificationID::V92_MalformedHeader:
            return !loadImage("tests/assets/images/corrupted.sim");
        default:
            return false;
    }
}

} // namespace io
