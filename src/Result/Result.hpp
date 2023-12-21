#include <stdexcept>
#include <variant>

namespace res {

template <typename T, typename E> class Result {
  static_assert(!std::is_same_v<E, std::monostate>, "E must not be std::monostate");

  enum ContentType { Value, Error };
  std::variant<T, E> content_;

  auto emplace_value(const T &value) -> void { content_.template emplace<ContentType::Value>(value); }
  auto emplace_error(const E &error) -> void { content_.template emplace<ContentType::Error>(error); }

public:
  [[nodiscard]] static auto Ok(const T &value) -> Result<T, E> {
    Result<T, E> result;
    result.emplace_value(value);
    return result;
  }
  [[nodiscard]] static auto Ok(T &&value) -> Result<T, E> {
    Result<T, E> result;
    result.emplace_value(std::move(value));
    return result;
  }
  [[nodiscard]] static auto Ok() -> Result<std::monostate, E> {
    Result<std::monostate, E> result;
    result.emplace_value(std::monostate{});
    return result;
  }

  [[nodiscard]] static auto Err(const E &error) -> Result<T, E> {
    Result<T, E> result;
    result.emplace_error(error);
    return result;
  }
  [[nodiscard]] static auto Err(E &&error) -> Result<T, E> {
    Result<T, E> result;
    result.emplace_error(std::move(error));
    return result;
  }

  [[nodiscard]] auto is_ok() const -> bool { return content_.index() == ContentType::Value; }
  [[nodiscard]] auto is_err() const -> bool { return content_.index() == ContentType::Error; }

  [[nodiscard]] auto unwrap() const -> T {
    if (is_err()) { throw std::runtime_error("Called unwrap on an Err value"); }
    if constexpr (std::is_same_v<T, std::monostate>) { throw std::runtime_error("Called unwrap on an empty value"); }
    return std::get<ContentType::Value>(content_);
  }

  [[nodiscard]] auto unwrap_err() const -> E {
    if (is_ok()) { throw std::runtime_error("Called unwrap_err on an Ok value"); }
    return std::get<ContentType::Error>(content_);
  }
};

} // namespace res