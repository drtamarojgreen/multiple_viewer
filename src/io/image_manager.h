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
};

class ImageManager {
public:
    enum class VerificationID {
        V1_SingleLaunch, V2_MultiStream, V3_EmptyDir, V4_Corrupted, V5_Unsupported,
        V6_LargeImage, V7_RapidSwitchLeaks, V8_BufferDisposal,
        V23_Alpha, V24_ProgressiveJpeg, V25_GifAnim, V26_GifControl, V27_TiffMulti,
        V28_ExifMissing, V29_ExifAuto, V30_DragDropMulti, V31_DragDropReject,
        V42_Utf8Paths, V43_UnicodeDir, V44_Symlinks, V45_PathTraversal, V46_CacheCleanup,
        V47_ThreadSafeLoad, V50_StartupTime, V51_ShutdownThreads,
        V52_FileWatcherAdd, V53_FileWatcherDel, V54_HotReload, V55_DuplicateEntry,
        V56_SlideshowInterval, V57_SlideshowPause, V58_SlideshowMissing, V59_SlideshowRandom,
        V60_Bookmarks, V61_SessionRestoration,
        V62_ConfigMalformed, V63_ConfigDefaults, V64_ConfigReload, V65_CliOverride, V66_CliError, V67_Headless,
        V68_LogFailures, V69_LogProd, V70_LogRotation,
        V80_MemoryUsage, V81_Survival24h, V82_Stress10k, V83_RaceConditions, V84_ThreadPoolExhaustion,
        V86_ScreenshotIdentical, V87_ScreenshotAlpha, V88_ClipboardLarge, V89_ClipboardPaste,
        V90_AutosaveRecovery, V91_SandboxExec, V92_MalformedHeader, V93_DecompressionBomb, V94_ResourceQuota,
        V96_WinPaths, V98_CiIntegration, V99_ReleaseBuild, V100_ReproducibleBuild
    };

    bool verify(VerificationID id);

    // Core IO
    bool loadImage(const std::filesystem::path& path);
    std::shared_ptr<ImageBuffer> getActiveImage() { return activeImage_; }

private:
    std::shared_ptr<ImageBuffer> activeImage_;
};

} // namespace io

#endif // IMAGE_MANAGER_H
