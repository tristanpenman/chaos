#ifndef __SEGA_ROM_H
#define __SEGA_ROM_H

/******************************************************************************
 *
 * SEGA ROM file wrapper
 *
 * Provides access to common features of Sega Genesis/Megadrive ROMs, and
 * methods to read/write big-endian addresses.
 *
 *****************************************************************************/

class SegaRom 
{
public:

	explicit SegaRom(std::fstream& file);
	virtual ~SegaRom() {}

	std::fstream&  getFile();
	std::string    getIdentifier();
	size_t         getSize();


	/** HEADER MODIFIERS **/

	void fixChecksum(std::streamoff length);
	void fixHeader(std::streamoff length);

	std::string getDomesticName();
	std::string getInternationalName();


	/** BIG-ENDIAN ADDRESS READERS **/

	std::streamoff readAddress_16bit();					   // Read address at current offset
	std::streamoff readAddress_16bit_at(std::streamoff o); // Read address at offset 'o' 
	std::streamoff readAddress_32bit();
	std::streamoff readAddress_32bit_at(std::streamoff o);

	
	/** BIG-ENDIAN ADDRESS WRITERS **/

	void writeAddress_16bit(std::streamoff address);
	void writeAddress_16bit_at(std::streamoff address, std::streamoff offset);
	void writeAddress_32bit(std::streamoff address);
	void writeAddress_32bit_at(std::streamoff address, std::streamoff offset);

protected:
	std::fstream& m_file;

};

inline std::fstream& SegaRom::getFile()
{
	return m_file;
}

#endif
