#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include "BlockInspector.h"

#include "../Block.h"
#include "../Chunk.h"
#include "../Level.h"
#include "../Logger.h"
#include "../Palette.h"
#include "../Pattern.h"

#define LOG Logger("BlockInspector")

using namespace std;

BlockInspector::BlockInspector(QWidget* parent, shared_ptr<Level>& level)
  : QDialog(parent)
  , m_level(level)
{
  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->setContentsMargins(8, 8, 8, 8);
  setLayout(vbox);

  // block selector
  QComboBox* blockCombo = new QComboBox();
  vbox->addWidget(blockCombo);
  for (size_t i = 0; i < m_level->getBlockCount(); i++) {
    const QString paletteName = QString("Block %1").arg(i);
    blockCombo->addItem(paletteName, QVariant::fromValue(i));
  }

  // create widget to display pixmap
  m_label = new QLabel();
  m_label->setFixedSize(Block::BLOCK_WIDTH, Block::BLOCK_HEIGHT);
  m_label->setMinimumWidth(Block::BLOCK_WIDTH);
  vbox->addWidget(m_label);

  // create pixmap
  m_pixmap = new QPixmap(Block::BLOCK_WIDTH, Block::BLOCK_HEIGHT);
  m_label->setPixmap(*m_pixmap);
  drawBlock(0);

  // handle switching blocks
  connect(blockCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(blockChanged(int)));
}

void BlockInspector::drawPattern(QImage& image,
                                 const Pattern& pattern,
                                 const Palette& palette,
                                 int dx,
                                 int dy,
                                 bool hFlip,
                                 bool vFlip)
{
  for (int py = 0; py < Pattern::PATTERN_HEIGHT; py++) {
    for (int px = 0; px < Pattern::PATTERN_WIDTH; px++) {
      const auto fx = hFlip ? 7 - px : px;
      const auto fy = vFlip ? 7 - py : py;

      const auto idx = pattern.getPixel(fx, fy);
      const auto color = palette.getColor(idx);

      image.setPixel(dx + px, dy + py, qRgb(color.r, color.g, color.b));
    }
  }
}

void BlockInspector::drawChunk(QImage& image, const Chunk& chunk, int dx, int dy, bool hFlip, bool vFlip)
{
  for (int py = 0; py < 2; py++) {
    for (int px = 0; px < 2; px++) {
      const auto& patternDesc = chunk.getPatternDesc(hFlip ? 1 - px : px, vFlip ? 1 - py : py);

      const auto paletteIndex = patternDesc.getPaletteIndex();
      const auto patternIndex = patternDesc.getPatternIndex();

      const auto& pattern = m_level->getPattern(patternIndex);
      const auto& palette = m_level->getPalette(paletteIndex);

      drawPattern(image,
                  pattern,
                  palette,
                  dx + px * Pattern::PATTERN_WIDTH,
                  dy + py * Pattern::PATTERN_HEIGHT,
                  patternDesc.getHFlip() ^ hFlip,
                  patternDesc.getVFlip() ^ vFlip);
    }
  }
}

void BlockInspector::drawBlock(size_t index)
{
  LOG << "Drawing block " << index;

  const Block& block = m_level->getBlock(index);

  QImage image(Block::BLOCK_WIDTH, Block::BLOCK_HEIGHT, QImage::Format_RGB888);
  image.fill(0);

  for (int dy = 0; dy < 8; dy++) {
    for (int dx = 0; dx < 8; dx++) {
      const auto& chunkDesc = block.getChunkDesc(dx, dy);
      const auto chunkIndex = chunkDesc.getChunkIndex();
      try {
        const auto& chunk = m_level->getChunk(chunkIndex);
        drawChunk(image, chunk, dx * 16, dy * 16, chunkDesc.getHFlip(), chunkDesc.getVFlip());
      } catch (const exception& e) {
        LOG << "Failed to draw chunk " << chunkIndex << ": " << e.what();
      }
    }
  }

  // copy to pixmap
  LOG << "Copying block image to pixmap";
  if (m_pixmap->convertFromImage(image)) {
    m_label->setPixmap(*m_pixmap);
  } else {
    LOG << "Failed to copy image to pixmap";
  }
}

void BlockInspector::blockChanged(int index)
{
  drawBlock(index);
}
