#include "PencilCommand.h"

#include "../Map.h"

using namespace std;

PencilCommand::PencilCommand(Map& map, int x, int y, int block)
  : m_map(map)
  , m_x(x)
  , m_y(y)
  , m_block(block)
{

}

PencilCommand::Result PencilCommand::perform()
{
  // update the map
  const auto oldValue = m_map.getValue(0, m_x, m_y);
  m_map.setValue(0, m_x, m_y, m_block);

  // make undo command
  auto undoCommand = std::make_shared<PencilCommand>(m_map, m_x, m_y, oldValue);

  // make list of changes
  vector<Change> changes = { { 0, m_x, m_y, m_block } };

  return {
    undoCommand,
    changes
  };
}
