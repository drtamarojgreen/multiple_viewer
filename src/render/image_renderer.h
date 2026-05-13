#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include <string>

namespace render {

class ImageRenderer {
public:
    enum class VerificationID {
        V9_AspectRatio, V10_FullscreenScale, V11_ZoomIn, V12_ZoomOut, V13_ZoomReset, V14_Panning,
        V15_Rotation, V16_FlipH, V17_FlipV, V18_Grayscale, V19_Brightness, V20_Contrast,
        V21_HistMono, V22_HistRgb,
        V32_Shortcuts, V33_ShortcutsLoad, V34_MouseWheel, V35_FullscreenToggle,
        V36_MultiMonitor, V37_ResizeRecalc, V38_MinimizeRestore, V39_DarkMode, V40_ThemeSwitch,
        V41_I18nMissing, V48_AsyncOrder, V49_RenderCancel,
        V74_Fallback, V75_GpuInit, V76_SoftwareParity, V77_TextureFail,
        V78_FrameTiming, V79_FpsCounter, V85_InvalidShader,
        V95_DeterministicDistro, V97_MacRetina
    };

    bool verify(VerificationID id);
    bool initialize();

private:
    bool initialized_ = false;
};

} // namespace render

#endif // IMAGE_RENDERER_H
