#include "Result/Result.hpp"
#include <iostream>

auto good() -> res::Result<int, std::string> { return res::Ok(5); }
auto bad() -> res::Result<int, std::string> { return res::Err(std::string("Something went wrong")); }

auto main() -> int {
  auto result = good();
  if (result.is_ok()) {
    std::cout << "Good: " << result.unwrap() << '\n';
  } else {
    std::cout << "Bad: " << result.unwrap_err() << '\n';
  }

  result = bad();
  if (result.is_ok()) {
    std::cout << "Good: " << result.unwrap() << '\n';
  } else {
    std::cout << "Bad: " << result.unwrap_err() << '\n';
  }
  return 0;
}