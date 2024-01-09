#include "../result.h"
#include <gtest/gtest.h>
#include <string>

TEST(Map, Ok) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  auto mapped_result = result.map([](int val) -> int { return val * 2; });
  EXPECT_TRUE(mapped_result);
  EXPECT_EQ(mapped_result.value(), 84);
}

TEST(Map, Err) {
  const std::string value = "error";
  res::result<int, std::string> result = res::err(value);
  auto mapped_result = result.map([](int val) -> int { return val * 2; });
  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), "error");
}

TEST(Map, OkChangeType) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  auto mapped_result = result.map([](int val) -> std::string { return std::to_string(val * 2); });
  EXPECT_TRUE(mapped_result);
  EXPECT_EQ(mapped_result.value(), "84");
}

TEST(Map, ErrChangeType) {
  const std::string value = "error";
  res::result<int, std::string> result = res::err(value);
  auto mapped_result = result.map([](int val) -> std::string { return std::to_string(val * 2); });
  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), "error");
}

TEST(Map, OkVoid) {
  res::result<void, std::string> result = res::ok();
  auto mapped_result = result.map([]() -> int {
    const int value = 42;
    return value;
  });

  EXPECT_TRUE(mapped_result);
  EXPECT_EQ(mapped_result.value(), 42);
}

TEST(Map, ErrVoid) {
  const std::string value = "error";
  res::result<void, std::string> result = res::err(value);
  auto mapped_result = result.map([]() -> int {
    const int value = 42;
    return value;
  });

  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), "error");
}

TEST(Map, OkChained) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  auto mapped_result = result.map([](int val) -> int { return val * 2; }).map([](int val) -> int { return val * 2; });
  EXPECT_TRUE(mapped_result);
  EXPECT_EQ(mapped_result.value(), 168);
}

TEST(Map, ErrChained) {
  const std::string value = "error";
  res::result<int, std::string> result = res::err(value);
  auto mapped_result = result.map([](int val) -> int { return val * 2; }).map([](int val) -> int { return val * 2; });
  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), "error");
}