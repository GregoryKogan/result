#include "../src/result.h"
#include <gtest/gtest.h>
#include <string>

TEST(Types, SameInt) {
  const int value = 42;
  const int error = 43;
  res::Result<int, int> ok_result = res::Ok(value);
  res::Result<int, int> err_result = res::Err(error);

  EXPECT_TRUE(ok_result.is_ok());
  EXPECT_FALSE(ok_result.is_err());
  EXPECT_EQ(ok_result.unwrap(), value);
  EXPECT_THROW(auto ___ = ok_result.unwrap_err(), std::runtime_error);

  EXPECT_TRUE(err_result.is_err());
  EXPECT_FALSE(err_result.is_ok());
  EXPECT_EQ(err_result.unwrap_err(), error);
  EXPECT_THROW(auto ___ = err_result.unwrap(), std::runtime_error);
}

TEST(Types, SameString) {
  const std::string value = "Hello";
  const std::string error = "World";
  res::Result<std::string, std::string> ok_result = res::Ok(value);
  res::Result<std::string, std::string> err_result = res::Err(error);

  EXPECT_TRUE(ok_result.is_ok());
  EXPECT_FALSE(ok_result.is_err());
  EXPECT_EQ(ok_result.unwrap(), value);
  EXPECT_THROW(auto ___ = ok_result.unwrap_err(), std::runtime_error);

  EXPECT_TRUE(err_result.is_err());
  EXPECT_FALSE(err_result.is_ok());
  EXPECT_EQ(err_result.unwrap_err(), error);
  EXPECT_THROW(auto ___ = err_result.unwrap(), std::runtime_error);
}

TEST(Types, EmptyValue) {
  res::Result<std::monostate, std::string> empty_ok_result = res::Ok();
  res::Result<std::monostate, std::string> empty_err_result = res::Err(std::string("Hello"));

  EXPECT_TRUE(empty_ok_result.is_ok());
  EXPECT_FALSE(empty_ok_result.is_err());
  EXPECT_THROW(auto ___ = empty_ok_result.unwrap(), std::runtime_error);
  EXPECT_THROW(auto ___ = empty_ok_result.unwrap_err(), std::runtime_error);

  EXPECT_TRUE(empty_err_result.is_err());
  EXPECT_FALSE(empty_err_result.is_ok());
  EXPECT_EQ(empty_err_result.unwrap_err(), "Hello");
  EXPECT_THROW(auto ___ = empty_err_result.unwrap(), std::runtime_error);
}