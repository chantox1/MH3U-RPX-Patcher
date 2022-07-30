#pragma once

#include <filesystem>
using namespace std;
namespace fs = filesystem;

bool existsAndIsDir(const fs::path path);
bool existsAndIsFile(const fs::path path);
string slurp(const string& path);
unsigned int hexCharToInt(const char c);
void decodeHexStr(const char* in, size_t len, char* out);
