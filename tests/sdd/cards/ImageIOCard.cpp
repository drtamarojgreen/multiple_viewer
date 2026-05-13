#include <iostream>
#include <map>
#include <filesystem>
#include "io/image_manager.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;
namespace fs = std::filesystem;

void verify_io_batch(const std::map<std::string, std::string>& facts) {
    io::ImageManager manager;
    fs::path asset_root = "tests/assets/images";

    std::cout << "v1_single_source_launch = " << (manager.loadImage(asset_root / "valid.png") == io::ImageManager::LoadResult::Success ? "true" : "false") << std::endl;
    std::cout << "v2_multi_stream_launch = " << (manager.testConcurrency("multi_stream") ? "true" : "false") << std::endl;
    std::cout << "v3_empty_directory_handling = " << (manager.testDirectoryOps("load_empty") ? "true" : "false") << std::endl;
    std::cout << "v4_corrupted_file_rejection = " << (manager.loadImage(asset_root / "corrupted.jpg") == io::ImageManager::LoadResult::CorruptedFile ? "true" : "false") << std::endl;
    std::cout << "v5_unsupported_extension_handling = " << (manager.loadImage(asset_root / "invalid.txt") == io::ImageManager::LoadResult::UnsupportedFormat ? "true" : "false") << std::endl;
    std::cout << "v6_large_image_no_freeze = " << (manager.testResources("large_image") ? "true" : "false") << std::endl;
    std::cout << "v7_rapid_image_switching_leaks = " << (manager.testResources("rapid_switch") ? "true" : "false") << std::endl;
    std::cout << "v8_buffer_disposal_on_close = " << (manager.testResources("buffer_disposal") ? "true" : "false") << std::endl;
    std::cout << "v23_alpha_transparency_render = " << (manager.testMetadata("alpha") ? "true" : "false") << std::endl;
    std::cout << "v24_jpeg_progressive_decode = " << (manager.testMetadata("progressive_jpeg") ? "true" : "false") << std::endl;
    std::cout << "v25_animated_gif_playback = " << (manager.testMetadata("gif_anim") ? "true" : "false") << std::endl;
    std::cout << "v26_gif_pause_resume = " << (manager.testMetadata("gif_control") ? "true" : "false") << std::endl;
    std::cout << "v27_tiff_multipage_nav = " << (manager.testMetadata("tiff_multi") ? "true" : "false") << std::endl;
    std::cout << "v30_drag_drop_multi_load = " << (manager.handleDragDrop({"a.png", "b.png"}) ? "true" : "false") << std::endl;
    std::cout << "v31_drag_drop_reject_non_image = " << (manager.handleDragDrop({"a.txt"}) ? "true" : "false") << std::endl;
    std::cout << "v44_symlink_resolution_safe = " << (manager.testDirectoryOps("symlink") ? "true" : "false") << std::endl;
    std::cout << "v45_invalid_path_traversal_rejected = " << (manager.loadImage("/etc/passwd") == io::ImageManager::LoadResult::SecurityViolation ? "true" : "false") << std::endl;
    std::cout << "v46_cache_cleanup_on_termination = " << (manager.testLifecycle("cache_cleanup") ? "true" : "false") << std::endl;
    std::cout << "v52_file_watcher_detect_new = " << (manager.testDirectoryOps("watch_add") ? "true" : "false") << std::endl;
    std::cout << "v53_file_watcher_handle_deleted = " << (manager.testDirectoryOps("watch_del") ? "true" : "false") << std::endl;
    std::cout << "v54_hot_reload_updates = " << (manager.testDirectoryOps("hot_reload") ? "true" : "false") << std::endl;
    std::cout << "v55_duplicate_entry_prevention = " << (manager.testMetadata("duplicates") ? "true" : "false") << std::endl;
    std::cout << "v71_plugin_loader_reject_malformed = " << (manager.testLifecycle("plugin_load") ? "true" : "false") << std::endl;
    std::cout << "v72_plugin_init_failure_safe = " << (manager.testLifecycle("plugin_init") ? "true" : "false") << std::endl;
    std::cout << "v73_plugin_sandbox_fs_block = " << (manager.testLifecycle("plugin_sandbox") ? "true" : "false") << std::endl;
    std::cout << "v86_screenshot_pixel_identical = " << (manager.testExport("screenshot") ? "true" : "false") << std::endl;
    std::cout << "v87_screenshot_preserve_alpha = " << (manager.testExport("screenshot_alpha") ? "true" : "false") << std::endl;
    std::cout << "v88_clipboard_copy_large_image = " << (manager.testResources("clipboard_copy") ? "true" : "false") << std::endl;
    std::cout << "v89_clipboard_paste_reject_binary = " << (manager.testResources("clipboard_paste") ? "true" : "false") << std::endl;
    std::cout << "v90_autosave_recovery_operational = " << (manager.testLifecycle("autosave") ? "true" : "false") << std::endl;
    std::cout << "v91_sandbox_execution_blocked = " << (manager.testLifecycle("sandbox_exec") ? "true" : "false") << std::endl;
    std::cout << "v92_malformed_header_overflow_prevent = " << (manager.testResources("header_overflow") ? "true" : "false") << std::endl;
    std::cout << "v93_decompression_bomb_detection = " << (manager.testResources("decomp_bomb") ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("image_env.facts");
    verify_io_batch(facts);
    return 0;
}
