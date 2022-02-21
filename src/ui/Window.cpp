#include <fstream>
#include <iostream>

#include <QAction>
#include <QFileDialog>
#include <QGuiApplication>
#include <QLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QStatusBar>

#include "../Game.h"
#include "../GameFactory.h"
#include "../Level.h"
#include "../Logger.h"
#include "../Map.h"
#include "../Rom.h"

#include "BlockInspector.h"
#include "ChunkInspector.h"
#include "LevelSelect.h"
#include "MapEditor.h"
#include "PaletteInspector.h"
#include "PatternInspector.h"
#include "RomInfo.h"

#include "Window.h"

#define LOG Logger("Window")

using namespace std;

Window::Window()
  : QMainWindow(nullptr)
  , m_levelSelect(nullptr)
  , m_paletteInspector(nullptr)
  , m_patternInspector(nullptr)
  , m_chunkInspector(nullptr)
  , m_blockInspector(nullptr)
  , m_romInfo(nullptr)
  , m_mapEditor(nullptr)
  , m_levelSelectAction(nullptr)
  , m_exportMapAction(nullptr)
  , m_undoAction(nullptr)
  , m_redoAction(nullptr)
  , m_actualSizeAction(nullptr)
  , m_zoomInAction(nullptr)
  , m_zoomOutAction(nullptr)
  , m_inspectorsMenu(nullptr)
  , m_statusBar(nullptr)
  , m_rom(nullptr)
  , m_game(nullptr)
  , m_level(nullptr)
  , m_levelIdx(0)
{
  setWindowTitle("Chaos");
  setMinimumSize(320, 240);
  setAttribute(Qt::WA_AcceptTouchEvents, false);

  // choose a nice default width and height, and center the window
  const auto geometry = QGuiApplication::primaryScreen()->geometry();
  const int width = geometry.height() * 0.75;
  const int height = geometry.height() * 0.5;
  setGeometry(0, 0, width, height);
  move(geometry.center() - rect().center());

  // menus
  createFileMenu();
  createEditMenu();
  createViewMenu();
  createToolsMenu();

  // statusbar
  m_statusBar = new QStatusBar(this);
  m_statusBar->showMessage(tr("Ready"));
  setStatusBar(m_statusBar);

  // open rom button
  const auto openRomButton = new QPushButton(tr("Open ROM..."));
  openRomButton->setMaximumWidth(250);
  connect(openRomButton, SIGNAL(clicked()), this, SLOT(showOpenRomDialog()));

  // level select button
  m_levelSelectButton = new QPushButton(tr("Level Select..."));
  m_levelSelectButton->setMaximumWidth(250);
  m_levelSelectButton->setDisabled(true);
  connect(m_levelSelectButton, SIGNAL(clicked()), this, SLOT(showLevelSelectDialog()));

  const auto buttonLayout = new QVBoxLayout();
  buttonLayout->addWidget(openRomButton);
  buttonLayout->addWidget(m_levelSelectButton);

  const auto buttonsWidget = new QWidget();
  buttonsWidget->setLayout(buttonLayout);
  setCentralWidget(buttonsWidget);

  buttonLayout->setAlignment(Qt::AlignHCenter);
}

bool Window::openRom(const QString &path)
{
  m_rom = make_shared<Rom>();
  if (!m_rom->open(path.toStdString())) {
    showError(tr("ROM Error"), tr("Failed to open ROM file"));
    m_rom.reset();
    return false;
  }

  m_game = GameFactory::build(m_rom);
  if (!m_game) {
    showError(tr("ROM Error"), tr("Failed to identify game"));
    m_rom.reset();
    return false;
  }

  LOG << "ROM identified";
  LOG << "Domestic name: '" << m_rom->readDomesticName() << "'";

  m_levelSelectAction->setEnabled(true);
  m_levelSelectButton->setEnabled(true);
  m_relocateLevelsAction->setEnabled(m_game->canRelocateLevels());

  if (m_romInfo) {
    delete m_romInfo;
    m_romInfo = nullptr;
  }

  return true;
}

void Window::openLevel(const QString& level)
{
  bool parsed = false;
  const unsigned int levelIdx = level.toUInt(&parsed);
  if (parsed) {
    levelSelected(levelIdx);
  } else {
    showError(tr("Level Error"), tr("Failed to parse level index"));
  }
}

