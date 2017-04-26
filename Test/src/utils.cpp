#include "utils.h"

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
        std::cout << "Unable to open file";
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

void Utils::Pause()
{
    std::string wait;
    std::getline(std::cin, wait);
}
