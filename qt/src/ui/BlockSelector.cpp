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
  auto* vbox = new QVBoxLayout(this);

  m_scene = new QGraphicsScene(this);
  for (size_t i = 0; i < blockCount; i++) {
    auto* item = m_scene->addPixmap(*blocks[i]);
    item->setPos(0, i * 133 + 5);
  }

  m_view = new QGraphicsView(this);
  m_view->setScene(m_scene);
  m_view->centerOn(0, -m_scene->height() / 2);
  m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  vbox->addWidget(m_view);
  vbox->setContentsMargins(0, 0, 0, 0);

  setLayout(vbox);

  m_view->setFixedWidth(128 + m_view->verticalScrollBar()->height());
}
