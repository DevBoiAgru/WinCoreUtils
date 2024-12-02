#include <iostream>
#include <filesystem>
#include "parseArgs.h"

namespace f = std::filesystem;

int main(int argc, char* argv[]) {
    args res = parseArgs(argc, argv);

    // TODO: Handle flags

    for (f::directory_entry file : f::directory_iterator(f::current_path())) {
        auto filename = file.path().string();

        std::cout << filename.substr(filename.find_last_of("\\"), filename.length()) << "\n";
    }
}