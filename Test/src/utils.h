#ifndef UTILS_H__
#define UTILS_H__

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

class Utils
{
    public:
        static std::string ReadFile(std::string filename, bool includeNewLine = true);
		static std::string RemoveWhiteSpace(std::string& str);
        static void Pause();
};

#endif
