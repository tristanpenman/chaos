#pragma once

#include "../Rom.h"

class Sonic3 : public Rom
{
public:
  Sonic3(const std::shared_ptr<std::fstream>& file);

  ~Sonic3() override = default;

  bool isCompatible() override;
  bool parseLevelData() override;
};

inline Sonic3::Sonic3(const std::shared_ptr<std::fstream>& file)
  : Rom(file)
{

}
