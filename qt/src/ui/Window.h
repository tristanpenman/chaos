#pragma once

#include <iosfwd>
#include <memory>

#include <QMainWindow>

class QVBoxLayout;

class BlockInspector;
class ChunkInspector;
class Game;
class Level;
class LevelSelect;
class MapEditor;
class PaletteInspector;
class PatternInspector;
class Rom;

class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window();

  bool openRom(const QString& path);
  void openLevel(const QString& level);

public slots:
  void levelSelect();
  void levelSelected(int levelIdx);

private slots:
  void showOpenRomDialog();
  void showPaletteInspector();
  void showPatternInspector();
  void showChunkInspector();
  void showBlockInspector();

private:
  void createFileMenu();
  void createViewMenu();
  void showError(const QString& title, const QString& text);

  // dialogs
  LevelSelect* m_levelSelect;
  PaletteInspector* m_paletteInspector;
  PatternInspector* m_patternInspector;
  ChunkInspector* m_chunkInspector;
  BlockInspector* m_blockInspector;

  // editor
  MapEditor* m_mapEditor;

  // menus
  QAction* m_levelSelectAction;
  QMenu *m_inspectorsMenu;

  // layouts
  QVBoxLayout *m_vbox;

  std::shared_ptr<Rom> m_rom;
  std::shared_ptr<Game> m_game;
  std::shared_ptr<Level> m_level;
};
