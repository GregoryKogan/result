/// @file
/// @brief result is a C++ library that provides a Result<T, E> type, which can be used to return and propagate
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

#ifndef RESULT_LIB
#define RESULT_LIB

#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>

namespace res {

template <typename T, typename E> class result;
template <typename E> class result<void, E>;

/// @brief Err object represents an unsuccessful outcome and can be implicitly converted to a result.
/// @details Err object holds an error of type E. Err object can't be empty, E must not be std::monostate.
/// @tparam E Type of the error.
template <typename E> class err {
  E error_;

public:
  err() = delete;
  explicit err(E error) : error_(std::move(error)) {}

  template <typename T> operator result<T, E>() const; // NOLINT(google-explicit-constructor)
  operator result<void, E>() const;                    // NOLINT(google-explicit-constructor)
};

template <typename E> template <typename T> inline err<E>::operator result<T, E>() const {
  return result<T, E>(result<T, E>::Unsuccessful::UNSUCCESSFUL, error_);
}

template <typename E> inline err<E>::operator result<void, E>() const {
  return result<void, E>(result<void, E>::Unsuccessful::UNSUCCESSFUL, error_);
}

} // namespace res

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

#endif // RESULT_LIB