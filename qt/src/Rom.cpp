#include <istream>
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

string Rom::readDomesticName()
{
  char buffer[DOMESTIC_NAME_LEN + 1];

  m_file->seekg(DOMESTIC_NAME_OFFSET);
  m_file->read(buffer, DOMESTIC_NAME_LEN);

  buffer[m_file->gcount()] = 0;

  return buffer;
}

string Rom::readInternationalName()
{
  char buffer[INTERNATIONAL_NAME_LEN + 1];

  m_file->seekg(INTERNATIONAL_NAME_OFFSET);
  m_file->read(buffer, INTERNATIONAL_NAME_LEN);

  buffer[m_file->gcount()] = 0;

  return buffer;
}
