#include "parseArgs.h"

args parseArgs(int argc, char* argv[]){

	bool flag{ false };
	args res;

	// Loop through all parameters passed
	for (int i = 1; i < argc; i++) {
		// Check if argument is a flag
		if (argv[i][0] != '-'){
			res.parameters.push_back(argv[i]);
			continue;
		}
		for (int j = 0; argv[i][j] != '\0'; ++j) {
			char ch = argv[i][j];

			if (ch == '-')
				flag = true;

			if (flag) {
				// Map 'a' -> bit 0, 'b' -> bit 1, ..., 'z' -> bit 25
				if ('a' <= ch && ch <= 'z') {
					res.flags.set(ch - 'a');
				}
				// Map 'A' -> bit 26, 'B' -> bit 27, ..., 'Z' -> bit 51
				else if ('A' <= ch && ch <= 'Z') {
					res.flags.set(ch - 'A' + 26);
				}
				// Map '0' -> bit 52, '1' -> bit 53, ..., '9' -> bit 61
				else if ('0' <= ch && ch <= '9') {
					res.flags.set(ch - '0' + 52);
				}
				// Map special characters
				else if (ch == '#') {
					res.flags.set(62);
				}
				else if (ch == '!') {
					res.flags.set(63);
				}
			}
		}
		flag = false;
	}
	return res;
}
