/// @file
/// @brief result-cpp is a C++ library that provides a Result<T, E> type, which can be used to return and propagate
/// errors. It's inspired by Rust's std::Result type.
///
/// This file contains the declaration of the Result class template.
/// @author GregoryKogan
/// @version 0.0.1
/// @date 2023, December 21
/// @par License
/// This software is released under the GNU GENERAL PUBLIC LICENSE Version 3.
/// @par Contact
/// My github page: https://github.com/GregoryKogan

#include <stdexcept>
#include <variant>

namespace res {

/// @brief `Result` is a type that represents either success or failure.
///
/// Result<T, E> is the type used for returning and propagating errors. It holds std::variant<T, E>,
/// where T is the type of a successful value and E is the type of an error.
/// @tparam T
/// @tparam E
template <typename T, typename E> class Result {
  static_assert(!std::is_same_v<E, std::monostate>, "E must not be std::monostate");

  enum ContentType { Value, Error };
  std::variant<T, E> content_;

  template <typename T1> friend struct Ok;
  template <typename E1> friend struct Err;

  auto set_value(const T &value) -> void { content_.template emplace<ContentType::Value>(value); }
  auto set_error(const E &error) -> void { content_.template emplace<ContentType::Error>(error); }

public:
  [[nodiscard]] auto is_ok() const noexcept -> bool;
  [[nodiscard]] auto is_err() const noexcept -> bool;

  [[nodiscard]] auto unwrap() const -> T;
  [[nodiscard]] auto unwrap_err() const -> E;

  /// @brief `true` if the Result is successful, `false` otherwise.
  constexpr explicit operator bool() const noexcept { return is_ok(); }

  /// @name Accesses the value of a successful Result.
  /// @return Pointer or reference to the value.
  ///@{

  /// @brief Returns a pointer to the value if the Result is successful, nullptr otherwise.
  constexpr auto operator->() const -> const T * { return &std::get<ContentType::Value>(content_); }
  /// @brief Returns a pointer to the value if the Result is successful, nullptr otherwise.
  constexpr auto operator->() -> T * { return &std::get<ContentType::Value>(content_); }
  /// @brief Returns a reference to the value if the Result is successful, throws otherwise.
  constexpr auto operator*() const & -> const T & { return std::get<ContentType::Value>(content_); }
  /// @brief Returns a reference to the value if the Result is successful, throws otherwise.
  constexpr auto operator*() & -> T & { return std::get<ContentType::Value>(content_); }
  /// @brief Returns a reference to the value if the Result is successful, throws otherwise.
  constexpr auto operator*() const && -> const T && { return std::get<ContentType::Value>(content_); }
  /// @brief Returns a reference to the value if the Result is successful, throws otherwise.
  constexpr auto operator*() && -> T && { return std::get<ContentType::Value>(content_); }

  ///@}
};

/// @brief Ok object represents a successful outcome and can be converted to a Result.
/// @details Ok object holds a value of type T. Empty Ok object can be created if T is std::monostate.
/// @tparam T Type of the value. Defaults to std::monostate.
template <typename T = std::monostate> class Ok {
  T value_;

public:
  /// @brief Creates an empty Ok object.
  Ok() : value_() {}
  /// @brief Creates an Ok object with a value.
  /// @param value
  explicit Ok(const T &value) : value_(value) {}
  /// @brief Creates an Ok object with a moved value.
  /// @param value
  explicit Ok(T &&value) : value_(std::move(value)) {}

  /// @brief Implicitly converts Ok<T> object to a Result<T, E>.
  /// @tparam E
  template <typename E> operator Result<T, E>() const { // NOLINT(google-explicit-constructor)
    Result<T, E> result;
    result.set_value(value_);
    return result;
  }
};

/// @brief Err object represents an unsuccessful outcome and can be converted to a Result.
/// @details Err object holds an error of type E. Err object can't be empty, E must not be std::monostate.
/// @tparam E
template <typename E> class Err {
  static_assert(!std::is_same_v<E, std::monostate>, "E must not be std::monostate");

  E error_;

public:
  Err() = delete;
  /// @brief Creates an Err object with an error.
  /// @param error
  explicit Err(const E &error) : error_(error) {}
  /// @brief Creates an Err object with a moved error.
  /// @param error
  explicit Err(E &&error) : error_(std::move(error)) {}

  /// @brief Implicitly converts Err<E> object to a Result<T, E>.
  /// @tparam T
  template <typename T> operator Result<T, E>() const { // NOLINT(google-explicit-constructor)
    Result<T, E> result;
    result.set_error(error_);
    return result;
  }
};

/// @brief Checks if the Result is successful.
/// @return bool True if the Result is successful, false otherwise.
template <typename T, typename E> inline auto Result<T, E>::is_ok() const noexcept -> bool {
  return content_.index() == ContentType::Value;
}

/// @brief Checks if the Result is unsuccessful.
/// @return bool True if the Result is unsuccessful, false otherwise.
template <typename T, typename E> inline auto Result<T, E>::is_err() const noexcept -> bool {
  return content_.index() == ContentType::Error;
}

/// @brief Returns the value of a successful Result.
/// @return T
/// @throw std::runtime_error if the Result is unsuccessful.
/// @throw std::runtime_error if the Result is empty.
template <typename T, typename E> inline auto Result<T, E>::unwrap() const -> T {
  if (is_err()) { throw std::runtime_error("Called unwrap on an Err value"); }
  if constexpr (std::is_same_v<T, std::monostate>) { throw std::runtime_error("Called unwrap on an empty value"); }
  return std::get<ContentType::Value>(content_);
}

/// @brief Returns the error of an unsuccessful Result.
/// @return E
/// @throw std::runtime_error if the Result is successful.
template <typename T, typename E> inline auto Result<T, E>::unwrap_err() const -> E {
  if (is_ok()) { throw std::runtime_error("Called unwrap_err on an Ok value"); }
  return std::get<ContentType::Error>(content_);
}

} // namespace res