void Window::exportMap(const QString& fileName)
{
  fstream file(fileName.toStdString(), ios::out | ios::binary);
  if (file.bad()) {
    showError(tr("Export Map"), tr("Failed to open file"));
    return;
  }

  auto& map = m_level->getMap();

  for (auto y = 0; y < map.getHeight(); y++) {
    for (auto x = 0; x < map.getWidth(); x++) {
      for (auto l = 0; l < 2; l++) {
        auto value = map.getValue(l, x, y);
        file.put(static_cast<char>(value));
      }
    }
  }

  showInfo(tr("Export Map"), tr("Map exported successfully."));
}

void Window::showOpenRomDialog()
{
  if (m_level) {
    const QMessageBox::StandardButton reply = QMessageBox::question(this,
          tr("Close Level"),
          tr("Are you sure you want to close the current level?"),
          QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
      return;
    }
  }

  const QString fileName = QFileDialog::getOpenFileName(this, tr("Open ROM"), QString(), QString("*.bin"));
  if (!fileName.isEmpty()) {
    m_level.reset();

    if (openRom(fileName)) {
      showLevelSelectDialog();
    }
  }
}

void Window::showLevelSelectDialog()
{
  if (m_level) {
    const QMessageBox::StandardButton reply = QMessageBox::question(this,
          tr("Close Level"),
          tr("Are you sure you want to close the current level?"),
          QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
      return;
    }
  }

  if (m_levelSelect) {
    delete m_levelSelect;
    m_levelSelect = nullptr;
  }

  m_levelSelect = new LevelSelect(this, m_game);
  connect(m_levelSelect, SIGNAL(levelSelected(int)), this, SLOT(levelSelected(int)));

  m_levelSelect->show();
}

void Window::saveRom()
{
  try {
    if (m_game->save(m_levelIdx, *m_level)) {
      QMessageBox msgBox;
      msgBox.information(this,
          tr("Save ROM"),
          tr("Level saved successfully."),
          QMessageBox::StandardButton::Ok);
    } else {
      QMessageBox msgBox;
      msgBox.warning(this,
          tr("Save ROM"),
          tr("There was not enough space to save this level. You may need to relocate the levels in this ROM."),
          QMessageBox::StandardButton::Ok);
    }
  } catch (const exception& e) {
    // show other error occurred
    QMessageBox msgBox;
    msgBox.warning(this,
        tr("Save ROM"),
        tr("Something went wrong while saving this level: ") + e.what(),
        QMessageBox::StandardButton::Ok);
  }
}

void Window::showExportMapDialog()
{
  if (!m_level) {
    return;
  }

  const auto fileName = QFileDialog::getSaveFileName(this, tr("Export Map"), QString(), QString("*.bin"));
  if (fileName.isEmpty()) {
    return;
  }

  exportMap(fileName);
}

void Window::undo()
{
  m_mapEditor->undo();
}

void Window::redo()
{
  m_mapEditor->redo();
}

void Window::actualSize()
{
  if (!m_mapEditor) {
    return;
  }

  m_mapEditor->actualSize();
}

void Window::zoomIn()
{
  if (!m_mapEditor) {
    return;
  }

  m_mapEditor->zoomIn();
}

void Window::zoomOut()
{
  if (!m_mapEditor) {
    return;
  }

  m_mapEditor->zoomOut();
}

void Window::showPaletteInspector()
{
  if (!m_paletteInspector) {
    m_paletteInspector = new PaletteInspector(this, m_level);
  }

  m_paletteInspector->show();
}

void Window::showPatternInspector()
{
  if (!m_patternInspector) {
    m_patternInspector = new PatternInspector(this, m_level);
  }

  m_patternInspector->show();
}

void Window::showChunkInspector()
{
  if (!m_chunkInspector) {
    m_chunkInspector = new ChunkInspector(this, m_level);
  }

  m_chunkInspector->show();
}

void Window::showBlockInspector()
{
  if (!m_blockInspector) {
    m_blockInspector = new BlockInspector(this, m_level);
  }

  m_blockInspector->show();
}

void Window::showRomInfo()
{
  if (!m_romInfo) {
    m_romInfo = new RomInfo(this, *m_rom, *m_game);
  }

  m_romInfo->show();
}

