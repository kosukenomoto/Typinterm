#pragma once
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Filereader {
 public:
  explicit Filereader(std::string filepath) : filepath_{filepath} {};
  std::string read_file_skip_blanks() {
    std::ifstream ifs(filepath_);
    if (!ifs) throw std::runtime_error("open failed: " + filepath_);
    std::ostringstream out;
    std::string line;
    bool first = true;

    while (std::getline(ifs, line)) {
      // if (is_blank_line(line)) continue;
      if (!first) out << '\n';
      first = false;
      out << line;
    }
    return out.str();
  }

 private:
  std::string filepath_;
  bool is_blank_line(const std::string& line) {
    return std::all_of(line.begin(), line.end(),
                       [](unsigned char ch) { return std::isspace(ch); });
  }
};
