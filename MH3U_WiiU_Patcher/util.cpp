#include "util.h"
#include <fstream>
#include <iostream>


bool existsAndIsDir(const fs::path path) {
	try {
		if (fs::exists(path)) {
			if (fs::is_directory(path)) {
				return true;
			}
			std::cerr << path << " is not a directory.\n";
			return false;
		}
	}
	catch (const fs::filesystem_error &fe) {
		cerr << fe.what() << endl;
		return false;
	}
	cout << path << " does not exist.\n";
	return false;
}

bool existsAndIsFile(const fs::path path) {
	try {
		if (fs::exists(path)) {
			if (fs::is_regular_file(path)) {
				return true;
			}
			std::cerr << path << " is not a file.\n";
			return false;
		}
	}
	catch (const fs::filesystem_error &fe) {
		cerr << fe.what() << endl;
		return false;
	}
	cout << path << " does not exist.\n";
	return false;
}

string slurp(const string& path) {
	ostringstream buf;
	ifstream input(path.c_str());
	buf << input.rdbuf();
	return buf.str();
}

unsigned int hexCharToInt(const char c) {
	return c > '9' ? (c | 32) - 'a' + 10 : c - '0';
}

void decodeHexStr(const char* in, size_t len, char* out) {
	unsigned int i, t, high, low;

	for (t = 0, i = 0; i < (len*2); i += 2, ++t) {
		high = hexCharToInt(in[i]);
		low = hexCharToInt(in[i + 1]);
		out[t] = (high << 4) | low;
	}
}

