#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMouseEvent>
#include <QScrollBar>
#include <QScrollEvent>
#include <QVBoxLayout>

#include "../Logger.h"

#include "Rectangle.h"

#include "BlockSelector.h"

#define LOG Logger("BlockSelector")

static constexpr int BLOCK_SPACING = 5;

BlockSelector::BlockSelector(QWidget *parent, QPixmap** blocks, size_t blockCount)
  : QWidget(parent)
  , m_scene(nullptr)
  , m_view(nullptr)
  , m_selected(nullptr)
  , m_blocks(blocks)
  , m_blockItems(nullptr)
  , m_blockCount(blockCount)
  , m_selectedBlock(0)
  , m_highlightedBlock(-1)
{
  // add blocks to scene
  m_scene = new QGraphicsScene(this);
  m_blockItems = new QGraphicsPixmapItem*[blockCount];
  for (size_t i = 0; i < blockCount; i++) {
    m_blockItems[i] = m_scene->addPixmap(*blocks[i]);
    m_blockItems[i]->setPos(0, i * (128 + BLOCK_SPACING));
  }

  // create view
  m_view = new QGraphicsView(this);
  m_view->setScene(m_scene);

  // no border and light background
  m_view->setFrameStyle(QFrame::NoFrame);
  m_view->setStyleSheet("background: #eee");

  // disable dragging and move to first tile
  m_view->setDragMode(QGraphicsView::DragMode::NoDrag);
  m_view->centerOn(0, -m_scene->height() / 2);

  // set width according to scrollbars...
  // TODO: not sure why height/2 works here, need to test across platforms
  const auto scrollbarSize = m_view->verticalScrollBar()->height() / 2;
  m_view->setFixedWidth(128 + scrollbarSize);
  m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  // styleable container for selected block
  auto selectedContainer = new QWidget(this);
  auto selectedLayout = new QVBoxLayout(this);
  // use half-scrollbar size so that it is as wide as the scrollable view
  const auto halfScrollbarSize = scrollbarSize / 2;
  selectedLayout->setContentsMargins(halfScrollbarSize, halfScrollbarSize, halfScrollbarSize, halfScrollbarSize);
  selectedContainer->setLayout(selectedLayout);
  selectedContainer->setStyleSheet("background: #ccc");

  // selected block
  m_selected = new QLabel(this);
  m_selected->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_selected->setAttribute(Qt::WA_TranslucentBackground);
  m_selected->setPixmap(*m_blocks[0]);
  selectedLayout->addWidget(m_selected);

  // layout
  auto vbox = new QVBoxLayout(this);
  vbox->addWidget(selectedContainer);
  vbox->addWidget(m_view);
  vbox->setContentsMargins(0, 0, 0, 0);
  vbox->setSpacing(8);
  setLayout(vbox);

  // highlight region
  m_highlight = new Rectangle(128, 128, QColor(128, 192, 255, 64));
  m_highlight->setPos(0, 0);
  m_highlight->setVisible(false);
  m_scene->addItem(m_highlight);

  // track mouse events
  m_view->viewport()->installEventFilter(this);
  m_view->setMouseTracking(true);
}

bool BlockSelector::eventFilter(QObject *object, QEvent *ev)
{
  if (object != m_view->viewport()) {
    return false;
  }

  switch (ev->type()) {
  case QEvent::Leave:
    m_highlightedBlock = -1;
    m_highlight->setVisible(false);
    break;

  case QEvent::MouseButtonPress:
    handleClick(m_view->viewport()->mapFromGlobal(QCursor::pos()));
    return true;

  case QEvent::MouseMove:
  case QEvent::Wheel:
    handleMove(m_view->viewport()->mapFromGlobal(QCursor::pos()));
    break;

  default:
    break;
  }

  return false;
}

void BlockSelector::handleClick(const QPoint& pos)
{
  const int y = pos.y() - BLOCK_SPACING + m_view->verticalScrollBar()->value();
  const int tileOffset = y % (128 + BLOCK_SPACING);

  if (tileOffset < 128) {
    const int tile = y / (128 + BLOCK_SPACING);
    LOG << "Selected block " << tile;
    m_selectedBlock = tile;
    m_selected->setPixmap(*m_blocks[tile]);
    emit blockSelected(tile);
  }
}

void BlockSelector::handleMove(const QPoint &pos)
{
  const int y = pos.y() - BLOCK_SPACING + m_view->verticalScrollBar()->value();
  const int tileOffset = y % (128 + BLOCK_SPACING);
  if (tileOffset >= 128) {
    m_highlightedBlock = -1;
    m_highlight->setVisible(false);
    return;
  }

  const int tile = y / (128 + BLOCK_SPACING);
  if (m_highlightedBlock != tile) {
    m_highlightedBlock = tile;
    m_highlight->setPos(0, tile * (128 + BLOCK_SPACING));
    m_highlight->setVisible(true);
  }
}
