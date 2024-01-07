#pragma once

#include <stdexcept>
#include <utility>

#include "../err/err.hpp"
#include "../ok/ok.hpp"

namespace res {

/// @brief `result` is a type that represents either success or failure.
///
/// result<T, E> is the type used for returning and propagating errors. It holds either a successful value of type T or
/// an error of type E.
/// @tparam T
/// @tparam E
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
  // Observers
  [[nodiscard]] auto is_ok() const -> bool { return successful_; }
  explicit operator bool() const { return is_ok(); }
  auto operator!() const -> bool { return !is_ok(); }

  [[nodiscard]] auto value() const -> const T &;
  [[nodiscard]] auto value_or(T &&default_value) const -> T;
  [[nodiscard]] auto error() const -> const E &;

  // Monadic operations
  // map method gets functor F(T x) -> U as an argument and returns result of applying this functor to the value of the
  // result object. If the result object is an error, the functor is not called and the error is propagated. But the
  // result value type is changed to U.
  template <typename F, typename U = std::invoke_result_t<F, T>> auto map(F &&functor) const -> result<U, E> {
    if (is_ok()) { return ok<U>(std::forward<F>(functor)(value_)); }
    return err<E>(error_);
  }
};

/// @brief `result` class specialization for void value type.
template <typename E> class result<void, E> {
  static_assert(!std::is_same_v<E, void>, "E (error type) must not be void");

  bool successful_;
  E error_;

  explicit result() : successful_(true) {}
  explicit result(E error) : error_(std::move(error)), successful_(false) {}

  template <typename U> friend class ok;
  template <typename U> friend class err;

public:
  // Observers
  [[nodiscard]] auto is_ok() const -> bool { return successful_; }
  explicit operator bool() const { return is_ok(); }
  auto operator!() const -> bool { return !is_ok(); }

  [[nodiscard]] auto error() const -> const E &;

  // Monadic operations
  // map method gets functor F() -> U as an argument and returns result of applying this functor to the value of the
  // result object. If the result object is an error, the functor is not called and the error is propagated. But the
  // result value type is changed to U.
  template <typename F, typename U = std::invoke_result_t<F>> auto map(F &&functor) const -> result<U, E> {
    if (is_ok()) { return ok<U>(std::forward<F>(functor)()); }
    return err<E>(error_);
  }
};

template <typename T, typename E> inline auto result<T, E>::value() const -> const T & {
  if (!is_ok()) { throw std::logic_error("value() called on result with error"); }
  return value_;
}

template <typename T, typename E> inline auto result<T, E>::value_or(T &&default_value) const -> T {
  if (is_ok()) { return value_; }
  return std::move(default_value);
}

template <typename T, typename E> inline auto result<T, E>::error() const -> const E & {
  if (is_ok()) { throw std::logic_error("error() called on result with value"); }
  return error_;
}

template <typename E> inline auto result<void, E>::error() const -> const E & {
  if (is_ok()) { throw std::logic_error("error() called on result with value"); }
  return error_;
}

} // namespace res