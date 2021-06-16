#include "Precompiled.h"

#include "SegaRom.h"

#define CHECKSUM_OFFSET 0x018E
#define CHECKSUM_BUFFER_SIZE 0x8000  // 32kB

#define ROM_HEADER_OFFSET 0x100

#define ROM_LENGTH_OFFSET 0x01A4

#define DOMESTIC_NAME_LEN 48
#define DOMESTIC_NAME_OFFSET ROM_HEADER_OFFSET + 32
#define INTERNATIONAL_NAME_LEN 48
#define INTERNATIONAL_NAME_OFFSET DOMESTIC_NAME_OFFSET + DOMESTIC_NAME_LEN

using namespace std;

SegaRom::SegaRom(fstream& file)
: m_file(file)
{

}

string SegaRom::getDomesticName()
{
	char buffer[DOMESTIC_NAME_LEN + 1];

	m_file.seekg(DOMESTIC_NAME_OFFSET);
	m_file.read(buffer, DOMESTIC_NAME_LEN);

	buffer[m_file.gcount()] = 0;

	return buffer;
}

string SegaRom::getInternationalName()
{
	char buffer[INTERNATIONAL_NAME_LEN + 1];

	m_file.seekg(INTERNATIONAL_NAME_OFFSET);
	m_file.read(buffer, INTERNATIONAL_NAME_LEN);

	buffer[m_file.gcount()] = 0;

	return buffer;
}

void SegaRom::fixChecksum(uint32_t length)
{
	char buffer[CHECKSUM_BUFFER_SIZE];
	uint16_t count = 0;

	// Header is not included in the checksum
	m_file.seekg(512);

	while (!m_file.eof())
	{
		m_file.read(buffer, CHECKSUM_BUFFER_SIZE);
		
		streamsize read_count = m_file.gcount();
		int num = 0;

		for (int i = 0; i < read_count; i += 2)
		{
			if (buffer[i] < 0)
			{
				num = buffer[i] + 256;
			}
			else
			{
				num = buffer[i];
			}

			count += num << 8;

			if ((i + 1) < read_count) 
			{
				if (buffer[i + 1] < 0) 
				{
					num = buffer[i + 1] + 256;
				}
				else 
				{
					num = buffer[i + 1];
				}

				count += num;
			}

			count &= 0xFFFF;
		}
	}

	// Count is a number, but it should be written like an address
	writeAddress_16bit_at(count, CHECKSUM_OFFSET);
}

void SegaRom::fixHeader(uint32_t rom_length)
{
	writeAddress_32bit_at(rom_length, ROM_LENGTH_OFFSET);
}

uint16_t SegaRom::readAddress_16bit()
{
	streamoff o;
	
	// Read a 16-bit big-endian address
	o  = m_file.get() << 8;
	o |= m_file.get();

	// Return little-endian address
	return o & 0xFFFF;
}

uint16_t SegaRom::readAddress_16bit_at(streamoff src_offset)
{
	streamoff o;
	streamoff p = m_file.tellg();
	
	m_file.seekg(src_offset);
	
	// Read a 16-bit big-endian address
	o  = m_file.get() << 8;
	o |= m_file.get();

	m_file.seekg(p);

	// Return little-endian address
	return o & 0xFFFF;
}

uint32_t SegaRom::readAddress_32bit()
{
	uint32_t o;

	// Read a 32-bit big-endian address
	o  = m_file.get() << 24;
	o |= m_file.get() << 16;
	o |= m_file.get() << 8;
	o |= m_file.get();

	// Return little-endian address
	return o;
}

uint32_t SegaRom::readAddress_32bit_at(streamoff src_offset)
{
	uint32_t o;
	streamoff p = m_file.tellg();
	
	m_file.seekg(src_offset);

	// Read a 32-bit big-endian address
	o  = m_file.get() << 24;
	o |= m_file.get() << 16;
	o |= m_file.get() << 8;
	o |= m_file.get();

	m_file.seekg(p);

	// Return little-endian address
	return o;
}

void SegaRom::writeAddress_16bit(uint16_t address)
{
	m_file.put((char)((address >> 8) & 0xFF));
	m_file.put((char)((address)      & 0xFF));
}

void SegaRom::writeAddress_16bit_at(uint16_t address, std::streamoff offset)
{
	streamoff p = m_file.tellp();

	m_file.seekp(offset);

	m_file.put((char)((address >> 8) & 0xFF));
	m_file.put((char)((address)      & 0xFF));

	m_file.seekp(p);
}


void SegaRom::writeAddress_32bit(uint32_t address)
{
	m_file.put((char)((address >> 24) & 0xFF));
	m_file.put((char)((address >> 16) & 0xFF));
	m_file.put((char)((address >> 8)  & 0xFF));
	m_file.put((char)((address)       & 0xFF));
}

void SegaRom::writeAddress_32bit_at(uint32_t address, std::streamoff offset)
{
	streamoff p = m_file.tellp();

	m_file.seekp(offset);

	m_file.put((char)((address >> 24) & 0xFF));
	m_file.put((char)((address >> 16) & 0xFF));
	m_file.put((char)((address >> 8)  & 0xFF));
	m_file.put((char)((address)       & 0xFF));

	m_file.seekp(p);
}
