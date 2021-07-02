#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollBar>
#include <QVBoxLayout>

#include "BlockSelector.h"

BlockSelector::BlockSelector(QWidget *parent, QPixmap** blocks, size_t blockCount)
  : QWidget(parent)
  , m_blocks(blocks)
  , m_blockCount(blockCount)
{
  // add blocks to scene
  m_scene = new QGraphicsScene(this);
  for (size_t i = 0; i < blockCount; i++) {
    auto* item = m_scene->addPixmap(*blocks[i]);
    item->setPos(0, i * 133 + 5);
  }

  // create view
  m_view = new QGraphicsView(this);
  m_view->setScene(m_scene);

  // no border and dark background
  m_view->setStyleSheet("background: #666");
  m_view->setFrameStyle(QFrame::NoFrame);

  // disable dragging and move to first tile
  m_view->setDragMode(QGraphicsView::DragMode::NoDrag);
  m_view->centerOn(0, -m_scene->height() / 2);

  // set width according to scrollbars... not sure why `height / 2` works
  m_view->setFixedWidth(128 + m_view->verticalScrollBar()->height() / 2);
  m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  // layout
  auto* vbox = new QVBoxLayout(this);
  vbox->addWidget(m_view);
  vbox->setContentsMargins(0, 0, 0, 0);
  setLayout(vbox);
}
