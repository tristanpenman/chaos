#pragma once

#include <memory>

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class QPixmap;

class Chunk;
class Level;
class Palette;
class Pattern;

class MapEditor : public QWidget
{
  Q_OBJECT

public:
  MapEditor(QWidget *parent, std::shared_ptr<Level>&);

private:
  void drawPattern(QImage&, const Pattern&, const Palette&, int dx, int dy, bool hFlip, bool vFlip);
  void drawChunk(QImage&, const Chunk&, int dx, int dy, bool hFlip, bool vFlip);
  void drawBlock(QPixmap&, size_t index);

  std::shared_ptr<Level> m_level;

  QGraphicsScene* m_scene;
  QGraphicsView* m_view;
  QPixmap** m_blocks;
};
