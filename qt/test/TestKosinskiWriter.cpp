#include <sstream>

#include <gtest/gtest.h>

#include "../src/KosinskiReader.h"
#include "../src/KosinskiWriter.h"

#include "data/ehz1.kosinski.h"
#include "data/ehz1.raw.h"

class TestKosinskiWriter : public testing::Test
{

};

TEST_F(TestKosinskiWriter, ThrowOnNullData)
{
  std::stringstream os;
  KosinskiWriter writer;
  KosinskiWriter::Result result;
  EXPECT_ANY_THROW(writer.compress(os, nullptr, sizeof(ehz1_raw)));
}

TEST_F(TestKosinskiWriter, ReturnFalseWhenByteLimitExceeded)
{
  // 1: too small to reserve bitfield
  {
    std::stringstream os;
    KosinskiWriter writer;
    KosinskiWriter::Result result;
    EXPECT_NO_THROW(result = writer.compress(os, reinterpret_cast<const uint8_t*>(ehz1_raw), sizeof(ehz1_raw), 1));
    EXPECT_FALSE(result.first);
    EXPECT_EQ(0, os.tellp());
    EXPECT_TRUE(os.good());
  }

  // 2: too small for compressed data
  {
    std::stringstream os;
    KosinskiWriter writer;
    KosinskiWriter::Result result;
    EXPECT_NO_THROW(result = writer.compress(os, reinterpret_cast<const uint8_t*>(ehz1_raw), sizeof(ehz1_raw), 5));
    EXPECT_FALSE(result.first);
    EXPECT_EQ(0, os.tellp());
    EXPECT_TRUE(os.good());
  }
}

TEST_F(TestKosinskiWriter, ReturnTrueOnHappyPath)
{
  std::stringstream os;

  // 1: compress
  {
    KosinskiWriter writer;
    KosinskiWriter::Result result;
    EXPECT_NO_THROW(result = writer.compress(os, reinterpret_cast<const uint8_t*>(ehz1_raw), sizeof(ehz1_raw)));
    EXPECT_TRUE(result.first);
    EXPECT_TRUE(os.good());
  }

  // 2: decompress
  {
    std::vector<uint8_t> buffer(sizeof(ehz1_raw));
    KosinskiReader reader;
    KosinskiReader::Result result;
    EXPECT_NO_THROW(result = reader.decompress(os, buffer.data(), buffer.size()));
    EXPECT_TRUE(result.first);
    EXPECT_EQ(sizeof(ehz1_raw), result.second);
    for (int i = 0; i < sizeof(ehz1_raw); i++) {
      EXPECT_EQ(ehz1_raw[i], buffer[i]);
    }
  }
}

TEST_F(TestKosinskiWriter, ReturnTrueOnHappyPathWithByteLimit)
{
  std::stringstream os;
  KosinskiWriter writer;
  KosinskiWriter::Result result;
  EXPECT_NO_THROW(result = writer.compress(os, reinterpret_cast<const uint8_t*>(ehz1_raw), sizeof(ehz1_raw), 512));
  EXPECT_TRUE(result.first);
  EXPECT_TRUE(os.good());
}
