#pragma once

#include <memory>

#include <QDialog>

class QImage;
class QLabel;
class QPixmap;

class Block;
class Chunk;
class Level;
class Palette;
class Pattern;

class BlockInspector : public QDialog
{
  Q_OBJECT

public:
  BlockInspector(QWidget *parent, std::shared_ptr<Level>& level);

private:
  void drawPattern(QImage&, const Pattern&, const Palette&, int dx, int dy, bool hFlip, bool vFlip);
  void drawChunk(QImage&, const Chunk&, int dx, int dy, bool hFlip, bool vFlip);
  void drawBlock(size_t index);

  std::shared_ptr<Level> m_level;

  QLabel* m_label;
  QPixmap* m_pixmap;

private slots:
  void blockChanged(int);
};
