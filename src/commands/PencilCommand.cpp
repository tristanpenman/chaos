#include "PencilCommand.h"

#include "../Map.h"

using namespace std;

PencilCommand::PencilCommand(Map& map)
  : m_map(map)
{

}

void PencilCommand::addChange(uint8_t layer, uint16_t x, uint16_t y, uint8_t value)
{
  m_changes[{layer, x, y}] = value;
}

PencilCommand::Result PencilCommand::commit()
{
  // make undo command
  auto undoCommand = std::make_shared<PencilCommand>(m_map);

  // update the map
  vector<Change> changes;
  for (auto& entry : m_changes) {
    const auto layer = entry.first.layer;
    const auto x = entry.first.x;
    const auto y = entry.first.y;
    const auto value = entry.second;

    // save old value to undo command
    const auto oldValue = m_map.getValue(layer, x, y);
    undoCommand->addChange(layer, x, y, oldValue);

    // commit changes
    m_map.setValue(layer, x, y, value);
    changes.push_back({ layer, x, y, value });
  }

  return {
    undoCommand,
    changes
  };
}
