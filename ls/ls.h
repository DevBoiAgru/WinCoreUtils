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
    std::wstring name;              // Filename
    fileType fileType;              // Type of the file
};

std::unordered_map<std::wstring, fileType> fileTypeMap{
    {L"exe", Executable},
    {L"bat", Executable},
    {L"cmd", Executable}
};

// Return a string with the formatted time as DD/MM/YYYY HOUR:MINUTE
static std::wstring format_time(std::time_t time);


// Takes a string, a character and width as input, outputs a string
// justified left with the specified width filled with specified char
static std::wstring ljust(std::wstring str, char pad, int totalWidth);


// Convert bytes to human readable size
static std::wstring humanSize(uintmax_t bytes);