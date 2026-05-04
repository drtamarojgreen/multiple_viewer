#include <iostream>

class SorrelTest_ArgumentEcho
{
public:
    static int run(int argc, char** argv)
    {
        if (argc > 1)
        {
            std::cout << argv[1] << std::endl;
        }
        return 0;
    }
};

int main(int argc, char** argv)
{
    return SorrelTest_ArgumentEcho::run(argc, argv);
}