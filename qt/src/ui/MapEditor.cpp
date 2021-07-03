#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QImage>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollBar>

#include "../Block.h"
#include "../Chunk.h"
#include "../Level.h"
#include "../Logger.h"
#include "../Map.h"
#include "../Palette.h"
#include "../Pattern.h"

#include "BlockSelector.h"
#include "Rectangle.h"
#include "ZoomSupport.h"

#include "MapEditor.h"

#define LOG Logger("MapEditor")

using namespace std;

MapEditor::MapEditor(QWidget *parent, shared_ptr<Level>& level)
  : QWidget(parent)
  , m_level(level)
  , m_highlightX(-1)
  , m_highlightY(-1)
  , m_selectedBlock(0)
{
  setStyleSheet("background: #ccc");

  // layout
  QHBoxLayout* hbox = new QHBoxLayout(this);
  hbox->setContentsMargins(8, 8, 8, 8);
  hbox->setSpacing(8);
  setLayout(hbox);

  // render block artwork into pixmaps
  LOG << "Drawing blocks";
  const size_t blockCount = m_level->getBlockCount();
  m_blocks = new QPixmap*[blockCount];
  for (size_t i = 0; i < blockCount; i++) {
    m_blocks[i] = new QPixmap();
    drawBlock(*m_blocks[i], i);
  }

  // populate scene
  const auto& map = m_level->getMap();
  m_tiles = new QGraphicsPixmapItem*[map.getWidth() * map.getHeight()];
  m_scene = new QGraphicsScene(this);
  for (int y = 0; y < map.getHeight(); y++) {
    for (int x = 0; x < map.getWidth(); x++) {
      auto& tile = m_tiles[y * map.getWidth() + x];
      tile = m_scene->addPixmap(*m_blocks[map.getValue(0, x, y)]);
      tile->setTransformationMode(Qt::SmoothTransformation);
      tile->setPos(x * 128, y * 128);
    }
  }

  // setup scene view
  m_view = new QGraphicsView(this);
  m_view->setScene(m_scene);
  m_view->setFrameStyle(QFrame::NoFrame);
  m_view->centerOn(-m_scene->width() / 2, -m_scene->height() / 2);
  m_view->setDragMode(QGraphicsView::DragMode::NoDrag);
  hbox->addWidget(m_view);

  // highlight region
  m_highlight = new Rectangle(128, 128, QColor(128, 192, 255, 64));
  m_highlight->setPos(0, 0);
  m_highlight->setVisible(false);
  m_scene->addItem(m_highlight);

  // track mouse events
  m_view->viewport()->installEventFilter(this);
  m_view->setMouseTracking(true);

  // zoom support
  new ZoomSupport(m_view);

  // selector
  auto blockSelector = new BlockSelector(this, m_blocks, blockCount);
  hbox->addWidget(blockSelector);
  connect(blockSelector, SIGNAL(blockSelected(int)), this, SLOT(blockSelected(int)));

  // allow map to grow but block selector remains the same size
  hbox->setStretch(0, 1);
  hbox->setStretch(1, 0);
}

bool MapEditor::eventFilter(QObject *object, QEvent *ev)
{
  if (object != m_view->viewport()) {
    return false;
  }

  switch (ev->type()) {
  case QEvent::Leave:
    m_highlightX = -1;
    m_highlightY = -1;
    m_highlight->setVisible(false);
    break;

  case QEvent::MouseButtonPress:
    return handleClick();

  case QEvent::MouseMove:
    {
      auto mouseEvent = dynamic_cast<QMouseEvent*>(ev);
      handleMove(m_view->mapToScene(mouseEvent->pos()));
      break;
    }

  default:
    break;
  }

  return false;
}

bool MapEditor::handleClick()
{
  if (m_highlightX < 0 || m_highlightY < 0) {
    return false;
  }

  // update scene
  const auto offset = m_highlightY * m_level->getMap().getWidth() + m_highlightX;
  m_tiles[offset]->setPixmap(*m_blocks[m_selectedBlock]);

  // modify map
  m_level->getMap().setValue(0, m_highlightX, m_highlightY, m_selectedBlock);

  return true;
}

void MapEditor::handleMove(const QPointF& pos)
{
  m_highlightX = (pos.x() / 128);
  m_highlightY = (pos.y() / 128);
  m_highlight->setPos(m_highlightX * 128, m_highlightY * 128);
  m_highlight->setVisible(true);
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
        LOG << "Failed to draw chunk: " << e.what();
      }
    }
  }

  if (!pixmap.convertFromImage(image)) {
    throw runtime_error("Failed to copy image to pixmap");
  }
}

void MapEditor::blockSelected(int blockIdx)
{
  m_selectedBlock = blockIdx;
}
