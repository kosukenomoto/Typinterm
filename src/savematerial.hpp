#include <string>

class SaveMaterial {
 public:
  explicit SaveMaterial(std::string& material);
  int save() const;

 private:
  std::string material_;
  std::string filename_;
};
