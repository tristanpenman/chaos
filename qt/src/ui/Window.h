#pragma once

#include <iosfwd>
#include <memory>

#include <QMainWindow>

class ChunkInspector;
class Game;
class Level;
class LevelSelect;
class PaletteInspector;
class PatternInspector;
class Rom;

class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window(bool debug);

  bool openRom(const QString& path);
  void openLevel(const QString& level);

public slots:
  void showLevelSelect();
  void levelSelected(int levelIdx);

private slots:
  void showOpenRomDialog();
  void showPaletteInspector();
  void showPatternInspector();
  void showChunkInspector();

private:
  // menus
  void createFileMenu();
  void createViewMenu();

  // helpers
  void showError(const QString& title, const QString& text);

  // dialogs
  LevelSelect* m_levelSelect;
  PaletteInspector* m_paletteInspector;
  PatternInspector* m_patternInspector;
  ChunkInspector* m_chunkInspector;

  // menus
  QAction* m_levelSelectAction;
  QMenu *m_inspectorsMenu;

  bool m_debug;

  std::shared_ptr<Rom> m_rom;
  std::shared_ptr<Game> m_game;
  std::shared_ptr<Level> m_level;
};
