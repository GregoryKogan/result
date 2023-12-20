#include "../src/Result/Result.hpp"
#include <gtest/gtest.h>
#include <string>

TEST(IsOk, Ok) {
  const int value = 5;
  res::Result<int, std::string> result = res::Ok(value);
  EXPECT_TRUE(result.is_ok());
  EXPECT_FALSE(result.is_err());
}

TEST(IsOk, Err) {
  res::Result<int, std::string> result = res::Err(std::string("Something went wrong"));
  EXPECT_FALSE(result.is_ok());
  EXPECT_TRUE(result.is_err());
}