#pragma once

#include <memory>

#include <QDialog>

class Game;
class Rom;

class RomInfo : public QDialog
{
  Q_OBJECT

public:
  RomInfo(QWidget *parent, Rom& rom, Game& game);
};
