#pragma once

#include <iosfwd>
#include <memory>

class Rom;

class RomFactory
{
public:
  RomFactory() = delete;

  static std::shared_ptr<Rom> build(std::shared_ptr<std::istream>& m_file);
  static std::shared_ptr<Rom> build(const std::string& kind);
};
