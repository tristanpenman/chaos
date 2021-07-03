#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../Level.h"
#include "../Palette.h"

#include "PaletteInspector.h"

PaletteInspector::PaletteInspector(QWidget* parent, std::shared_ptr<Level>& level)
  : QDialog(parent)
  , m_level(level)
{
  setWindowFlag(Qt::WindowStaysOnTopHint);

  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->setContentsMargins(8, 8, 8, 8);
  vbox->setSpacing(0);

  for (size_t r = 0; r < m_level->getPaletteCount(); r++) {
    const Palette& palette = m_level->getPalette(r);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->setSpacing(0);

    for (size_t c = 0; c < palette.getColorCount(); c++) {
      const Palette::Color& color = palette.getColor(c);
      const QString stylesheet = QString("background: rgb(%1,%2,%3)")
          .arg(color.r)
          .arg(color.g)
          .arg(color.b);

      QWidget* cell = new QWidget();
      cell->setMinimumSize(20, 20);
      cell->setStyleSheet(stylesheet);

      hbox->addWidget(cell);
    }

    vbox->addLayout(hbox);
  }

  setLayout(vbox);
}
