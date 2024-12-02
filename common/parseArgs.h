#pragma once

#include <vector>
#include <string>
#include <bitset>

struct args {
	std::vector<std::string>	parameters;		// A collection of all parameters (Non flag arguments)
	std::bitset<64>				flags;			// All english characters and numbers can be passed as flags
};

// Returns a struct containing collections of all parameters passed and flags passed as a bitset.
// Bitset format (characters: bits): a-z: 0-25; A-Z: 26-51; 0-9: 52-61; # and !: bits 62 and 63
// Ignores non valid characters
args parseArgs(int argc, char* argv[]);