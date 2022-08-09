#include <iostream>
#include <fstream>
#include <string>

#include "Graph.hpp"
#include "utils.hpp"

#define FILE_NOT_OPEN 1
#define ARGC_INVALID 2

#define EXPECTED_ARGC(argc) ((argc) >= 3 && (argc) <= 4)

#define OPEN_FILE(fileObj, fileName)        \
    do {                                    \
        fileObj.open(fileName);             \
        if (!fileObj.is_open()) {           \
            std::cerr                       \
                << " error opening file `"  \
                << fileName << "`\n";       \
            return FILE_NOT_OPEN;           \
        }                                   \
    } while(0)

int main(int argc, char const *argv[])
{
    const char *programName = *argv;
    
    if (!EXPECTED_ARGC(argc)) {
        std::cerr << "Uso: " << programName << " INPUT_FILE OUTPUT_FILE\n\n";
        return ARGC_INVALID;
    }

    std::ifstream input_file;
    std::ofstream outpt_file;

    OPEN_FILE(input_file, argv[1]);

    OPEN_FILE(outpt_file, argv[2]);

    Graph g = Graph::readFile(input_file, "joseph");

    long seed = time(NULL) * time(NULL) / rand();
    
    if (argc == 4) {
        seed = std::stol(argv[3]);
    }

    srand(seed);

    std::cout << "\nseed: " << seed << "\n";
    Solution s = g.aco();
    std::cout << "\n\ncusto: " << s.getCost() << "\n";

    return 0;
}
