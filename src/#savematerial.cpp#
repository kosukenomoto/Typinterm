#include "savematerial.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

SaveMaterial::SaveMaterial(std::string& material) : material_{material} {
  // －－ ② 日時 → 文字列（YYYYMMDD_HHMMSS）－－
  using clock = std::chrono::system_clock;
  auto now = clock::now();
  std::time_t t = clock::to_time_t(now);  // time_t に変換
  std::tm tm = *std::localtime(&t);       // 現地時刻へ（Asia/Tokyo）

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y%m%d_%H%M%S");  // 20250615_135942
  filename_ = oss.str() + ".txt";              // 拡張子を付ける
};

int SaveMaterial::save() const {
  std::ofstream ofs(filename_, std::ios::out | std::ios::binary);
  if (!ofs) {
    std::cerr << "ファイルを開けませんでした\n";
    return 1;
  }
  ofs << material_;
  std::cout << "書き込み完了: " << filename_ << '\n';
  return 0;
}
