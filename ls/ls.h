#pragma once

#include <iostream>
#include <filesystem>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <format>
#include <deque>
#include <unordered_map>

#include "parseArgs.h"


namespace f = std::filesystem;

enum fileType {
    Executable,
    Directory,
    Regular
};

struct fileData {
    uintmax_t size;                 // File size in bytes
    std::time_t* lastWrite;         // Last written time
    std::string name;               // Filename
    fileType fileType;              // Type of the file
};

std::unordered_map<std::string, fileType> fileTypeMap{
    {"exe", Executable},
    {"bat", Executable},
    {"cmd", Executable}
};

// Return a string with the formatted time as DD/MM/YYYY HOUR:MINUTE
static std::string format_time(std::time_t time);


// Takes a string, a character and width as input, outputs a string
// justified left with the specified width filled with specified char
static std::string ljust(std::string str, char pad, int totalWidth);


// Convert bytes to human readable size
static std::string humanSize(uintmax_t bytes);