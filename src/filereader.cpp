#include "filereader.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Filereader::Filereader(std::string filepath) : filepath_{filepath} {};
std::string Filereader::read_file_skip_blanks() {
  std::ifstream ifs(filepath_);
  if (!ifs) throw std::runtime_error("open failed: " + filepath_);
  std::ostringstream out;
  std::string line;
  bool first = true;

  while (std::getline(ifs, line)) {
    // TODO To be optional pram
    if (is_blank_line(line)) continue;
    if (!first) out << '\n';
    first = false;
    out << line;
  }
  return out.str();
}

bool Filereader::is_blank_line(const std::string& line) {
  return std::all_of(line.begin(), line.end(),
                     [](unsigned char ch) { return std::isspace(ch); });
}
