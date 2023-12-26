#include "../src/result.h"
#include <gtest/gtest.h>
#include <string>

TEST(AccessOperators, Star) {
  const int int_value = 5;
  res::Result<int, std::string> int_result = res::Ok(int_value);

  std::string str_value = "some string";
  res::Result<std::string, int> str_result = res::Ok(str_value);

  EXPECT_EQ(*int_result, int_value);
  EXPECT_EQ(*str_result, str_value);
}

TEST(AccessOperators, Arrow) {
  std::string str_value = "some string";
  res::Result<std::string, int> result = res::Ok(str_value);

  EXPECT_EQ(result->length(), str_value.length());
}