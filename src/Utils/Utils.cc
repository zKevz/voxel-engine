#include "Utils.hh"

#include "Logger.hh"

#include <fstream>

std::string Utils::ReadFile(std::string_view path)
{
    std::fstream fs(path);
    if (!fs)
    {
        LogError("Cannot read file {}", path);
        return "";
    }

    std::string string((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    return string;
}
