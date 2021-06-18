#pragma once

#include "../Rom.h"

class Sonic2 : public Rom
{
public:
  Sonic2(const std::shared_ptr<std::istream>& file);

  ~Sonic2() override = default;

  bool isCompatible() override;
  bool parseLevelData() override;
};

inline Sonic2::Sonic2(const std::shared_ptr<std::istream>& file)
  : Rom(file)
{

}
