#include "../src/result.h"
#include <gtest/gtest.h>
#include <string>

TEST(Unwrap, Ok) {
  const int value = 5;
  res::Result<int, std::string> result = res::Ok(value);

  EXPECT_EQ(result.unwrap(), value);
  EXPECT_THROW(auto res = result.unwrap_err(), std::runtime_error);
}

TEST(Unwrap, Err) {
  const std::string value = "Something went wrong";
  res::Result<int, std::string> result = res::Err(value);

  EXPECT_EQ(result.unwrap_err(), value);
  EXPECT_THROW(auto res = result.unwrap(), std::runtime_error);
}