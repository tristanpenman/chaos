#pragma once

#include <iosfwd>
#include <memory>
#include <string>

class Rom
{
public:
  virtual ~Rom() = default;

  std::string readDomesticName();
  std::string readInternationalName();

  uint8_t readByte();
  uint8_t readByte(std::streamoff offset);

  uint16_t read16BitAddr();
  uint16_t read16BitAddr(std::streamoff offset);
  uint32_t read32BitAddr();
  uint32_t read32BitAddr(std::streamoff offset);

  void write16BitAddr(uint16_t addr);
  void write16BitAddr(uint16_t addr, std::streamoff offset);
  void write32BitAddr(uint32_t addr);
  void write32BitAddr(uint32_t addr, std::streamoff offset);

  virtual uint32_t getPalettesAddr(unsigned int levelIdx) = 0;
  virtual uint32_t getPatternsAddr(unsigned int levelIdx) = 0;
  virtual uint32_t getChunksAddr(unsigned int levelIdx) = 0;
  virtual uint32_t getBlocksAddr(unsigned int levelIdx) = 0;
  virtual uint32_t getTilesAddr(unsigned int levelIdx) = 0;

  virtual bool isCompatible() = 0;
  virtual bool parseLevelData() = 0;

protected:
  Rom(const std::shared_ptr<std::fstream>& file);

private:
  std::shared_ptr<std::fstream> m_file;
};

inline Rom::Rom(const std::shared_ptr<std::fstream>& file)
  : m_file(file)
{

}
