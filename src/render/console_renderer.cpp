#include "console_renderer.h"
#include "layout/layout_manager.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <limits>
#include <algorithm>
#include <map>

namespace render {

bool ConsoleRenderer::initialize(int width, int height) {
    width_ = width;
    height_ = height;
    frameBuffer_ = std::make_unique<FrameBuffer>(width, height);
    viewport_ = std::make_unique<Viewport>(width, height);
    return true;
}

void ConsoleRenderer::clear() {
    if (frameBuffer_) {
        frameBuffer_->clear();
    }
}

void ConsoleRenderer::render(const Graph& graph, const ViewContext& view) {
    if (!frameBuffer_ || !viewport_) return;

    Graph& mutableGraph = const_cast<Graph&>(graph);
    ViewContext& mutableView = const_cast<ViewContext&>(view);

    viewport_->setPan(view.panX, view.panY);
    // Note: Zoom handling might need adjustment if viewport zoom is float and graph zoom is enum

    if (view.currentViewMode == VM_PERSPECTIVE) {
        layout::LayoutManager::applyPerspectiveBFS(mutableGraph, mutableView);
        frameBuffer_->setTitle("CBT Graph Viewer (Full Layout)");
    } else if (view.currentViewMode == VM_NEXUS_FLOW) {
        layout::LayoutManager::applyForceDirected(mutableGraph, mutableView);
        frameBuffer_->setTitle("CBT Graph Viewer (Nexus Flow)");
    } else if (view.currentViewMode == VM_BOOK_VIEW) {
        frameBuffer_->setTitle("CBT Graph Viewer (Book View)");
    }

    // Render Edges
    for (const auto& node : graph.nodes) {
        if (graph.layoutPositions.count(node.index) == 0) continue;
        Point2D p1 = viewport_->worldToScreen(graph.layoutPositions.at(node.index).x, graph.layoutPositions.at(node.index).y);

        for (int neighbor_id : node.neighbors) {
            if (graph.layoutPositions.count(neighbor_id) == 0) continue;
            Point2D p2 = viewport_->worldToScreen(graph.layoutPositions.at(neighbor_id).x, graph.layoutPositions.at(neighbor_id).y);

            frameBuffer_->drawLine(static_cast<int>(p1.x), static_cast<int>(p1.y),
                                   static_cast<int>(p2.x), static_cast<int>(p2.y), '.', 1.0f);
        }
    }

    // Render Nodes
    for (const auto& node : graph.nodes) {
        if (graph.layoutPositions.count(node.index) == 0) continue;
        Point2D p = viewport_->worldToScreen(graph.layoutPositions.at(node.index).x, graph.layoutPositions.at(node.index).y);

        int depth = 0;
        if (graph.nodePos.count(node.index)) {
            depth = static_cast<int>(graph.nodePos.at(node.index).z);
        }

        int size = graph.calculateNodeSize(depth, view.zoomLevel);
        char glyph = (graph.isNodeFocused(node.index)) ? 'O' :
                     (node.subjectIndex % 4 == 0 ? '@' :
                      node.subjectIndex % 4 == 1 ? '#' : 'X');

        if (size <= 1) {
            frameBuffer_->drawChar(static_cast<int>(p.x), static_cast<int>(p.y), glyph, static_cast<float>(depth));
        } else {
            frameBuffer_->drawRect(static_cast<int>(p.x - size / 2), static_cast<int>(p.y - size / 2), size, size, glyph, static_cast<float>(depth));
        }

        // Label
        frameBuffer_->drawString(static_cast<int>(p.x + size / 2 + 1), static_cast<int>(p.y + size / 2 + 1), node.label, static_cast<float>(depth));
    }

    if (view.currentViewMode == VM_BOOK_VIEW) {
        auto chapters = createBookStructure(graph, view);
        int y = 2;
        for (const auto& ch : chapters) {
            frameBuffer_->drawString(2, y++, "-- " + ch.chapterTitle + " (Depth " + std::to_string(ch.chapterDepth) + ") --", -1.0f);
            for (int nodeId : ch.nodeIds) {
                bool isFocused = graph.isNodeFocused(nodeId);
                frameBuffer_->drawString(4, y++, (isFocused ? "> [" : "  [") + graph.nodeMap.at(nodeId).label + "]", -1.0f);
            }
            y++;
        }
    }

    if (view.showHelp) {
        int y = 2;
        frameBuffer_->drawRect(5, 1, 70, 12, '+', -1.0f);
        frameBuffer_->drawString(7, y++, "=== CBT Graph Viewer Menu ===", -1.0f);
        frameBuffer_->drawString(7, y++, "[↑↓←→/IJKL] Pan  [A] Add  [R] Remove  [F] Focus  [O] Unfocus", -1.0f);
        frameBuffer_->drawString(7, y++, "[T] Set Dist  [/] Search  [TAB] Cycle Focus  [B] Book View", -1.0f);
        frameBuffer_->drawString(7, y++, "[N] Next View [E] Page View [V] Page Cycle [Z] Zoom In [X] Zoom Out", -1.0f);
        frameBuffer_->drawString(7, y++, "[M] Multi Foci Toggle  [H] Toggle Help", -1.0f);
        frameBuffer_->drawString(7, y++, "[G] Analytics  [D] DepthScale  [W] Weights  [S] Save  [U] Load", -1.0f);
        frameBuffer_->drawString(7, y++, "[ESC] Exit", -1.0f);
    }
}

void ConsoleRenderer::present() {
    if (frameBuffer_) {
        frameBuffer_->present();
    }
}

void ConsoleRenderer::shutdown() {
}

void ConsoleRenderer::setStatusMessage(const std::string& message) {
    if (frameBuffer_) {
        frameBuffer_->setStatusMessage(message);
    }
}

} // namespace render
