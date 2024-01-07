/// result is a C++ library that provides a Result<T, E> type, which can be used to return and propagate errors. It's
/// inspired by Rust's std::Result type.
///
/// author: GregoryKogan
/// My github page: https://github.com/GregoryKogan
/// version: 0.0.0
/// date: 2023, December 21
/// This software is released under the GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef RESULT_LIB
#define RESULT_LIB

#include <stdexcept>
#include <utility>

namespace res {

template <typename T, typename E> class result;
template <typename E> class result<void, E>;

template <typename E> class err {
  E error_;

public:
  err() = delete;
  explicit err(E error) : error_(std::move(error)) {}

  template <typename T> operator result<T, E>() const; // NOLINT(google-explicit-constructor)
};

template <typename E> template <typename T> inline err<E>::operator result<T, E>() const {
  return result<T, E>(error_, false);
}

} // namespace res

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

namespace res {

template <typename T, typename E> class result {
  static_assert(!std::is_same_v<T, void>, "T (value type) must not be void");
  static_assert(!std::is_same_v<E, void>, "E (error type) must not be void");

  bool successful_;
  T value_;
  E error_;

  result(T value, bool successful) : value_(std::move(value)), successful_(successful) {}
  result(E error, bool successful) : error_(std::move(error)), successful_(successful) {}

  template <typename U> friend class ok;
  template <typename U> friend class err;

public:
  [[nodiscard]] auto is_ok() const -> bool { return successful_; }
  explicit operator bool() const { return is_ok(); }
  auto operator!() const -> bool { return !is_ok(); }

  [[nodiscard]] auto value() const -> const T &;
  [[nodiscard]] auto error() const -> const E &;
};

// result class specialization for void value type
template <typename E> class result<void, E> {
  static_assert(!std::is_same_v<E, void>, "E (error type) must not be void");

  bool successful_;
  E error_;

  explicit result() : successful_(true) {}
  explicit result(const E &error) : error_(error), successful_(false) {}

  template <typename U> friend class ok;
  template <typename U> friend class err;

public:
  [[nodiscard]] auto is_ok() const -> bool { return successful_; }
  explicit operator bool() const { return is_ok(); }
  auto operator!() const -> bool { return !is_ok(); }

  [[nodiscard]] auto error() const -> const E &;
};

template <typename T, typename E> inline auto result<T, E>::value() const -> const T & {
  if (!is_ok()) { throw std::runtime_error("value() called on result with error"); }
  return value_;
}

template <typename T, typename E> inline auto result<T, E>::error() const -> const E & {
  if (is_ok()) { throw std::runtime_error("error() called on result with value"); }
  return error_;
}

} // namespace res

#endif // RESULT_LIB