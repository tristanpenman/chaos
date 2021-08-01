#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

class Rom
{
public:
  bool open(const std::string& path);

  std::fstream& getFile();

  size_t getSize();

  uint32_t readAddrRange();
  void writeSize(uint32_t size);

  uint16_t calculateChecksum();
  uint16_t readChecksum();
  void writeChecksum(uint16_t checksum);

  std::string readDomesticName();
  std::string readInternationalName();

  uint8_t readByte(std::streamoff offset);

  std::vector<char> readBytes(std::streamoff offset, size_t count);

  uint16_t read16BitAddr(std::streamoff offset);
  uint32_t read32BitAddr(std::streamoff offset);

  void write16BitAddr(uint16_t addr, std::streamoff offset);
  void write32BitAddr(uint32_t addr, std::streamoff offset);

private:
  std::fstream m_file;
};
