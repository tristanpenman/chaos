#pragma once

#include <fstream>
#include <optional>
#include <string>

class Rom
{
public:
  bool open(const std::string& path);

  std::string readDomesticName();
  std::string readInternationalName();

  uint8_t readByte(std::streamoff offset);

  uint16_t read16BitAddr(std::streamoff offset);
  uint32_t read32BitAddr(std::streamoff offset);

  void write16BitAddr(uint16_t addr, std::streamoff offset);
  void write32BitAddr(uint32_t addr, std::streamoff offset);

private:
  std::fstream m_file;
};
