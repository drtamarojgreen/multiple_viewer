#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <filesystem>
#include <fstream>
#include "processor_logic.h"

void testSplitBlocks() {
    std::cout << "Running testSplitBlocks...\n";
    std::string text = "Block 1\n\nBlock 2\n\nBlock 3";
    auto blocks = tp::splitBlocks(text);
    assert(blocks.size() == 3);
    assert(blocks[0] == "Block 1");
    assert(blocks[1] == "Block 2");
    assert(blocks[2] == "Block 3");
    std::cout << "testSplitBlocks passed.\n";
}

void testSplitLines() {
    std::cout << "Running testSplitLines...\n";
    std::string block = "Line 1\nLine 2\r\nLine 3";
    auto lines = tp::splitLines(block);
    assert(lines.size() == 3);
    assert(lines[0] == "Line 1");
    assert(lines[1] == "Line 2");
    assert(lines[2] == "Line 3");
    std::cout << "testSplitLines passed.\n";
}

void testProcessTopics() {
    std::cout << "Running testProcessTopics...\n";
    std::filesystem::path testIn = "test_source";
    std::filesystem::path testOut = "test_target";
    std::filesystem::create_directories(testIn);
    
    std::ofstream ofs(testIn / "test.txt");
    ofs << "Topic1\nSubtopic1.\nThis is a paragraph with multiple dots. It should be classified as such.\n\nTopic2\nSubtopic2.";
    ofs.close();

    int result = tp::processTopics(testIn, testOut);
    assert(result == 0);
    assert(std::filesystem::exists(testOut / "output.csv"));
    assert(std::filesystem::exists(testOut / "onelines.txt"));
    assert(std::filesystem::exists(testOut / "paragraphs.txt"));

    // Cleanup
    std::filesystem::remove_all(testIn);
    std::filesystem::remove_all(testOut);
    std::cout << "testProcessTopics passed.\n";
}

int main() {
    testSplitBlocks();
    testSplitLines();
    testProcessTopics();
    std::cout << "All processor tests passed!\n";
    return 0;
}
