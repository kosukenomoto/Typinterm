#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "llm_infobody.hpp"
class ExerciseMaterial {
 public:
  explicit ExerciseMaterial(struct Llm_infobody& llmin);
  std::optional<std::string> material() const;

 private:
  std::string editedMaterial(const std::string& src,
                             std::size_t witdh = 80) const;
  std::string& url_;
  nlohmann::json body_;
};
