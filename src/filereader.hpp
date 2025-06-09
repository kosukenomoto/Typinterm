#pragma once
#include <string>

class Filereader {
 public:
  explicit Filereader(std::string filepath);
  std::string read_file_skip_blanks();

 private:
  std::string filepath_;
  bool is_blank_line(const std::string& line);
};
