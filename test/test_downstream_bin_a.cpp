#include <gtest/gtest.h>

#include "downstream_bin_a/downstream_bin_a.hpp"

TEST(DownstreamBinA, LabelsThroughTheWholeChain)
{
  EXPECT_EQ(
    "demo: elements=2",
    downstream_bin_a::label_from_yaml("name: demo\nxml: \"<a><b/></a>\"\n"));
}

TEST(DownstreamBinA, InvalidXmlPayload)
{
  EXPECT_EQ(
    "demo: invalid",
    downstream_bin_a::label_from_yaml("name: demo\nxml: \"<a>\"\n"));
}

TEST(DownstreamBinA, MissingKeyIsInvalid)
{
  EXPECT_EQ("invalid", downstream_bin_a::label_from_yaml("name: demo\n"));
}

TEST(DownstreamBinA, MalformedYamlIsInvalid)
{
  EXPECT_EQ("invalid", downstream_bin_a::label_from_yaml("name: [unclosed\n"));
}

TEST(DownstreamBinA, NestedValuesAreIgnored)
{
  EXPECT_EQ(
    "demo: elements=1",
    downstream_bin_a::label_from_yaml(
      "extra: {nested: 1}\nname: demo\nxml: \"<a/>\"\n"));
}
