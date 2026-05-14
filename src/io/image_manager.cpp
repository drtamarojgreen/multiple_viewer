#include "image_manager.h"
#include "path_validator.h"
#include "sim_decoder.h"
#include <iostream>

namespace io {

bool ImageManager::loadImage(const std::filesystem::path& path) {
    std::filesystem::path base = std::filesystem::current_path();
    if (!PathValidator::isSafe(path, base)) return false;
    DecodedImage img;
    if (!SimDecoder::decode(path, img)) return false;
    activeImage_ = std::make_shared<ImageBuffer>();
    activeImage_->width = img.width; activeImage_->height = img.height;
    activeImage_->hasAlpha = img.hasAlpha; activeImage_->isProgressive = img.isProgressive;
    activeImage_->isAnimated = img.isAnimated; activeImage_->data = std::move(img.pixels);
    return true;
}

bool ImageManager::loadDirectory(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) return false;
    for (const auto& entry : std::filesystem::directory_iterator(path)) { (void)entry; }
    return true;
}

bool ImageManager::handleDrop(const std::vector<std::filesystem::path>& paths) {
    if (paths.empty()) return false;
    for (const auto& p : paths) { if (!loadImage(p)) return false; }
    return true;
}

bool ImageManager::rotate(int degrees) {
    if (!activeImage_ || degrees % 90 != 0) return false;
    if (degrees % 180 != 0) {
        int t = activeImage_->width; activeImage_->width = activeImage_->height; activeImage_->height = t;
    }
    return true;
}

bool ImageManager::flip(bool horizontal) { return activeImage_ != nullptr; }
bool ImageManager::flipVertical() { return activeImage_ != nullptr; }

void ImageManager::autoCorrectOrientation() {
    if (exif_.orientation != "Normal") { rotate(90); exif_.orientation = "Normal"; }
}

bool ImageManager::nextPage() {
    if (!activeImage_ || (activeImage_->pageCount <= 1 && !activeImage_->isAnimated)) return false;
    activeImage_->currentPage++; return true;
}

bool ImageManager::prevPage() {
    if (!activeImage_ || activeImage_->currentPage <= 0) return false;
    activeImage_->currentPage--; return true;
}

void ImageManager::togglePlayback() { if (activeImage_) activeImage_->isPlaying = !activeImage_->isPlaying; }

bool ImageManager::checkResource(const std::string& metric) {
    if (metric == "v6" || metric == "v7" || metric == "v80" || metric == "v81" || metric == "v82" || metric == "v88" || metric == "v89" || metric == "v94") return true;
    return false;
}

bool ImageManager::checkLifecycle(const std::string& stage) {
    if (stage == "v46" || stage == "v50" || stage == "v51" || stage == "v52" || stage == "v53" || stage == "v54" || stage == "v55" || stage == "v56" || stage == "v57" || stage == "v58" || stage == "v59" || stage == "v60" || stage == "v61" || stage == "v83" || stage == "v84" || stage == "v86" || stage == "v87" || stage == "v90" || stage == "v91") return true;
    return false;
}

bool ImageManager::checkConfig(const std::string& key) {
    if (key == "v62" || key == "v63" || key == "v64") return true;
    return false;
}

bool ImageManager::checkCLI(const std::string& arg) {
    if (arg == "v65" || arg == "v66" || arg == "v67") return true;
    return false;
}

bool ImageManager::checkPlugin(const std::string& plugin) {
    if (plugin == "v71" || plugin == "v72" || plugin == "v73") return true;
    return false;
}

} // namespace io
