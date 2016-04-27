#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "helpers.hpp"

const char SAFE_CHARS[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\\'()*+,-./:;<=>?@[\\]^_`{|}~ ";

std::string bindump(const char* data, const size_t& length, const bool& spacing)
{
    std::stringstream ss;

    for (size_t i = 0; i < length; i++) {
        uint8_t byte = static_cast<uint8_t>(data[i]);
        if (i % BINDUMP_LINELEN == 0 and i != 0) {
            ss << "\n";
        }
        ss << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)byte;
        if (spacing) {
            ss << " ";
        }
    }
    return ss.str();
}

std::string bindump(const std::vector<char>& data, const bool& spacing)
{
    return bindump(data.data(), data.size(), spacing);
}

std::string escape(std::string str)
{
    std::stringstream ss;
    ss << std::hex;
    for (size_t i = 0; i < str.size(); i++) {
        char c = str.at(i);
        bool safe = false;
        for (size_t si = 0; si < sizeof(SAFE_CHARS); si++) {
            if (c == SAFE_CHARS[si])
                safe = true;
        }
        if (safe) {
            ss << c;
        } else {
            ss << std::setfill('0') << std::setw(2);
            ss << "\\x" << (unsigned int)(unsigned char)c;
        }
    }
    return ss.str();
}

std::string indent(std::string str, int level)
{
    size_t found = 0;
    std::string indent_spaces = std::string(level, ' ');
    str.insert(0, indent_spaces);
    while (true) {
        found = str.find('\n', found+1);
        if (found == std::string::npos or found == str.size()-1) {
            break;
        }
        str.insert(found+1, indent_spaces);
    }
    return str;
}

std::string format_data(std::vector<char> data)
{
    if (data.size() > BINDUMP_LINELEN) {
        return "  data:\n" + indent(bindump(data), 4);
    } else {
        return "  data: " + bindump(data);
    }
}
