#include "model/quanta_glia.h"
#include <iostream>
#include <cassert>

using namespace model;

void testQuantaGliaManagement() {
    auto& manager = QuantaGlia::getInstance();
    manager.clear();

    // Create repositories
    auto repo1 = manager.createRepository("repo1");
    auto repo2 = manager.createRepository("repo2");

    assert(repo1 != nullptr);
    assert(repo2 != nullptr);
    assert(repo1 != repo2);

    // Retrieve repositories
    assert(manager.getRepository("repo1") == repo1);
    assert(manager.getRepository("repo2") == repo2);
    assert(manager.getRepository("non_existent") == nullptr);

    // Check existence
    assert(manager.hasRepository("repo1") == true);
    assert(manager.hasRepository("repo2") == true);
    assert(manager.hasRepository("repo3") == false);

    // List IDs
    auto ids = manager.getAllRepositoryIds();
    assert(ids.size() == 2);
    assert((ids[0] == "repo1" && ids[1] == "repo2") || (ids[0] == "repo2" && ids[1] == "repo1"));

    // Remove repository
    manager.removeRepository("repo1");
    assert(manager.hasRepository("repo1") == false);
    assert(manager.getAllRepositoryIds().size() == 1);

    std::cout << "[PASS] QuantaGlia Unit Tests\n";
}

#ifdef QUANTA_GLIA_STANDALONE
int main() {
    testQuantaGliaManagement();
    return 0;
}
#endif
