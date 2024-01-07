#include "../result.h"
#include <gtest/gtest.h>
#include <string>

TEST(ContentGetters, Ok) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  EXPECT_EQ(result.value(), 42);
  EXPECT_THROW(auto val = result.error(), std::logic_error); // NOLINT
}

TEST(ContentGetters, Err) {
  const std::string error = "error";
  res::result<int, std::string> result = res::err(error);
  EXPECT_THROW(auto val = result.value(), std::logic_error); // NOLINT
  EXPECT_EQ(result.error(), "error");
}

TEST(ContentGetters, VoidOk) {
  res::result<void, std::string> result = res::ok();
  EXPECT_THROW(auto val = result.error(), std::logic_error); // NOLINT
  // EXPECT_NO_THROW(auto val = result.value()); - Should not compile
}

TEST(ContentGetters, VoidErr) {
  const std::string error = "error";
  res::result<void, std::string> result = res::err(error);
  EXPECT_EQ(result.error(), "error");
  // EXPECT_THROW(auto val = result.value(), std::logic_error); - Should not compile
}

TEST(ContentGetters, ValueOrOk) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  EXPECT_EQ(result.value_or(0), 42);
}

TEST(ContentGetters, ValueOrErr) {
  const std::string error = "error";
  res::result<int, std::string> result = res::err(error);
  EXPECT_EQ(result.value_or(0), 0);
}
