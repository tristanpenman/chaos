#pragma once

#include <iosfwd>
#include <memory>

#include <QMainWindow>

class Game;
class LevelSelect;
class Rom;

class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window(QWidget* parent = 0);

  void setDebug(bool debug);

  bool openRom(const QString& path);

  void openLevel(int levelIdx);
  void openLevel(const QString& level);

public slots:
  // open rom
  void showOpenModelDialog();

  // level select
  void showLevelSelectDialog();
  void levelSelected(int levelIdx);
  void levelSelectFinished(int);

private:
  void showError(const QString& title, const QString& text);

  bool m_debug;

  std::shared_ptr<Rom> m_rom;
  std::shared_ptr<Game> m_game;

  LevelSelect* m_levelSelect;
};
