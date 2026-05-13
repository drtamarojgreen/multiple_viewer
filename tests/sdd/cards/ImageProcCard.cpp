#include <iostream>
#include <map>
#include "render/image_renderer.h"
#include "io/image_manager.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

void verify_proc_batch(const std::map<std::string, std::string>& facts) {
    render::ImageRenderer renderer;
    io::ImageManager manager;

    std::cout << "v15_rotation_metadata_preserved = " << (manager.applyTransform("rotate") ? "true" : "false") << std::endl;
    std::cout << "v16_horizontal_flip_non_destructive = " << (manager.applyTransform("flip_h") ? "true" : "false") << std::endl;
    std::cout << "v17_vertical_flip_consistent = " << (manager.applyTransform("flip_v") ? "true" : "false") << std::endl;
    std::cout << "v18_grayscale_conversion_deterministic = " << (renderer.applyFilter("grayscale") ? "true" : "false") << std::endl;
    std::cout << "v19_brightness_adjustment_clamps = " << (renderer.applyFilter("brightness") ? "true" : "false") << std::endl;
    std::cout << "v20_contrast_adjustment_no_overflow = " << (renderer.applyFilter("contrast") ? "true" : "false") << std::endl;
    std::cout << "v21_histogram_monochrome_accurate = " << (renderer.testAnalytics("hist_mono") ? "true" : "false") << std::endl;
    std::cout << "v22_histogram_rgb_accurate = " << (renderer.testAnalytics("hist_rgb") ? "true" : "false") << std::endl;
    std::cout << "v28_exif_metadata_missing_fields = " << (manager.testMetadata("exif_missing") ? "true" : "false") << std::endl;
    std::cout << "v29_exif_orientation_autocorrect = " << (manager.testMetadata("exif_auto") ? "true" : "false") << std::endl;
    std::cout << "v68_logging_records_failures = " << (manager.testLogging("error") ? "true" : "false") << std::endl;
    std::cout << "v69_debug_logging_disabled_prod = " << (manager.testLogging("prod") ? "true" : "false") << std::endl;
    std::cout << "v70_log_rotation_operational = " << (manager.testLogging("rotation") ? "true" : "false") << std::endl;
    std::cout << "v74_renderer_fallback_operational = " << (renderer.testGPU("fallback") ? "true" : "false") << std::endl;
    std::cout << "v75_opengl_vulkan_init_errors = " << (renderer.initialize() ? "true" : "false") << std::endl;
    std::cout << "v76_software_rendering_equivalent = " << (renderer.testLayout("software") ? "true" : "false") << std::endl;
    std::cout << "v77_gpu_texture_allocation_failure = " << (renderer.testGPU("texture_fail") ? "true" : "false") << std::endl;
    std::cout << "v80_memory_consumption_stable = " << (manager.testResources("memory_usage") ? "true" : "false") << std::endl;
    std::cout << "v81_survives_24h_slideshow = " << (manager.testResources("stress_24h") ? "true" : "false") << std::endl;
    std::cout << "v82_stress_test_10k_images = " << (manager.testResources("stress_10k") ? "true" : "false") << std::endl;
    std::cout << "v83_race_conditions_close_open = " << (manager.testConcurrency("race_check") ? "true" : "false") << std::endl;
    std::cout << "v84_thread_pool_exhaustion = " << (manager.testConcurrency("exhaustion") ? "true" : "false") << std::endl;
    std::cout << "v85_invalid_shader_programs_fail = " << (renderer.testGPU("bad_shader") ? "true" : "false") << std::endl;
    std::cout << "v94_resource_quotas_enforced = " << (manager.testResources("quota") ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("image_env.facts");
    verify_proc_batch(facts);
    return 0;
}
