#include <fstream>
#include <stdexcept>
#include <string>

#include "Rom.h"

#define CHECKSUM_OFFSET 0x018E
#define CHECKSUM_BUFFER_SIZE 0x8000  // 32kB
#define ROM_HEADER_OFFSET 0x100
#define ROM_LENGTH_OFFSET 0x01A4
#define DOMESTIC_NAME_LEN 48
#define DOMESTIC_NAME_OFFSET ROM_HEADER_OFFSET + 32
#define INTERNATIONAL_NAME_LEN 48
#define INTERNATIONAL_NAME_OFFSET DOMESTIC_NAME_OFFSET + DOMESTIC_NAME_LEN

using namespace std;

bool Rom::open(const string& path)
{
  m_file.open(path, ios::in | ios::out | ios::binary);

  return m_file.good();
}

fstream& Rom::getFile()
{
  return m_file;
}

size_t Rom::getSize()
{
  m_file.seekg(0, ios::end);
  return m_file.tellg();
}

uint32_t Rom::readAddrRange()
{
  return read32BitAddr(0x1A4);
}

void Rom::writeSize(uint32_t size)
{
  write32BitAddr(size, 0x1A4);

  m_file.flush();
}

uint16_t Rom::calculateChecksum()
{
  vector<char> buffer(CHECKSUM_BUFFER_SIZE);
  m_file.seekg(512);
  int count = 0;
  while (!m_file.eof()) {
    m_file.read(buffer.data(), buffer.size());
    auto readCount = m_file.gcount();
    for (auto i = 0; i < readCount; i += 2) {
      int num;

      if (buffer[i] < 0) {
        num = buffer[i] + 256;
      } else {
        num = buffer[i];
      }

      count += num << 8;

      if ((i + 1) < readCount) {
        if (buffer[i + 1] < 0) {
          num = buffer[i + 1] + 256;
        } else {
          num = buffer[i + 1];
        }

        count += num;
      }

      count &= 0xFFFF;
    }
  }

  m_file.clear();

  return static_cast<uint16_t>(count);
}

uint16_t Rom::readChecksum()
{
  return read16BitAddr(CHECKSUM_OFFSET);
}

void Rom::writeChecksum(uint16_t checksum)
{
  write16BitAddr(checksum, CHECKSUM_OFFSET);

  m_file.flush();
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
  m_file.seekg(offset);
  uint8_t value = static_cast<uint8_t>(m_file.get());

  return value;
}

vector<char> Rom::readBytes(streamoff offset, size_t count)
{
  vector<char> buffer(count);

  m_file.seekg(offset);

  m_file.read(buffer.data(), count);
  buffer.resize(m_file.gcount());

  return buffer;
}

uint16_t Rom::read16BitAddr(streamoff offset)
{
  m_file.seekg(offset);

  uint16_t addr = static_cast<uint16_t>(m_file.get()) << 8;
  addr |= static_cast<uint16_t>(m_file.get());

  return addr;
}

uint32_t Rom::read32BitAddr(streamoff offset)
{
  m_file.seekg(offset);

  uint32_t addr = static_cast<uint32_t>(m_file.get()) << 24;
  addr |= static_cast<uint32_t>(m_file.get()) << 16;
  addr |= static_cast<uint32_t>(m_file.get()) << 8;
  addr |= static_cast<uint32_t>(m_file.get());

  return addr;
}

void Rom::write16BitAddr(uint16_t addr, streamoff offset)
{
  m_file.seekp(offset);

  m_file.put(static_cast<char>((addr >> 8) & 0xFF));
  m_file.put(static_cast<char>((addr) & 0xFF));
}

void Rom::write32BitAddr(uint32_t addr, streamoff offset)
{
  m_file.seekp(offset);

  m_file.put(static_cast<char>((addr >> 24) & 0xFF));
  m_file.put(static_cast<char>((addr >> 16) & 0xFF));
  m_file.put(static_cast<char>((addr >> 8) & 0xFF));
  m_file.put(static_cast<char>((addr) & 0xFF));
}
