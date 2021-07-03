#include <cmath>
#include <iostream>

#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "../Chunk.h"
#include "../Level.h"
#include "../Logger.h"
#include "../Palette.h"
#include "../Pattern.h"

#include "ChunkInspector.h"

#define LOG Logger("ChunkInspector")

using namespace std;

static constexpr int PIXMAP_WIDTH = 320;
static constexpr int CHUNKS_PER_ROW = PIXMAP_WIDTH / Chunk::CHUNK_WIDTH;

ChunkInspector::ChunkInspector(QWidget* parent, shared_ptr<Level>& level)
  : QDialog(parent)
  , m_level(level)
  , m_pixmap(nullptr)
{
  const auto chunkCount = level->getChunkCount();
  const int pixmapHeight = ceilf(static_cast<float>(chunkCount) / CHUNKS_PER_ROW) * Chunk::CHUNK_HEIGHT;

  // main layout
  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->setContentsMargins(8, 8, 8, 8);
  vbox->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(vbox);

  // create widget to display pixmap
  m_label = new QLabel();
  m_label->setFixedSize(PIXMAP_WIDTH, pixmapHeight);
  m_label->setMinimumWidth(PIXMAP_WIDTH);
  vbox->addWidget(m_label);

  // create pixmap
  m_pixmap = new QPixmap(PIXMAP_WIDTH, pixmapHeight);
  m_label->setPixmap(*m_pixmap);
  drawChunks();
}

void ChunkInspector::drawPattern(QImage& image,
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

void ChunkInspector::drawChunk(QImage& image, const Chunk& chunk, int dx, int dy)
{
  for (int py = 0; py < 2; py++) {
    for (int px = 0; px < 2; px++) {
      const auto& patternDesc = chunk.getPatternDesc(px, py);

      const auto paletteIndex = patternDesc.getPaletteIndex();
      const auto patternIndex = patternDesc.getPatternIndex();

      const auto& pattern = m_level->getPattern(patternIndex);
      const auto& palette = m_level->getPalette(paletteIndex);

      drawPattern(image,
                  pattern,
                  palette,
                  dx + px * Pattern::PATTERN_WIDTH,
                  dy + py * Pattern::PATTERN_HEIGHT,
                  patternDesc.getHFlip(),
                  patternDesc.getVFlip());
    }
  }
}

void ChunkInspector::drawChunks()
{
  LOG << "Drawing chunks";

  // image to draw to
  QImage image(m_pixmap->width(), m_pixmap->height(), QImage::Format_RGB888);
  image.fill(qRgb(0, 0, 0));

  // draw individual chunks
  for (size_t i = 0; i < m_level->getChunkCount(); i++) {
    const auto row = static_cast<int>(i / CHUNKS_PER_ROW);
    const auto col = static_cast<int>(i % CHUNKS_PER_ROW);

    drawChunk(image, m_level->getChunk(i), col * Chunk::CHUNK_WIDTH, row * Chunk::CHUNK_HEIGHT);
  }

  // copy to pixmap
  LOG << "Copying pattern image to pixmap";
  if (m_pixmap->convertFromImage(image)) {
    m_label->setPixmap(*m_pixmap);
  } else {
    LOG << "Failed to copy image to pixmap";
  }
}
