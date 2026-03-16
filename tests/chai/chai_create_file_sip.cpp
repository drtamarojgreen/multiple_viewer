#include <fstream>
#include <iostream> // For error reporting

class ChaiTest_CreateFile
{
public:
    static int run(int argc, char** argv)
    {
        std::ofstream outfile("chai_created_file.tmp");
        if (outfile.is_open())
        {
            outfile.close();
            // We don't delete it here, as cleanup is a separate question/sip.
            // The fact we could open and close it is the observation.
            return 0; // Success: file was created/opened and closed
        }
        else
        {
            std::cerr << "Error: Could not create/open file." << std::endl;
            return 1; // Failure: file could not be created/opened
        }
    }
};

int main(int argc, char** argv)
{
    return ChaiTest_CreateFile::run(argc, argv);
}
