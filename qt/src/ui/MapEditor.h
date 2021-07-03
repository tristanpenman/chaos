#pragma once

#include <memory>

#include <QWidget>

class QGraphicsPixmapItem;
class QGraphicsScene;
class QGraphicsView;
class QPixmap;

class Chunk;
class Level;
class Palette;
class Pattern;
class Rectangle;

class MapEditor : public QWidget
{
  Q_OBJECT

public:
  MapEditor(QWidget *parent, std::shared_ptr<Level>&);

protected:
  bool eventFilter(QObject *object, QEvent *ev) override;

private:
  bool handleClick();
  void handleMove(const QPointF& pos);

  void drawPattern(QImage&, const Pattern&, const Palette&, int dx, int dy, bool hFlip, bool vFlip);
  void drawChunk(QImage&, const Chunk&, int dx, int dy, bool hFlip, bool vFlip);
  void drawBlock(QPixmap&, size_t index);

  std::shared_ptr<Level> m_level;

  QGraphicsScene* m_scene;
  QGraphicsView* m_view;
  QGraphicsPixmapItem** m_tiles;
  QPixmap** m_blocks;
  Rectangle* m_highlight;

  int m_highlightX;
  int m_highlightY;

  size_t m_selectedBlock;

private slots:
  void blockSelected(int);
};
