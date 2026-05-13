#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include <string>

namespace render {

class ImageRenderer {
public:
    enum class RenderPath {
        None,
        Software,
        GPU
    };

    // V75
    bool initialize();

    // V9, V10, V36, V37, V38, V39, V40, V76, V97
    bool testLayout(const std::string& feature);

    // V11, V12, V13, V34
    bool testZoom(float level);

    // V14
    bool testPanning(int x, int y);

    // V18, V19, V20
    bool applyFilter(const std::string& name);

    // V21, V22
    bool testAnalytics(const std::string& metric);

    // V32, V33, V35
    bool testInput(const std::string& action);

    // V41
    bool testLocalization(const std::string& key);

    // V48, V49
    bool testRenderQueue(const std::string& mode);

    // V74, V77, V85
    bool testGPU(const std::string& state);

    // V78, V79, V95
    bool testPerformance(const std::string& metric);

    float getFPS() const { return 0.0f; }
    RenderPath getActivePath() const { return path_; }

private:
    RenderPath path_ = RenderPath::None;
};

} // namespace render

#endif // IMAGE_RENDERER_H
