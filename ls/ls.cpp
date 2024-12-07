#include "ls.h"

// Constants
constexpr int MARGIN = 1;           // Spaces added between fields

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
    bool PAD_NAMES      = true;                             // Whether to output filenames in a grid
    bool SHOW_ALL       = parsedArgs.flags.test(0);         // -a flag
    bool LONG_FORMAT    = parsedArgs.flags.test(11);        // -l flag
    bool HUMAN_READABLE = parsedArgs.flags.test(7);         // -h flag

    if (LONG_FORMAT) 
        PAD_NAMES = false;


    // Add spacing according to terminal width
    int termWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int longestNameLength{ 0 };

    std::vector<fileData> files;

    for (f::directory_entry file : f::directory_iterator(f::current_path())) {
        std::string filepath = file.path().string();
        std::string filename = filepath.substr(filepath.find_last_of("\\") + 1, filepath.length());

        if (filename[0] == '.' && !SHOW_ALL)     // Hidden file, do not show by default
            continue;

        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            f::last_write_time(file.path()) - f::file_time_type::clock::now() + std::chrono::system_clock::now()
        );

        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

        files.push_back(
            fileData(
                file.file_size(),
                &cftime,
                filename
            )
        );

        if (filename.length() > longestNameLength)
            longestNameLength = filename.length();
    }

    // Calculating how many file names we can fit in 1 line
    int columns = termWidth / (longestNameLength + MARGIN);

    for (int fileNum = 0; fileNum < files.size(); fileNum++) {
        if (fileNum % columns == 0 && fileNum != 0 && PAD_NAMES)
            std::cout << "\n";

        // Output filenames with appropriate padding (if any)
        if (PAD_NAMES) {
            std::cout << ljust(files[fileNum].name, ' ', longestNameLength + MARGIN);
        }
        if (LONG_FORMAT) {
            std::string fileSize = HUMAN_READABLE ? humanSize(files[fileNum].size) : std::to_string(files[fileNum].size);
            std::cout << ljust(files[fileNum].name, ' ', longestNameLength + MARGIN) 
                      << format_time(*files[fileNum].lastWrite) << ljust("", ' ', MARGIN)
                      << fileSize << "\n";
        }
    }
}


static std::string format_time(std::time_t time) {
    std::tm timeinfo{};
    // Use localtime_s to safely populate the tm structure
    localtime_s(&timeinfo, &time);

    // Buffer to hold the formatted time string
    char buffer[100];
    if (std::strftime(buffer, sizeof(buffer), "%b %d %Y %H:%M", &timeinfo)) {
        return std::string(buffer);
    }
    else {
        return std::string{ "000" };
    }
}

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

static std::string humanSize(uintmax_t bytes) {
    constexpr long long K = 1024;      // 1 KB = 1024 bytes
    constexpr long long M = K * 1024;  // 1 MB = 1024 KB
    constexpr long long G = M * 1024;  // 1 GB = 1024 MB


    double size_in_bytes = static_cast<double>(bytes);
    std::string unit = "B"; // Default unit is bytes

    // Determine the appropriate unit (KB, MB, GB) based on the size
    if (size_in_bytes >= G) {
        size_in_bytes /= G;  // Convert to gigabytes if size is >= 1 GB
        unit = "G";
    }
    else if (size_in_bytes >= M) {
        size_in_bytes /= M;  // Convert to megabytes if size is >= 1 MB
        unit = "M";
    }
    else if (size_in_bytes >= K) {
        size_in_bytes /= K;  // Convert to kilobytes if size is >= 1 KB
        unit = "K";
    }

    // Format the result to 1 decimal place for readability
    return std::format("{:.1f} {}", size_in_bytes, unit);
}