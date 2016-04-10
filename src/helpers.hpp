#pragma once
#include <stdint.h>
#include <string>
#include <vector>

const size_t BINDUMP_LINELEN = 16;

std::string bindump(const char* data, const size_t& length, const bool& spacing=true);
std::string bindump(const std::vector<char>& data, const bool& spacing=true);
std::string escape(std::string str);
std::string indent(std::string str, int level);
std::string format_data(std::vector<char> data);
