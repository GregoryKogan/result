#pragma once

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