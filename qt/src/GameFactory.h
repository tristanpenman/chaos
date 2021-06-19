#pragma once

#include <iosfwd>
#include <memory>

class Game;
class Rom;

class GameFactory
{
public:
  GameFactory() = delete;

  static std::shared_ptr<Game> build(std::shared_ptr<Rom>& m_file);
};
