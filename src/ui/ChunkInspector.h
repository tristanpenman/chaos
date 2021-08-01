#pragma once

#include <memory>

#include <QDialog>

class QImage;
class QLabel;
class QPixmap;

class Chunk;
class Level;
class Palette;
class Pattern;

class ChunkInspector : public QDialog
{
  Q_OBJECT

public:
  ChunkInspector(QWidget *parent, std::shared_ptr<Level>& level);

private:
  void drawPattern(QImage&, const Pattern&, const Palette&, int dx, int dy, bool hFlip, bool vFlip);
  void drawChunk(QImage&, const Chunk&, int dx, int dy);
  void drawChunks();

  std::shared_ptr<Level> m_level;

  QLabel* m_label;
  QPixmap* m_pixmap;
};
