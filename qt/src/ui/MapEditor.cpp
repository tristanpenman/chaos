#include <iostream>

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>

#include "MapEditor.h"

#include "../Block.h"
#include "../Chunk.h"
#include "../Level.h"
#include "../Map.h"
#include "../Palette.h"
#include "../Pattern.h"

MapEditor::MapEditor(QWidget *parent, std::shared_ptr<Level>& level)
  : QWidget(parent)
  , m_level(level)
{
  // layout
  QHBoxLayout* hbox = new QHBoxLayout();
  hbox->setContentsMargins(0, 0, 0, 0);
  setLayout(hbox);

  // render block artwork into pixmaps
  m_blocks = new QPixmap*[m_level->getBlockCount()];
  for (size_t i = 0; i < m_level->getBlockCount(); i++) {
    m_blocks[i] = new QPixmap();
    drawBlock(*m_blocks[i], i);
  }

  // populate scene
  m_scene = new QGraphicsScene();
  const auto& map = m_level->getMap();
  for (int y = 0; y < map.getHeight(); y++) {
    for (int x = 0; x < map.getWidth(); x++) {
      auto item = m_scene->addPixmap(*m_blocks[map.getValue(0, x, y)]);
      item->setTransformationMode(Qt::SmoothTransformation);
      item->setPos(x * 128, y * 128);
    }
  }

  // setup scene view
  m_view = new QGraphicsView(this);
  m_view->setScene(m_scene);
  m_view->setFrameStyle(QFrame::NoFrame);
  hbox->addWidget(m_view);
}

void MapEditor::drawPattern(QImage& image,
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

void MapEditor::drawChunk(QImage& image, const Chunk& chunk, int dx, int dy, bool hFlip, bool vFlip)
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

void MapEditor::drawBlock(QPixmap& pixmap, size_t index)
{
  std::cout << "[BlockInspector] Drawing block " << index << std::endl;

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
      } catch (const std::exception& e) {
        std::cout << "[BlockInspector] Failed to draw chunk " << chunkIndex << ": " << e.what() << std::endl;
      }
    }
  }

  if (!pixmap.convertFromImage(image)) {
    throw std::runtime_error("Failed to copy image to pixmap");
  }
}
