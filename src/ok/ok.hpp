#pragma once

#include <variant>

namespace res {

template <typename T, typename E> class result;
template <typename E> class result<void, E>;

template <typename T = std::monostate> class ok {
  T value_;

public:
  ok() = default;
  explicit ok(const T &value) : value_(value) {}

  template <typename E> operator result<T, E>() const;    // NOLINT(google-explicit-constructor)
  template <typename E> operator result<void, E>() const; // NOLINT(google-explicit-constructor)
};

template <typename T> template <typename E> inline ok<T>::operator result<T, E>() const {
  return result<T, E>(value_, true);
}

template <typename T> template <typename E> inline ok<T>::operator result<void, E>() const { return result<void, E>(); }

} // namespace res