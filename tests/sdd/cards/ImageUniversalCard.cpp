#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include "io/image_manager.h"
#include "render/image_renderer.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

struct VerificationTask {
    std::string name;
    bool is_io;
    int id;
};

void run_universal_sdd() {
    io::ImageManager manager;
    render::ImageRenderer renderer;
    renderer.initialize();

    std::vector<VerificationTask> tasks = {
        {"V1_SingleLaunch", true, (int)io::ImageManager::VerificationID::V1_SingleLaunch},
        {"V2_MultiStream", true, (int)io::ImageManager::VerificationID::V2_MultiStream},
        {"V3_EmptyDir", true, (int)io::ImageManager::VerificationID::V3_EmptyDir},
        {"V4_Corrupted", true, (int)io::ImageManager::VerificationID::V4_Corrupted},
        {"V5_Unsupported", true, (int)io::ImageManager::VerificationID::V5_Unsupported},
        {"V6_LargeImage", true, (int)io::ImageManager::VerificationID::V6_LargeImage},
        {"V7_RapidSwitchLeaks", true, (int)io::ImageManager::VerificationID::V7_RapidSwitchLeaks},
        {"V8_BufferDisposal", true, (int)io::ImageManager::VerificationID::V8_BufferDisposal},
        {"V9_AspectRatio", false, (int)render::ImageRenderer::VerificationID::V9_AspectRatio},
        {"V10_FullscreenScale", false, (int)render::ImageRenderer::VerificationID::V10_FullscreenScale},
        {"V11_ZoomIn", false, (int)render::ImageRenderer::VerificationID::V11_ZoomIn},
        {"V12_ZoomOut", false, (int)render::ImageRenderer::VerificationID::V12_ZoomOut},
        {"V13_ZoomReset", false, (int)render::ImageRenderer::VerificationID::V13_ZoomReset},
        {"V14_Panning", false, (int)render::ImageRenderer::VerificationID::V14_Panning},
        {"V15_Rotation", false, (int)render::ImageRenderer::VerificationID::V15_Rotation},
        {"V16_FlipH", false, (int)render::ImageRenderer::VerificationID::V16_FlipH},
        {"V17_FlipV", false, (int)render::ImageRenderer::VerificationID::V17_FlipV},
        {"V18_Grayscale", false, (int)render::ImageRenderer::VerificationID::V18_Grayscale},
        {"V19_Brightness", false, (int)render::ImageRenderer::VerificationID::V19_Brightness},
        {"V20_Contrast", false, (int)render::ImageRenderer::VerificationID::V20_Contrast},
        {"V21_HistMono", false, (int)render::ImageRenderer::VerificationID::V21_HistMono},
        {"V22_HistRgb", false, (int)render::ImageRenderer::VerificationID::V22_HistRgb},
        {"V23_Alpha", true, (int)io::ImageManager::VerificationID::V23_Alpha},
        {"V24_ProgressiveJpeg", true, (int)io::ImageManager::VerificationID::V24_ProgressiveJpeg},
        {"V25_GifAnim", true, (int)io::ImageManager::VerificationID::V25_GifAnim},
        {"V26_GifControl", true, (int)io::ImageManager::VerificationID::V26_GifControl},
        {"V27_TiffMulti", true, (int)io::ImageManager::VerificationID::V27_TiffMulti},
        {"V28_ExifMissing", true, (int)io::ImageManager::VerificationID::V28_ExifMissing},
        {"V29_ExifAuto", true, (int)io::ImageManager::VerificationID::V29_ExifAuto},
        {"V30_DragDropMulti", true, (int)io::ImageManager::VerificationID::V30_DragDropMulti},
        {"V31_DragDropReject", true, (int)io::ImageManager::VerificationID::V31_DragDropReject},
        {"V32_Shortcuts", false, (int)render::ImageRenderer::VerificationID::V32_Shortcuts},
        {"V33_ShortcutsLoad", false, (int)render::ImageRenderer::VerificationID::V33_ShortcutsLoad},
        {"V34_MouseWheel", false, (int)render::ImageRenderer::VerificationID::V34_MouseWheel},
        {"V35_FullscreenToggle", false, (int)render::ImageRenderer::VerificationID::V35_FullscreenToggle},
        {"V36_MultiMonitor", false, (int)render::ImageRenderer::VerificationID::V36_MultiMonitor},
        {"V37_ResizeRecalc", false, (int)render::ImageRenderer::VerificationID::V37_ResizeRecalc},
        {"V38_MinimizeRestore", false, (int)render::ImageRenderer::VerificationID::V38_MinimizeRestore},
        {"V39_DarkMode", false, (int)render::ImageRenderer::VerificationID::V39_DarkMode},
        {"V40_ThemeSwitch", false, (int)render::ImageRenderer::VerificationID::V40_ThemeSwitch},
        {"V41_I18nMissing", false, (int)render::ImageRenderer::VerificationID::V41_I18nMissing},
        {"V42_Utf8Paths", true, (int)io::ImageManager::VerificationID::V42_Utf8Paths},
        {"V43_UnicodeDir", true, (int)io::ImageManager::VerificationID::V43_UnicodeDir},
        {"V44_Symlinks", true, (int)io::ImageManager::VerificationID::V44_Symlinks},
        {"V45_PathTraversal", true, (int)io::ImageManager::VerificationID::V45_PathTraversal},
        {"V46_CacheCleanup", true, (int)io::ImageManager::VerificationID::V46_CacheCleanup},
        {"V47_ThreadSafeLoad", true, (int)io::ImageManager::VerificationID::V47_ThreadSafeLoad},
        {"V48_AsyncOrder", false, (int)render::ImageRenderer::VerificationID::V48_AsyncOrder},
        {"V49_RenderCancel", false, (int)render::ImageRenderer::VerificationID::V49_RenderCancel},
        {"V50_StartupTime", true, (int)io::ImageManager::VerificationID::V50_StartupTime},
        {"V51_ShutdownThreads", true, (int)io::ImageManager::VerificationID::V51_ShutdownThreads},
        {"V52_FileWatcherAdd", true, (int)io::ImageManager::VerificationID::V52_FileWatcherAdd},
        {"V53_FileWatcherDel", true, (int)io::ImageManager::VerificationID::V53_FileWatcherDel},
        {"V54_HotReload", true, (int)io::ImageManager::VerificationID::V54_HotReload},
        {"V55_DuplicateEntry", true, (int)io::ImageManager::VerificationID::V55_DuplicateEntry},
        {"V56_SlideshowInterval", true, (int)io::ImageManager::VerificationID::V56_SlideshowInterval},
        {"V57_SlideshowPause", true, (int)io::ImageManager::VerificationID::V57_SlideshowPause},
        {"V58_SlideshowMissing", true, (int)io::ImageManager::VerificationID::V58_SlideshowMissing},
        {"V59_SlideshowRandom", true, (int)io::ImageManager::VerificationID::V59_SlideshowRandom},
        {"V60_Bookmarks", true, (int)io::ImageManager::VerificationID::V60_Bookmarks},
        {"V61_SessionRestoration", true, (int)io::ImageManager::VerificationID::V61_SessionRestoration},
        {"V62_ConfigMalformed", true, (int)io::ImageManager::VerificationID::V62_ConfigMalformed},
        {"V63_ConfigDefaults", true, (int)io::ImageManager::VerificationID::V63_ConfigDefaults},
        {"V64_ConfigReload", true, (int)io::ImageManager::VerificationID::V64_ConfigReload},
        {"V65_CliOverride", true, (int)io::ImageManager::VerificationID::V65_CliOverride},
        {"V66_CliError", true, (int)io::ImageManager::VerificationID::V66_CliError},
        {"V67_Headless", true, (int)io::ImageManager::VerificationID::V67_Headless},
        {"V68_LogFailures", true, (int)io::ImageManager::VerificationID::V68_LogFailures},
        {"V69_LogProd", true, (int)io::ImageManager::VerificationID::V69_LogProd},
        {"V70_LogRotation", true, (int)io::ImageManager::VerificationID::V70_LogRotation},
        {"V74_Fallback", false, (int)render::ImageRenderer::VerificationID::V74_Fallback},
        {"V75_GpuInit", false, (int)render::ImageRenderer::VerificationID::V75_GpuInit},
        {"V76_SoftwareParity", false, (int)render::ImageRenderer::VerificationID::V76_SoftwareParity},
        {"V77_TextureFail", false, (int)render::ImageRenderer::VerificationID::V77_TextureFail},
        {"V78_FrameTiming", false, (int)render::ImageRenderer::VerificationID::V78_FrameTiming},
        {"V79_FpsCounter", false, (int)render::ImageRenderer::VerificationID::V79_FpsCounter},
        {"V80_MemoryUsage", true, (int)io::ImageManager::VerificationID::V80_MemoryUsage},
        {"V81_Survival24h", true, (int)io::ImageManager::VerificationID::V81_Survival24h},
        {"V82_Stress10k", true, (int)io::ImageManager::VerificationID::V82_Stress10k},
        {"V83_RaceConditions", true, (int)io::ImageManager::VerificationID::V83_RaceConditions},
        {"V84_ThreadPoolExhaustion", true, (int)io::ImageManager::VerificationID::V84_ThreadPoolExhaustion},
        {"V85_InvalidShader", false, (int)render::ImageRenderer::VerificationID::V85_InvalidShader},
        {"V86_ScreenshotIdentical", true, (int)io::ImageManager::VerificationID::V86_ScreenshotIdentical},
        {"V87_ScreenshotAlpha", true, (int)io::ImageManager::VerificationID::V87_ScreenshotAlpha},
        {"V88_ClipboardLarge", true, (int)io::ImageManager::VerificationID::V88_ClipboardLarge},
        {"V89_ClipboardPaste", true, (int)io::ImageManager::VerificationID::V89_ClipboardPaste},
        {"V90_AutosaveRecovery", true, (int)io::ImageManager::VerificationID::V90_AutosaveRecovery},
        {"V91_SandboxExec", true, (int)io::ImageManager::VerificationID::V91_SandboxExec},
        {"V92_MalformedHeader", true, (int)io::ImageManager::VerificationID::V92_MalformedHeader},
        {"V93_DecompressionBomb", true, (int)io::ImageManager::VerificationID::V93_DecompressionBomb},
        {"V94_ResourceQuota", true, (int)io::ImageManager::VerificationID::V94_ResourceQuota},
        {"V95_DeterministicDistro", false, (int)render::ImageRenderer::VerificationID::V95_DeterministicDistro},
        {"V96_WinPaths", true, (int)io::ImageManager::VerificationID::V96_WinPaths},
        {"V97_MacRetina", false, (int)render::ImageRenderer::VerificationID::V97_MacRetina},
        {"V98_CiIntegration", true, (int)io::ImageManager::VerificationID::V98_CiIntegration},
        {"V99_ReleaseBuild", true, (int)io::ImageManager::VerificationID::V99_ReleaseBuild},
        {"V100_ReproducibleBuild", true, (int)io::ImageManager::VerificationID::V100_ReproducibleBuild}
    };

    std::cout << "--- UNIVERSAL SDD EMPIRICAL REPORT ---" << std::endl;
    for (const auto& task : tasks) {
        auto start = std::chrono::high_resolution_clock::now();
        bool result = task.is_io ? manager.verify((io::ImageManager::VerificationID)task.id)
                                 : renderer.verify((render::ImageRenderer::VerificationID)task.id);
        auto end = std::chrono::high_resolution_clock::now();
        long long us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << task.name << " = " << (result ? "PASS" : "FAIL")
                  << " [Exec: " << us << "us]" << std::endl;
    }
}

int main() {
    run_universal_sdd();
    return 0;
}
