#include "PaletteInspector.h"

#include "../Level.h"

PaletteInspector::PaletteInspector(QWidget* parent, std::shared_ptr<Level>& level)
  : QDialog(parent)
  , m_level(level)
{

}
