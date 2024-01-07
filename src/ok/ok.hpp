#pragma once

#include <variant>

namespace res {

template <typename T, typename E> class result;
template <typename E> class result<void, E>;

/// @brief Ok object represents a successful outcome and can be implicitly converted to a result.
/// @details Ok object holds a value of type T. Empty Ok object can be created if T is std::monostate.
/// @tparam T Type of the value. Defaults to std::monostate.
template <typename T = std::monostate> class ok {
  T value_;

public:
  ok() = default;
  explicit ok(T value) : value_(std::move(value)) {}

  template <typename E> operator result<T, E>() const;    // NOLINT(google-explicit-constructor)
  template <typename E> operator result<void, E>() const; // NOLINT(google-explicit-constructor)
};

template <typename T> template <typename E> inline ok<T>::operator result<T, E>() const {
  return result<T, E>(result<T, E>::Successful::SUCCESSFUL, value_);
}

template <typename T> template <typename E> inline ok<T>::operator result<void, E>() const {
  return result<void, E>(result<void, E>::Successful::SUCCESSFUL);
}

} // namespace res