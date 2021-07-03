#include "games/Sonic2.h"
#include "games/Sonic3.h"

#include "GameFactory.h"

using namespace std;

shared_ptr<Game> GameFactory::build(shared_ptr<Rom>& rom)
{
  // try Sonic2
  shared_ptr<Game> game = make_shared<Sonic2>(rom);
  if (game->isCompatible()) {
    return game;
  }

  // try Sonic3
  game.reset(new Sonic3(rom));
  if (game->isCompatible()) {
    return game;
  }

  return nullptr;
}
