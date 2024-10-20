#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include "vendor/base64.h"
#include <vector>


std::string base64EncodeFile(fs::path p) {
    std::ifstream file(p, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << "\n";
        return "ERROR";
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);

    if (!file.read(buffer.data(), size)) {
        std::cerr << "Failed to read the file." << "\n";
        return "ERROR";
    }

    return to_base64(buffer.data(), buffer.size());
}