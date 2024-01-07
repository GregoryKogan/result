#pragma once

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
  auto res = result<T, E>();
  res.make_unsuccessful(error_);
  return res;
}

template <typename E> inline err<E>::operator result<void, E>() const {
  auto res = result<void, E>();
  res.make_unsuccessful(error_);
  return res;
}

} // namespace res