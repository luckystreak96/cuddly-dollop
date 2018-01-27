#ifndef UTILS_H__
#define UTILS_H__

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>

class Utils
{
    public:
        static std::string ReadFile(std::string filename, bool includeNewLine = true);
		static std::string RemoveWhiteSpace(std::string& str);
		static std::vector<uint32_t> ConvertUTF8(std::string text);
		static std::vector<std::string> Split(const std::string &s, char delim);
        static void Pause();
};

#endif
