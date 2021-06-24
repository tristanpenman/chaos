#include "PatternInspector.h"

PatternInspector::PatternInspector(QWidget* parent, std::shared_ptr<Level>& level)
  : QDialog(parent)
  , m_level(level)
{

}
