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

  virtual bool isCompatible() = 0;
  virtual bool parseLevelData() = 0;

protected:
  Rom(const std::shared_ptr<std::istream>& file);

private:
  std::shared_ptr<std::istream> m_file;
};

inline Rom::Rom(const std::shared_ptr<std::istream>& file)
  : m_file(file)
{

}
