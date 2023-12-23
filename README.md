# result-cpp

![GitHub License](https://img.shields.io/github/license/GregoryKogan/result-cpp)
![Github Tests workflow](https://img.shields.io/github/actions/workflow/status/GregoryKogan/result-cpp/tests.yml?logo=github&label=tests)
![Github Tests workflow](https://img.shields.io/github/actions/workflow/status/GregoryKogan/result-cpp/docs.yml?logo=files&logoColor=f5f5f5&label=docs)

Result-CPP is a C++ library that provides a `Result<T, E>` type, which can be used to return and propagate errors. It's inspired by Rust's `std::Result` type.

## Features

- `Result<T, E>` type for returning and propagating errors.
- `Ok` and `Err` static methods for creating successful and unsuccessful `Result` objects respectively.
- `is_ok` and `is_err` methods for checking if the `Result` is successful or unsuccessful.
- `unwrap` and `unwrap_err` methods for extracting the value or error from the `Result`.

## Usage

```cpp
#include <iostream>
#include "result.h"

res::Result<int, std::string> divide(int a, int b) {
  if (b == 0) {
    return res::Err(std::string("Division by zero"));
  } else {
    return res::Ok(a / b);
  }
}

int main() {
  auto result = divide(10, 2);

  if (result.is_ok()) {
    std::cout << "Result: " << result.unwrap() << '\n';
  } else {
    std::cout << "Error: " << result.unwrap_err() << '\n';
  }
}
```
