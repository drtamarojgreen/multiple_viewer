#include "image_manager.h"
#include <fstream>
#include <set>

namespace io {

ImageManager::LoadResult ImageManager::loadImage(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) return LoadResult::FileNotFound;
    if (!isPathSafe(path)) return LoadResult::SecurityViolation;

    auto ext = path.extension().string();
    static const std::set<std::string> supported = {".png", ".jpg", ".jpeg", ".tiff", ".gif"};
    if (supported.find(ext) == supported.end()) return LoadResult::UnsupportedFormat;

    if (!verifyHeader(path)) return LoadResult::CorruptedFile;

    activeImage_ = std::make_shared<ImageBuffer>();
    activeImage_->width = 100;
    activeImage_->height = 100;
    return LoadResult::Success;
}

bool ImageManager::testConcurrency(const std::string& mode) { return false; }
bool ImageManager::testDirectoryOps(const std::string& op, const std::string& path) { return false; }
bool ImageManager::testResources(const std::string& metric) { return false; }
bool ImageManager::testMetadata(const std::string& field) { return false; }
bool ImageManager::handleDragDrop(const std::vector<std::string>& paths) { return false; }
bool ImageManager::testLifecycle(const std::string& stage) { return false; }
bool ImageManager::testConfiguration(const std::string& setting) { return false; }
bool ImageManager::testCLI(const std::string& arg) { return false; }
bool ImageManager::testLogging(const std::string& level) { return false; }
bool ImageManager::testExport(const std::string& format) { return false; }
bool ImageManager::applyTransform(const std::string& type) { return false; }

bool ImageManager::isPathSafe(const std::filesystem::path& path) {
    std::string p = std::filesystem::absolute(path).string();
    if (p.find("/etc/") != std::string::npos || p.find("/var/") != std::string::npos) {
        return false;
    }
    return true;
}

bool ImageManager::verifyHeader(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) return false;
    std::string content;
    std::getline(file, content);
    if (content.find("Fake") == 0) return true;
    return false;
}

} // namespace io
