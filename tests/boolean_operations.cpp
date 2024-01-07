#include "../result.h"
#include <gtest/gtest.h>
#include <string>

TEST(BooleanOperations, Ok) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  EXPECT_TRUE(result);
  EXPECT_FALSE(!result);
  EXPECT_TRUE(result.is_ok());
  EXPECT_FALSE(!result.is_ok());
  EXPECT_EQ(result.value(), 42);
}

TEST(BooleanOperations, Err) {
  const std::string error = "error";
  res::result<int, std::string> result = res::err(error);
  EXPECT_FALSE(result);
  EXPECT_TRUE(!result);
  EXPECT_FALSE(result.is_ok());
  EXPECT_TRUE(!result.is_ok());
  EXPECT_EQ(result.error(), "error");
}

TEST(BooleanOperations, VoidOk) {
  res::result<void, std::string> result = res::ok();
  EXPECT_TRUE(result);
  EXPECT_FALSE(!result);
  EXPECT_TRUE(result.is_ok());
  EXPECT_FALSE(!result.is_ok());
}