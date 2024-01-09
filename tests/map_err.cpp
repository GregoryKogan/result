#include "../result.h"
#include <gtest/gtest.h>
#include <string>

TEST(MapErr, Err) {
  const std::string value = "error";
  res::result<int, std::string> result = res::err(value);
  auto mapped_result = result.map_err([](const std::string &val) -> std::string { return val + " mapped"; });
  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), "error mapped");
}

TEST(MapErr, Ok) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  auto mapped_result = result.map_err([](const std::string &val) -> std::string { return val + " mapped"; });
  EXPECT_TRUE(mapped_result);
  EXPECT_EQ(mapped_result.value(), 42);
}

TEST(MapErr, ErrChangeType) {
  const std::string value = "error";
  res::result<int, std::string> result = res::err(value);
  auto mapped_result = result.map_err([](const std::string &val) -> std::size_t { return val.size(); });
  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), 5);
}

TEST(MapErr, OkChangeType) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  auto mapped_result = result.map_err([](const std::string &val) -> std::size_t { return val.size(); });
  EXPECT_TRUE(mapped_result);
  EXPECT_EQ(mapped_result.value(), 42);
}

TEST(MapErr, ErrVoid) {
  const std::string value = "error";
  res::result<void, std::string> result = res::err(value);
  auto mapped_result = result.map_err([](const std::string &val) -> std::size_t { return val.size(); });
  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), 5);
}

TEST(MapErr, OkVoid) {
  res::result<void, std::string> result = res::ok();
  auto mapped_result = result.map_err([](const std::string &val) -> std::size_t { return val.size(); });
  EXPECT_TRUE(mapped_result);
}

TEST(MapErr, ErrChained) {
  const std::string value = "error";
  res::result<int, std::string> result = res::err(value);
  auto mapped_result = result.map_err([](const std::string &val) -> std::string { return val + " mapped"; })
                           .map_err([](const std::string &val) -> std::size_t { return val.size(); });
  EXPECT_FALSE(mapped_result);
  EXPECT_EQ(mapped_result.error(), 12);
}

TEST(MapErr, OkChained) {
  const int value = 42;
  res::result<int, std::string> result = res::ok(value);
  auto mapped_result = result.map_err([](const std::string &val) -> std::string { return val + " mapped"; })
                           .map_err([](const std::string &val) -> std::size_t { return val.size(); });
  EXPECT_TRUE(mapped_result);
  EXPECT_EQ(mapped_result.value(), 42);
}