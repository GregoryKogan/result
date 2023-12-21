#include <stdexcept>
#include <variant>

namespace res {

template <typename T, typename E> class Result {
  std::variant<T, E> content;

  explicit Result(const T &value) : content(value) {}
  explicit Result(const E &value) : content(value) {}

public:
  static auto Ok(const T &value) -> Result { return Result<T, E>(value); }
  static auto Err(const E &value) -> Result { return Result<T, E>(value); }

  [[nodiscard]] auto is_ok() const -> bool { return std::holds_alternative<T>(content); }
  [[nodiscard]] auto is_err() const -> bool { return std::holds_alternative<E>(content); }

  [[nodiscard]] auto unwrap() const -> T {
    if (is_err()) { throw std::runtime_error("Called unwrap on an Err value"); }
    return std::get<T>(content);
  }

  [[nodiscard]] auto unwrap_err() const -> E {
    if (is_ok()) { throw std::runtime_error("Called unwrap_err on an Ok value"); }
    return std::get<E>(content);
  }
};

} // namespace res