#include <cmath>

#include <QComboBox>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "../Level.h"
#include "../Logger.h"
#include "../Palette.h"
#include "../Pattern.h"

#include "PatternInspector.h"

#define LOG Logger("PatternInspector")

using namespace std;

static constexpr int PIXMAP_WIDTH = 320;
static constexpr int PATTERNS_PER_ROW = PIXMAP_WIDTH / Pattern::PATTERN_WIDTH;

PatternInspector::PatternInspector(QWidget* parent, shared_ptr<Level>& level)
  : QDialog(parent)
  , m_level(level)
  , m_pixmap(nullptr)
{
  const auto patternCount = level->getPatternCount();
  const int pixmapHeight = ceilf(static_cast<float>(patternCount) / PATTERNS_PER_ROW) * Pattern::PATTERN_HEIGHT;

  // main layout
  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->setContentsMargins(8, 8, 8, 8);
  vbox->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(vbox);

  // palette combo box
  QComboBox* paletteCombo = new QComboBox();
  vbox->addWidget(paletteCombo);
  for (size_t i = 0; i < level->getPaletteCount(); i++) {
    const QString paletteName = QString("Pallete %1").arg(i);
    paletteCombo->addItem(paletteName, QVariant::fromValue(i));
  }

  // create widget to display pixmap
  m_label = new QLabel();
  m_label->setFixedSize(PIXMAP_WIDTH, pixmapHeight);
  m_label->setMinimumWidth(PIXMAP_WIDTH);
  vbox->addWidget(m_label);

  // create pixmap
  m_pixmap = new QPixmap(PIXMAP_WIDTH, pixmapHeight);
  m_label->setPixmap(*m_pixmap);
  drawPatterns(0);

  // handle switching palettes
  connect(paletteCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(paletteChanged(int)));
}

void PatternInspector::drawPattern(QImage& image, const Pattern& pattern, const Palette& palette, int dx, int dy)
{
  for (int py = 0; py < 8; py++) {
    for (int px = 0; px < 8; px++) {
      const auto idx = pattern.getPixel(px, py);
      const auto color = palette.getColor(idx);

      image.setPixel(dx + px, dy + py, qRgb(color.r, color.g, color.b));
    }
  }
}

void PatternInspector::drawPatterns(size_t paletteIndex)
{
  LOG << "Drawing patterns using palette " << paletteIndex;

  const Palette& palette = m_level->getPalette(paletteIndex);

  // image to draw to
  QImage image(m_pixmap->width(), m_pixmap->height(), QImage::Format_RGB888);
  image.fill(qRgb(0, 0, 0));

  // draw individual patterns
  for (size_t i = 0; i < m_level->getPatternCount(); i++) {
    const auto row = static_cast<int>(i / PATTERNS_PER_ROW);
    const auto col = static_cast<int>(i % PATTERNS_PER_ROW);

    drawPattern(image, m_level->getPattern(i), palette, col * Pattern::PATTERN_WIDTH, row * Pattern::PATTERN_HEIGHT);
  }

  // copy to pixmap
  LOG << "Copying pattern image to pixmap";
  if (m_pixmap->convertFromImage(image)) {
    m_label->setPixmap(*m_pixmap);
  } else {
    LOG << "Failed to copy image to pixmap";
  }
}

void PatternInspector::paletteChanged(int paletteIndex)
{
  drawPatterns(paletteIndex);
}
