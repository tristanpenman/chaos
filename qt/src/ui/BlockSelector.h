#pragma once

#include <memory>

#include <QWidget>

class QGraphicsPixmapItem;
class QGraphicsScene;
class QGraphicsView;
class QLabel;

class Level;
class Rectangle;

class BlockSelector : public QWidget
{
  Q_OBJECT

public:
  BlockSelector(QWidget *parent, QPixmap** blocks, size_t blockCount);

protected:
  bool eventFilter(QObject *object, QEvent *ev) override;

private:
  void handleClick(const QPoint& pos);
  void handleMove(const QPoint& pos);

  QGraphicsScene* m_scene;
  QGraphicsView* m_view;
  QLabel* m_selected;
  QPixmap** m_blocks;
  QGraphicsPixmapItem** m_blockItems;

  Rectangle* m_highlight;

  size_t m_blockCount;

  int m_selectedBlock;
  int m_highlightedBlock;

signals:
  void blockSelected(int);
};
