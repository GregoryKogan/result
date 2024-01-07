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
  EXPECT_THROW(auto val = result.error(), std::logic_error); // NOLINT
}

TEST(BooleanOperations, Err) {
  const std::string error = "error";
  res::result<int, std::string> result = res::err(error);
  EXPECT_FALSE(result);
  EXPECT_TRUE(!result);
  EXPECT_FALSE(result.is_ok());
  EXPECT_TRUE(!result.is_ok());
  EXPECT_EQ(result.error(), "error");
  EXPECT_THROW(auto val = result.value(), std::logic_error); // NOLINT
}

TEST(BooleanOperations, VoidOk) {
  res::result<void, std::string> result = res::ok();
  EXPECT_TRUE(result);
  EXPECT_FALSE(!result);
  EXPECT_TRUE(result.is_ok());
  EXPECT_FALSE(!result.is_ok());
  EXPECT_THROW(auto val = result.error(), std::logic_error); // NOLINT
}

TEST(BooleanOperations, VoidErr) {
  const std::string error = "error";
  res::result<void, std::string> result = res::err(error);
  EXPECT_FALSE(result);
  EXPECT_TRUE(!result);
  EXPECT_FALSE(result.is_ok());
  EXPECT_TRUE(!result.is_ok());
  EXPECT_EQ(result.error(), "error");
  // EXPECT_THROW(auto val = result.value(), std::logic_error); - Should not compile
}