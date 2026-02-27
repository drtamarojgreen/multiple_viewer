#ifndef UI_PRINTER_H
#define UI_PRINTER_H

#include "../../src/render/render_interface.h"
#include <string>
#include <sstream>
#include <vector>

namespace print {

class UIPrinter : public render::IRenderer {
public:
    UIPrinter() = default;
    ~UIPrinter() override = default;

    bool initialize(int width, int height) override;
    void clear() override;
    void render(const Graph& graph, const ViewContext& view) override;
    void present() override;
    void shutdown() override;

    bool isWindowOpen() const override { return true; }

    std::string getPrintedOutput() const;

private:
    int width_ = 80;
    int height_ = 25;
    std::stringstream outputBuffer_;

    std::string resolveNodeColor(int weight) const;
};

} // namespace print

#endif // UI_PRINTER_H