void Window::relocateLevels()
{
  try {
    const QMessageBox::StandardButton confirmation = QMessageBox::question(this,
          tr("Relocate Levels"),
          tr("Relocating levels will permanently modify the structure of this ROM. ") +
                tr("The current level will be closed and any unsaved changes will be lost.\n\n") +
                tr("Are you sure you want to continue?"),
          QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::No) {
      return;
    }

    delete m_mapEditor;
    m_mapEditor = nullptr;
    m_level.reset();

    if (m_game->relocateLevels(false)) {
      showInfo(tr("Relocate Levels"), tr("Levels relocated successfully."));
      return;
    }

    const QMessageBox::StandardButton reply = QMessageBox::question(this,
          tr("Relocate Levels"),
          tr("Levels could not be relocated safely. Would you like to try anyway?"),
          QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
      return;
    }

    if (m_game->relocateLevels(true)) {
      showInfo(tr("Relocate Levels"), tr("Levels relocated successfully (unsafe)."));
    } else {
      showError(tr("Relocate Levels"), tr("Level relocation was attempted but rolled back."));
    }
  } catch (exception& e) {
    showError(tr("Relocate Levels"), tr("Exception while relocating levels: ") + e.what());
  }
}

void Window::levelSelected(int levelIdx)
{
  if (m_level) {
    m_inspectorsMenu->setEnabled(false);

    delete m_paletteInspector;
    m_paletteInspector = nullptr;

    delete m_patternInspector;
    m_patternInspector = nullptr;

    delete m_chunkInspector;
    m_chunkInspector = nullptr;

    delete m_blockInspector;
    m_blockInspector = nullptr;

    delete m_mapEditor;
    m_mapEditor = nullptr;
  }

  m_level.reset();

  if (!m_rom) {
    showError(tr("Level Error"), tr("Cannot load level until ROM has been loaded"));
    return;
  }

  LOG << "Loading level: " << levelIdx << " (" << m_game->getTitleCards()[levelIdx] << ")";

  m_level = m_game->loadLevel(levelIdx);
  if (!m_level) {
    showError(tr("Level Error"), tr("Failed to load level"));
    return;
  }

  m_levelIdx = levelIdx;

  if (m_game->canSave()) {
    m_saveRomAction->setEnabled(true);
  }

  m_exportMapAction->setEnabled(true);
  m_inspectorsMenu->setEnabled(true);
  m_romInfoAction->setEnabled(true);

  m_mapEditor = new MapEditor(this, m_level);
  connect(m_mapEditor, SIGNAL(currentTile(uint16_t,uint16_t,uint8_t)), this, SLOT(currentTile(uint16_t,uint16_t,uint8_t)));
  connect(m_mapEditor, SIGNAL(noTile()), this, SLOT(noTile()));
  connect(m_mapEditor, SIGNAL(undosRedosChanged(size_t,size_t)), this, SLOT(undosRedosChanged(size_t,size_t)));
  this->setCentralWidget(m_mapEditor);
}

void Window::currentTile(uint16_t x, uint16_t y, uint8_t value)
{
  m_statusBar->showMessage(
      QString("[%1, %2]: 0x%3")
          .arg(x)
          .arg(y)
          .arg(QString("%1")
               .arg(value, 1, 16)
               .toUpper()
               .rightJustified(2, '0')));
}

void Window::noTile()
{
  m_statusBar->showMessage(QString());
}

void Window::undosRedosChanged(size_t undos, size_t redos)
{
  LOG << "Undos: " << undos << ", redos: " << redos;

  m_undoAction->setEnabled(undos > 0);
  m_redoAction->setEnabled(redos > 0);
}

