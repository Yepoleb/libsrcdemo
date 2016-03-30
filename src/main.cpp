#include <iostream>
#include <fstream>
#include <stdint.h>

bool read_file(const char* filename, uint8_t*& buffer, size_t& size)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (not file.is_open()) {
        std::cerr << "read_file: File not open!" << std::endl;
        return false;
    }
    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new uint8_t[size];
    if (!file.read(reinterpret_cast<char*>(buffer), size)) {
        std::cerr << "read_file: Could not read buffer!" << std::endl;
        file.close();
        return false;
    } else {
        file.close();
        return true;
    }
}

int main(int argc, char **argv)
{
    const char* filename = "/home/gabriel/Spiele/Steam/Team Fortress 2/tf/crab.dem";
    uint8_t* buffer;
    size_t buffer_size;
    if (not read_file(filename, buffer, buffer_size)) {
        std::cerr << "Failed to read demo file!" << std::endl;
        return 1;
    }
    std::cerr << "Read " << buffer_size << " bytes" << std::endl;

    //_currentFile = new DemoFile(f);

//    foreach (var msg in _currentFile.Messages)
//    {
//        messageList.Items.Add(new DemoMessageItem(msg));
//    }

    delete buffer;
    return 0;
}
