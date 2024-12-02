#include <iostream>
#include <filesystem>
#include <windows.h>

#include "parseArgs.h"

namespace f = std::filesystem;

// Constants
constexpr int MARGIN = 1;           // Spaces added while adding padding between file names


// Takes a string, a character and width as input, outputs a string
// justified left with the specified width filled with specified char
static std::string ljust(std::string str, char pad, int totalWidth) {
    if (str.length() >= totalWidth) {
        return str;
    }
    int padDiff = totalWidth - str.length();
    for (int i = 0; i <= padDiff; i++) {
        str += pad;
    }
    return str;
}

int main(int argc, char* argv[]) {

    args parsedArgs = parseArgs(argc, argv);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Check if console handle is valid
    if (hConsole == INVALID_HANDLE_VALUE) {
        perror("GetStdHandle failed");
        return 1;
    }

    // Get console screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        perror("GetConsoleScreenBufferInfo failed");
        return 1;
    }

    // Check flags passed
    bool PAD_NAMES = true;                       // Whether to output filenames in a grid
    bool SHOW_ALL = parsedArgs.flags.test(0);         // -a flag

    // Add spacing according to terminal width
    int termWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int longestNameLength{ 0 };

    std::vector<std::string> filenames;
    for (f::directory_entry file : f::directory_iterator(f::current_path())) {
        std::string filepath = file.path().string();
        std::string filename = filepath.substr(filepath.find_last_of("\\") + 1, filepath.length());

        if (filename[0] == '.' && !SHOW_ALL)     // Hidden file
            continue;

        filenames.push_back(filename);

        if (filename.length() > longestNameLength)
            longestNameLength = filename.length();
    }

    // Calculating how many file names we can fit in 1 line
    int columns = termWidth / (longestNameLength + MARGIN);

    for (int fileNum = 0; fileNum < filenames.size(); fileNum++) {
        if (fileNum % columns == 0 && fileNum != 0 && PAD_NAMES)
            std::cout << "\n";

        // Output filenames with appropriate padding (if any)
        if (PAD_NAMES) {
            std::cout << ljust(filenames[fileNum], ' ', longestNameLength + MARGIN);
        }
        else {
            std::cout << filenames[fileNum] << "\n";
        }
        
    }
}