void Window::createFileMenu()
{
  // open rom
  m_openRomAction = new QAction(tr("&Open ROM..."));
  m_openRomAction->setShortcuts(QKeySequence::Open);
  connect(m_openRomAction, SIGNAL(triggered()), this, SLOT(showOpenRomDialog()));

  // level select
  m_levelSelectAction = new QAction(tr("&Level Select..."));
  m_levelSelectAction->setDisabled(true);
  connect(m_levelSelectAction, SIGNAL(triggered()), this, SLOT(showLevelSelectDialog()));

  // save rom
  m_saveRomAction = new QAction(tr("&Save ROM"));
  m_saveRomAction->setDisabled(true);
  connect(m_saveRomAction, SIGNAL(triggered()), this, SLOT(saveRom()));

  // export map
  m_exportMapAction = new QAction(tr("Export &Map..."));
  m_exportMapAction->setDisabled(true);
  connect(m_exportMapAction, SIGNAL(triggered()), this, SLOT(showExportMapDialog()));

  // file menu
  auto fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(m_openRomAction);
  fileMenu->addSeparator()->setSeparator(true);
  fileMenu->addAction(m_levelSelectAction);
  fileMenu->addSeparator();
  fileMenu->addAction(m_saveRomAction);
  fileMenu->addAction(m_exportMapAction);
}

void Window::createEditMenu()
{
  auto editMenu = menuBar()->addMenu(tr("&Edit"));

  m_undoAction = new QAction(tr("Undo"));
  m_undoAction->setDisabled(true);
  connect(m_undoAction, SIGNAL(triggered()), this, SLOT(undo()));

  m_redoAction = new QAction(tr("Redo"));
  m_redoAction->setDisabled(true);
  connect(m_redoAction, SIGNAL(triggered()), this, SLOT(redo()));

  editMenu->addAction(m_undoAction);
  editMenu->addAction(m_redoAction);
}

void Window::createViewMenu()
{
  auto viewMenu = menuBar()->addMenu(tr("&View"));

  // wire up inspectors
  auto inspectPalettesAction = new QAction(tr("Palettes"), this);
  connect(inspectPalettesAction, SIGNAL(triggered()), this, SLOT(showPaletteInspector()));
  auto inspectPatternsAction = new QAction(tr("Patterns (8x8)"), this);
  connect(inspectPatternsAction, SIGNAL(triggered()), this, SLOT(showPatternInspector()));
  auto inspectChunksAction = new QAction(tr("Chunks (16x16)"), this);
  connect(inspectChunksAction, SIGNAL(triggered()), this, SLOT(showChunkInspector()));
  auto inspectBlocksAction = new QAction(tr("Blocks (128x128)"), this);
  connect(inspectBlocksAction, SIGNAL(triggered()), this, SLOT(showBlockInspector()));

  // build inspectors sub-menu
  m_inspectorsMenu = viewMenu->addMenu(tr("&Inspectors"));
  m_inspectorsMenu->setDisabled(true);
  m_inspectorsMenu->addAction(inspectPalettesAction);
  m_inspectorsMenu->addSeparator();
  m_inspectorsMenu->addAction(inspectPatternsAction);
  m_inspectorsMenu->addAction(inspectChunksAction);
  m_inspectorsMenu->addAction(inspectBlocksAction);

  // zoom
  m_actualSizeAction = new QAction(tr("Actual Size"), this);
  m_actualSizeAction->setDisabled(true);
  connect(m_actualSizeAction, SIGNAL(triggered()), this, SLOT(actualSize()));
  m_zoomInAction = new QAction(tr("Zoom In"), this);
  m_zoomInAction->setDisabled(true);
  connect(m_zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
  m_zoomOutAction = new QAction(tr("Zoom Out"), this);
  m_zoomOutAction->setDisabled(true);
  connect(m_zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));

  viewMenu->addSeparator();
  viewMenu->addAction(m_actualSizeAction);
  viewMenu->addAction(m_zoomInAction);
  viewMenu->addAction(m_zoomOutAction);
}

void Window::createToolsMenu()
{
  auto toolsMenu = menuBar()->addMenu(tr("&Tools"));

  m_romInfoAction = new QAction(tr("ROM Info..."), this);
  m_romInfoAction->setDisabled(true);
  connect(m_romInfoAction, SIGNAL(triggered()), this, SLOT(showRomInfo()));

  m_relocateLevelsAction = new QAction(tr("Relocate Levels"), this);
  m_relocateLevelsAction->setDisabled(true);
  connect(m_relocateLevelsAction, SIGNAL(triggered()), this, SLOT(relocateLevels()));

  toolsMenu->addAction(m_romInfoAction);
  toolsMenu->addSeparator();
  toolsMenu->addAction(m_relocateLevelsAction);
}

void Window::showError(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}

void Window::showInfo(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}
