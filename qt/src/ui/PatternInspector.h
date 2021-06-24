#pragma once

#include <memory>

#include <QDialog>

class Level;

class PatternInspector : public QDialog
{
  Q_OBJECT

public:
  PatternInspector(QWidget *parent, std::shared_ptr<Level>& level);

private:
  std::shared_ptr<Level> m_level;
};
