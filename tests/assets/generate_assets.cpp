#include <iostream>
#include <fstream>
#include <vector>

void create_test_image(const std::string& path, int w, int h, unsigned char gray = 128) {
    std::ofstream f(path, std::ios::binary);
    f << "SIM_IMG\n" << w << " " << h << "\n";
    std::vector<unsigned char> data(w * h * 3, gray); // RGB
    f.write(reinterpret_cast<char*>(data.data()), data.size());
}

int main() {
    create_test_image("tests/assets/images/valid_100x50.sim", 100, 50, 200);
    create_test_image("tests/assets/images/valid_square.sim", 100, 100, 100);
    create_test_image("tests/assets/images/valid_alpha.sim", 10, 10, 50);
    std::ofstream f("tests/assets/images/corrupted.sim");
    f << "SIM_IMG\nBAD DATA";
    return 0;
}
