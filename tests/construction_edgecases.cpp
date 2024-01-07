#include "../result.h"
#include <gtest/gtest.h>
#include <string>

TEST(ConstructionEdgeCases, SameTypeOk) {
  const int value = 42;
  res::result<int, int> result = res::ok(value);
  ASSERT_TRUE(result);
  EXPECT_EQ(result.value(), 42);
  EXPECT_THROW(auto val = result.error(), std::logic_error); // NOLINT
}

TEST(ConstructionEdgeCases, SameTypeErr) {
  const int value = 42;
  res::result<int, int> result = res::err(value);
  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), 42);
  EXPECT_THROW(auto val = result.value(), std::logic_error); // NOLINT
}