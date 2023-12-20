#include <stdexcept>
#include <variant>

namespace res {

template <typename T, typename E> class Result {
public:
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

private:
  std::variant<T, E> content;

  explicit Result(T value) : content(value) {}
  explicit Result(E error) : content(error) {}

  // Make the global structs friends of this class so they can access the private constructor
  template <typename T1> friend struct Ok;
  template <typename E1> friend struct Err;
};

template <typename T> struct Ok {
  T value;
  explicit Ok(T value) : value(std::move(value)) {}
  template <typename E> operator Result<T, E>() const { // NOLINT (google-explicit-constructor)
    return Result<T, E>(value);
  }
};

template <typename E> struct Err {
  E error;
  explicit Err(E error) : error(std::move(error)) {}
  template <typename T> operator Result<T, E>() const { // NOLINT (google-explicit-constructor)
    return Result<T, E>(error);
  }
};

} // namespace res