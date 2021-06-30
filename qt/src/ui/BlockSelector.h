#pragma once

#include <memory>

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;

class Level;

class BlockSelector : public QWidget
{
  Q_OBJECT

public:
  BlockSelector(QWidget *parent, QPixmap** blocks, size_t blockCount);

private:
  QGraphicsScene* m_scene;
  QGraphicsView* m_view;
  QPixmap** m_blocks;
  size_t m_blockCount;
};
