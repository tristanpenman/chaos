#include <fstream>
#include <stdexcept>
#include <string>

#include "Rom.h"

// TODO: not currently used
#define CHECKSUM_OFFSET 0x018E
#define CHECKSUM_BUFFER_SIZE 0x8000  // 32kB

#define ROM_HEADER_OFFSET 0x100
#define ROM_LENGTH_OFFSET 0x01A4
#define DOMESTIC_NAME_LEN 48
#define DOMESTIC_NAME_OFFSET ROM_HEADER_OFFSET + 32
#define INTERNATIONAL_NAME_LEN 48
#define INTERNATIONAL_NAME_OFFSET DOMESTIC_NAME_OFFSET + DOMESTIC_NAME_LEN

using namespace std;

bool Rom::open(const std::string& path)
{
  m_file.open(path, ios::in | ios::out | ios::binary);

  return m_file.good();
}

string Rom::readDomesticName()
{
  char buffer[DOMESTIC_NAME_LEN + 1];

  m_file.seekg(DOMESTIC_NAME_OFFSET);
  m_file.read(buffer, DOMESTIC_NAME_LEN);

  buffer[m_file.gcount()] = 0;

  return buffer;
}

string Rom::readInternationalName()
{
  char buffer[INTERNATIONAL_NAME_LEN + 1];

  m_file.seekg(INTERNATIONAL_NAME_OFFSET);
  m_file.read(buffer, INTERNATIONAL_NAME_LEN);

  buffer[m_file.gcount()] = 0;

  return buffer;
}

uint8_t Rom::readByte(streamoff offset)
{
  streamoff bookmark = m_file.tellg();

  m_file.seekg(offset);
  uint8_t value = m_file.get();

  m_file.seekg(bookmark);

  return value;
}

uint16_t Rom::read16BitAddr(streamoff offset)
{
  streamoff bookmark = m_file.tellg();

  m_file.seekg(offset);

  uint16_t addr = m_file.get() << 8;
  addr |= m_file.get();

  m_file.seekg(bookmark);

  return addr;
}

uint32_t Rom::read32BitAddr(streamoff offset)
{
  streamoff bookmark = m_file.tellg();

  m_file.seekg(offset);

  uint32_t addr = m_file.get() << 24;
  addr |= m_file.get() << 16;
  addr |= m_file.get() << 8;
  addr |= m_file.get();

  m_file.seekg(bookmark);

  return addr;
}

void Rom::write16BitAddr(uint16_t addr, streamoff offset)
{
  streamoff bookmark = m_file.tellp();

  m_file.seekp(offset);
  m_file.put((char)((addr >> 8) & 0xFF));
  m_file.put((char)((addr) & 0xFF));

  m_file.seekp(bookmark);
}

void Rom::write32BitAddr(uint32_t addr, streamoff offset)
{
  streamoff bookmark = m_file.tellp();

  m_file.seekp(offset);
  m_file.put((char)((addr >> 24) & 0xFF));
  m_file.put((char)((addr >> 16) & 0xFF));
  m_file.put((char)((addr >> 8) & 0xFF));
  m_file.put((char)((addr) & 0xFF));

  m_file.seekp(bookmark);
}
