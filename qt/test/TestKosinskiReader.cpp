#include <gtest/gtest.h>

#include "../src/KosinskiReader.h"

class TestKosinskiReader : public testing::Test
{

};

TEST_F(TestKosinskiReader, ThrowOnEmptyStream)
{
  uint8_t buffer[16];

  std::stringstream ss;
  KosinskiReader reader;
  EXPECT_ANY_THROW(reader.decompress(ss, buffer, 16));
}

TEST_F(TestKosinskiReader, ThrowOnIncompleteSteam)
{
  uint8_t buffer[16];

  // case 1: literal bytes
  {
    std::stringstream ss;
    ss << char(0xff);
    ss << char(0xff);
    ss.seekg(0);
    KosinskiReader reader;
    EXPECT_ANY_THROW(reader.decompress(ss, buffer, 16));
  }

  // case 2: various
  {
    std::stringstream ss;
    ss << char(0x33);
    ss << char(0x66);
    ss.seekg(0);
    KosinskiReader reader;
    EXPECT_ANY_THROW(reader.decompress(ss, buffer, 16));
  }
}

TEST_F(TestKosinskiReader, ReturnFalseOnNullBuffer)
{
  std::stringstream ss;
  ss << char(0xff);
  ss << char(0xff);
  ss << char(0x01);
  ss << char(0x02);
  ss.seekg(0);
  KosinskiReader reader;
  auto result = reader.decompress(ss, nullptr, 0);
  EXPECT_FALSE(result.first);
  EXPECT_EQ(0, result.second);
}

TEST_F(TestKosinskiReader, ReturnFalseOnBufferOverflow)
{
  uint8_t buffer[1];

  // case 1: literal byte mode
  {
    std::stringstream ss;
    ss << char(0xff);
    ss << char(0xff);
    ss << char(0x01);
    ss << char(0x02);
    ss.seekg(0);
    KosinskiReader reader;
    auto result = reader.decompress(ss, buffer, 1);
    EXPECT_FALSE(result.first);
    EXPECT_EQ(1, result.second);
  }
}
