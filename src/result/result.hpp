#pragma once

#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>

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

  std::variant<T, E> content_;

  // Simulate named constructors
  enum Successful { SUCCESSFUL };
  enum Unsuccessful { UNSUCCESSFUL };
  result(Successful successful, T value) { content_.template emplace<0>(std::move(value)); }
  result(Unsuccessful unsuccessful, E error) { content_.template emplace<1>(std::move(error)); }

  template <typename U> friend class ok;
  template <typename U> friend class err;

public:
  // Observers
  [[nodiscard]] auto is_ok() const -> bool { return content_.index() == 0; }
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
    if (is_ok()) { return ok<U>(std::forward<F>(functor)(value())); }
    return err<E>(error());
  }
  // map_err method gets functor F(E x) -> R as an argument and returns result of applying this functor to the error of
  // the result object. If the result object is a success, the functor is not called and the value is propagated. But
  // the result error type is changed to R.
  template <typename F, typename R = std::invoke_result_t<F, E>> auto map_err(F &&functor) const -> result<T, R> {
    if (!is_ok()) { return err<R>(std::forward<F>(functor)(error())); }
    return ok<T>(value());
  }
};

/// @brief `result` class specialization for void value type.
template <typename E> class result<void, E> {
  static_assert(!std::is_same_v<E, void>, "E (error type) must not be void");

  std::optional<E> error_;

  // Simulate named constructors
  enum Successful { SUCCESSFUL };
  enum Unsuccessful { UNSUCCESSFUL };
  explicit result(Successful successful) {}
  result(Unsuccessful unsuccessful, E error) : error_(std::move(error)) {}

  template <typename U> friend class ok;
  template <typename U> friend class err;

public:
  // Observers
  [[nodiscard]] auto is_ok() const -> bool { return !error_.has_value(); }
  explicit operator bool() const { return is_ok(); }
  auto operator!() const -> bool { return !is_ok(); }

  [[nodiscard]] auto error() const -> const E &;

  // Monadic operations
  // map method gets functor F() -> U as an argument and returns result of applying this functor to the value of the
  // result object. If the result object is an error, the functor is not called and the error is propagated. But the
  // result value type is changed to U.
  template <typename F, typename U = std::invoke_result_t<F>> auto map(F &&functor) const -> result<U, E> {
    if (is_ok()) { return ok<U>(std::forward<F>(functor)()); }
    return err<E>(error_.value());
  }
  // map_err method for void value type is same as for non-void value type because it does not depend on the value type.
  template <typename F, typename R = std::invoke_result_t<F, E>> auto map_err(F &&functor) const -> result<void, R> {
    if (!is_ok()) { return err<R>(std::forward<F>(functor)(error_.value())); }
    return ok();
  }
};

template <typename T, typename E> inline auto result<T, E>::value() const -> const T & {
  if (!is_ok()) { throw std::logic_error("value() called on result with error"); }
  return std::get<0>(content_);
}

template <typename T, typename E> inline auto result<T, E>::value_or(T &&default_value) const -> T {
  if (is_ok()) { return std::get<0>(content_); }
  return std::move(default_value);
}

template <typename T, typename E> inline auto result<T, E>::error() const -> const E & {
  if (is_ok()) { throw std::logic_error("error() called on result with value"); }
  return std::get<1>(content_);
}

template <typename E> inline auto result<void, E>::error() const -> const E & {
  if (is_ok()) { throw std::logic_error("error() called on result with value"); }
  return error_.value();
}

} // namespace res