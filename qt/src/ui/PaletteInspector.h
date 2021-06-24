#pragma once

#include <memory>

#include <QDialog>

class Level;

class PaletteInspector : public QDialog
{
  Q_OBJECT

public:
  PaletteInspector(QWidget *parent, std::shared_ptr<Level>& level);

private:
  std::shared_ptr<Level> m_level;
};
