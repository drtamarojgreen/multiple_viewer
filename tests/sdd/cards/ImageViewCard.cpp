#include <iostream>
#include <map>
#include "render/image_renderer.h"
#include "io/image_manager.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

void verify_view_batch(const std::map<std::string, std::string>& facts) {
    render::ImageRenderer renderer;
    io::ImageManager manager;
    renderer.initialize();

    std::cout << "v9_aspect_ratio_preserved = " << (renderer.testLayout("aspect_ratio") ? "true" : "false") << std::endl;
    std::cout << "v10_fullscreen_no_distortion = " << (renderer.testLayout("fullscreen") ? "true" : "false") << std::endl;
    std::cout << "v11_zoom_in_fidelity = " << (renderer.testZoom(2.0f) ? "true" : "false") << std::endl;
    std::cout << "v12_zoom_out_clamping = " << (renderer.testZoom(0.1f) ? "true" : "false") << std::endl;
    std::cout << "v13_zoom_reset_operational = " << (renderer.testZoom(1.0f) ? "true" : "false") << std::endl;
    std::cout << "v14_panning_bounds_respected = " << (renderer.testPanning(100, 100) ? "true" : "false") << std::endl;
    std::cout << "v32_keyboard_shortcuts_trigger = " << (renderer.testInput("shortcuts") ? "true" : "false") << std::endl;
    std::cout << "v33_shortcuts_responsive_high_load = " << (renderer.testInput("shortcuts_load") ? "true" : "false") << std::endl;
    std::cout << "v34_mouse_wheel_zoom_sensitivity = " << (renderer.testZoom(0.5f) ? "true" : "false") << std::endl;
    std::cout << "v35_double_click_fullscreen_idempotent = " << (renderer.testInput("double_click") ? "true" : "false") << std::endl;
    std::cout << "v36_multimonitor_placement_preserved = " << (renderer.testLayout("multimonitor") ? "true" : "false") << std::endl;
    std::cout << "v37_resize_recalculates_viewport = " << (renderer.testLayout("resize") ? "true" : "false") << std::endl;
    std::cout << "v38_minimized_restores_state = " << (renderer.testLayout("minimize") ? "true" : "false") << std::endl;
    std::cout << "v39_dark_mode_theme_consistent = " << (renderer.testLayout("dark_mode") ? "true" : "false") << std::endl;
    std::cout << "v40_theme_switch_no_restart = " << (renderer.testLayout("theme_switch") ? "true" : "false") << std::endl;
    std::cout << "v41_localization_missing_keys_safe = " << (renderer.testLocalization("missing") ? "true" : "false") << std::endl;
    std::cout << "v42_utf8_filenames_load_correctly = " << (manager.loadImage("utf8_测试.png") == io::ImageManager::LoadResult::Success ? "true" : "false") << std::endl;
    std::cout << "v43_unicode_dir_traversal_ok = " << (manager.testDirectoryOps("unicode_traversal") ? "true" : "false") << std::endl;
    std::cout << "v47_concurrent_image_load_threadsafe = " << (manager.testConcurrency("threadsafe_load") ? "true" : "false") << std::endl;
    std::cout << "v48_async_render_queue_ordering = " << (renderer.testRenderQueue("ordering") ? "true" : "false") << std::endl;
    std::cout << "v49_render_cancellation_interrupts = " << (renderer.testRenderQueue("cancel") ? "true" : "false") << std::endl;
    std::cout << "v50_startup_time_below_threshold = " << (manager.testLifecycle("startup_time") ? "true" : "false") << std::endl;
    std::cout << "v51_shutdown_releases_threads = " << (manager.testLifecycle("shutdown_threads") ? "true" : "false") << std::endl;
    std::cout << "v56_slideshow_advances_interval = " << (manager.testLifecycle("slideshow_advance") ? "true" : "false") << std::endl;
    std::cout << "v57_slideshow_pause_persists = " << (manager.testLifecycle("slideshow_pause") ? "true" : "false") << std::endl;
    std::cout << "v58_slideshow_missing_next_safe = " << (manager.testLifecycle("slideshow_missing") ? "true" : "false") << std::endl;
    std::cout << "v59_random_selection_avoids_repeat = " << (manager.testLifecycle("slideshow_random") ? "true" : "false") << std::endl;
    std::cout << "v60_bookmark_persistence_restart = " << (manager.testLifecycle("bookmarks") ? "true" : "false") << std::endl;
    std::cout << "v61_session_restoration_reloads = " << (manager.testLifecycle("session_restore") ? "true" : "false") << std::endl;
    std::cout << "v78_frame_timing_remains_stable = " << (renderer.testPerformance("timing") ? "true" : "false") << std::endl;
    std::cout << "v79_fps_counter_reports_accurate = " << (renderer.testPerformance("fps") ? "true" : "false") << std::endl;
    std::cout << "v95_deterministic_across_linux = " << (renderer.testPerformance("deterministic") ? "true" : "false") << std::endl;
    std::cout << "v96_windows_path_separator_handling = " << (manager.testDirectoryOps("win_paths") ? "true" : "false") << std::endl;
    std::cout << "v97_macos_retina_scaling_sharp = " << (renderer.testLayout("retina") ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("image_env.facts");
    verify_view_batch(facts);
    return 0;
}
