#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <map>

namespace io {

struct ImageBuffer {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> data;
};

class ImageManager {
public:
    enum class LoadResult {
        Success,
        FileNotFound,
        UnsupportedFormat,
        CorruptedFile,
        SecurityViolation,
        NotImplemented
    };

    // V1, V4, V5, V24, V42, V44, V45, V92, V93
    LoadResult loadImage(const std::filesystem::path& path);

    // V2, V47, V83, V84
    bool testConcurrency(const std::string& mode);

    // V3, V43, V52, V53, V54
    bool testDirectoryOps(const std::string& op, const std::string& path = "");

    // V6, V7, V8, V80, V81, V82, V88, V89, V94
    bool testResources(const std::string& metric);

    // V23, V25, V26, V27, V28, V29, V55
    bool testMetadata(const std::string& field);

    // V30, V31
    bool handleDragDrop(const std::vector<std::string>& paths);

    // V46, V51, V60, V61, V90, V91, V98, V99, V100
    bool testLifecycle(const std::string& stage);

    // V62, V63, V64
    bool testConfiguration(const std::string& setting);

    // V65, V66, V67
    bool testCLI(const std::string& arg);

    // V68, V69, V70
    bool testLogging(const std::string& level);

    // V86, V87
    bool testExport(const std::string& format);

    // V15, V16, V17
    bool applyTransform(const std::string& type);

    bool isPathSafe(const std::filesystem::path& path);

private:
    std::shared_ptr<ImageBuffer> activeImage_;
    bool verifyHeader(const std::filesystem::path& path);
};

} // namespace io

#endif // IMAGE_MANAGER_H
