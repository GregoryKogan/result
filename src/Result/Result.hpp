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

  auto emplace_value(const T &value) noexcept -> void;
  auto emplace_error(const E &error) noexcept -> void;

public:
  [[nodiscard]] static auto Ok(const T &value) noexcept -> Result<T, E>;
  [[nodiscard]] static auto Ok(T &&value) noexcept -> Result<T, E>;
  [[nodiscard]] static auto Ok() noexcept -> Result<std::monostate, E>;

  [[nodiscard]] static auto Err(const E &error) noexcept -> Result<T, E>;
  [[nodiscard]] static auto Err(E &&error) noexcept -> Result<T, E>;

  [[nodiscard]] auto is_ok() const noexcept -> bool;
  [[nodiscard]] auto is_err() const noexcept -> bool;

  [[nodiscard]] auto unwrap() const -> T;
  [[nodiscard]] auto unwrap_err() const -> E;
};

/// @brief Sets the Result content to T(value) with index based std::variant::get.
/// @details This is necessary in case of T and E being the same type.
/// @param value The value to be set.
/// @return void
template <typename T, typename E> inline auto Result<T, E>::emplace_value(const T &value) noexcept -> void {
  content_.template emplace<ContentType::Value>(value);
}

/// @brief Analogous to `emplace_value` but for error type.
/// @param error The error to be set.
/// @return void
template <typename T, typename E> inline auto Result<T, E>::emplace_error(const E &error) noexcept -> void {
  content_.template emplace<ContentType::Error>(error);
}

/// @brief Creates a successful Result object containing a value.
/// @param value
/// @return Result<T, E>
template <typename T, typename E> inline auto Result<T, E>::Ok(const T &value) noexcept -> Result<T, E> {
  Result<T, E> result;
  result.emplace_value(value);
  return result;
}

/// @brief Analogous to `Ok(const T &value)` but for rvalue reference.
/// @param value
/// @return Result<T, E>
template <typename T, typename E> inline auto Result<T, E>::Ok(T &&value) noexcept -> Result<T, E> {
  Result<T, E> result;
  result.emplace_value(std::move(value));
  return result;
}

/// @brief Creates a successful Result object containing an empty value.
/// @return Result<std::monostate, E>
template <typename T, typename E> inline auto Result<T, E>::Ok() noexcept -> Result<std::monostate, E> {
  Result<std::monostate, E> result;
  result.emplace_value(std::monostate{});
  return result;
}

/// @brief Creates an unsuccessful Result object containing an error.
/// @param error
/// @note Although it's possible to create a Result with an empty value, it's impossible to create it with an empty
/// error. This is enforced by static_assert.
/// @return Result<T, E>
template <typename T, typename E> inline auto Result<T, E>::Err(const E &error) noexcept -> Result<T, E> {
  Result<T, E> result;
  result.emplace_error(error);
  return result;
}

/// @brief Analogous to `Err(const E &error)` but for rvalue reference.
/// @param error
/// @return Result<T, E>
template <typename T, typename E> inline auto Result<T, E>::Err(E &&error) noexcept -> Result<T, E> {
  Result<T, E> result;
  result.emplace_error(std::move(error));
  return result;
}

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