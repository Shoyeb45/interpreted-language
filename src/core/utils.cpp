#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>


std::vector<std::string> read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);       
    }

    std::vector<std::string> file_contents;

    std::string line;
    while (std::getline(file, line)) {
        file_contents.push_back(line);
    }
    return file_contents;
}

// Define standard whitespace characters
const std::string WHITESPACE = " \n\r\t\f\\v";

std::string ltrim(const std::string& s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s) {
    return rtrim(ltrim(s));
}
