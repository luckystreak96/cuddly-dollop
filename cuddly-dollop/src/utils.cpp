#include "utils.h"

#include <sstream>
#include <string>

#ifndef _WIN32
#include <dirent.h>
#else
#include <Windows.h>
#endif

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> Utils::GetAllFiles(std::string directory, std::string extension)
{
	std::vector<std::string> vs;
#ifdef _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	hFind = FindFirstFile((directory + "/*." + extension).c_str(), &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			std::string filename = FindFileData.cFileName;
			vs.push_back(filename);
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
#else
	DIR* d;
	struct dirent *dir;
	d = opendir(directory.c_str());
	if(d)
	{
		while((dir = readdir(d)) != NULL)
		{
			std::string temp(dir->d_name);
			if(temp.find(extension) != std::string::npos)
			//if(temp != "." && temp != "..")
				vs.push_back(temp);
		}
		closedir(d);
	}
#endif
	return vs;
}

std::vector<std::string> Utils::Split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

std::string Utils::ReadFile(std::string filename, bool includeNewLine)
{
    std::string whole_text;//stores the whole file text
    std::string line;//one line of text
    std::ifstream myfile(filename);//reads the file


    if (myfile.is_open())
    {
        while ( std::getline (myfile,line) )
        {
			if(includeNewLine)
				whole_text += line + "\n";
			else
				whole_text += line;
        }
        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file" << filename.c_str();
        exit(1);
    }

    return whole_text;
}

std::string Utils::RemoveWhiteSpace(std::string& str)
{
	std::string result;
	for (char c : str)
	{
		if (c != ' ' && c != '\n')
			result.push_back(c);
	}
	return result;
}

std::vector<uint32_t> Utils::ConvertUTF8(std::string text)
{
	std::vector<uint32_t> result;
	for (unsigned int i = 0; i < text.size(); i++)
	{
		uint32_t let = (uint8_t)text[i];
		uint32_t let2 = 0;
		uint32_t let3 = 0;
		uint32_t let4 = 0;
		if (let >> 6 == 0b11)
		{
			// 2 chars
			if (text.size() <= i + 1)
				continue;
			let2 = (uint8_t)text[++i];
		}
		if (let >> 5 == 0b111)
		{
			// 3 chars
			if (text.size() <= i + 1)
				continue;
			let3 = (uint8_t)text[++i];
		}
		if (let >> 3 == 0b11110)
		{
			// 4 chars
			if (text.size() <= i + 1)
				continue;
			let4 = (uint8_t)text[++i];
		}
		uint32_t letter = let4 << 24 | let3 << 16 | let2 << 8 | let;
		result.push_back(letter);
	}

	return result;
}

void Utils::Pause()
{
    std::string wait;
    std::getline(std::cin, wait);
}

BaseType Utils::interpret_type(std::string val) {

    // Check if the value is a float
	std::istringstream iss(val);
    float f;
    iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    bool is_number = iss.eof() && !iss.fail();

	// ints are technically floats as well, so we need to see if the value could be a simple int
	if(is_number)
	{
		int a;
		std::stringstream stream(val);
		if (!(stream >>a) || val.find('.') != std::string::npos ){
			//it wasn't an integer
			//so it's truly a float
			return BT_float;
		}
		// it wasn't a float, so it's an int
		return BT_int;
	}

	// not int or float so it's a string
    return BT_string;
}
