#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <string>
#include <vector>
#include <filesystem>
#include <memory>

namespace io {

struct ImageBuffer {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> data;
    bool hasAlpha = false;
    bool isProgressive = false;
    int pageCount = 1;
    int currentPage = 0;
    bool isAnimated = false;
    bool isPlaying = false;
};

struct ExifData {
    std::string orientation = "Normal";
};

class ImageManager {
public:
    bool loadImage(const std::filesystem::path& path);
    bool loadDirectory(const std::filesystem::path& path);
    bool handleDrop(const std::vector<std::filesystem::path>& paths);

    bool rotate(int degrees);
    bool flip(bool horizontal);
    bool flipVertical();

    // EXIF and Metadata
    ExifData getExif() { return exif_; }
    void setExif(const ExifData& d) { exif_ = d; }
    void autoCorrectOrientation();

    // Multipage / Animation
    bool nextPage();
    bool prevPage();
    void togglePlayback();

    std::shared_ptr<ImageBuffer> getActiveImage() { return activeImage_; }
    void closeImage() { activeImage_.reset(); }

    // System Check Hooks (Manual SDD Enforcement)
    bool checkResource(const std::string& metric);
    bool checkLifecycle(const std::string& stage);
    bool checkConfig(const std::string& key);
    bool checkCLI(const std::string& arg);
    bool checkPlugin(const std::string& plugin);

private:
    std::shared_ptr<ImageBuffer> activeImage_;
    ExifData exif_;
};

} // namespace io

#endif
