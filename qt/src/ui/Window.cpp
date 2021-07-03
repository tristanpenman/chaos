#include <fstream>
#include <iostream>

#include <QAction>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>

#include "../Game.h"
#include "../GameFactory.h"
#include "../Logger.h"
#include "../Rom.h"

#include "BlockInspector.h"
#include "ChunkInspector.h"
#include "LevelSelect.h"
#include "MapEditor.h"
#include "PaletteInspector.h"
#include "PatternInspector.h"

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
  , m_mapEditor(nullptr)
  , m_levelSelectAction(nullptr)
  , m_inspectorsMenu(nullptr)
  , m_vbox(nullptr)
  , m_rom(nullptr)
  , m_game(nullptr)
  , m_level(nullptr)
{
  setWindowTitle("Chaos");
  setMinimumSize(320, 240);
  setAttribute(Qt::WA_AcceptTouchEvents, false);

  // choose a nice default width and height, and center the window
  QRect geometry = QGuiApplication::primaryScreen()->geometry();
  const int width = geometry.height() * 0.75;
  const int height = geometry.height() * 0.5;
  setGeometry(0, 0, width, height);
  move(geometry.center() - rect().center());

  createFileMenu();
  createViewMenu();
}

void Window::createFileMenu()
{
  // write up Open ROM action
  QAction* openRomAction = new QAction(tr("&Open ROM..."));
  openRomAction->setShortcuts(QKeySequence::Open);
  connect(openRomAction, SIGNAL(triggered()), this, SLOT(showOpenRomDialog()));

  // track Level Select action so we can enable it later
  m_levelSelectAction = new QAction(tr("&Level Select..."));
  m_levelSelectAction->setDisabled(true);
  connect(m_levelSelectAction, SIGNAL(triggered()), this, SLOT(levelSelect()));

  // build file menu
  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openRomAction);
  fileMenu->addSeparator();
  fileMenu->addAction(m_levelSelectAction);
}

void Window::createViewMenu()
{
  QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

  // wire up inspectors
  QAction* inspectPalettesAction = new QAction(tr("Palettes"), this);
  connect(inspectPalettesAction, SIGNAL(triggered()), this, SLOT(showPaletteInspector()));
  QAction* inspectPatternsAction = new QAction(tr("Patterns (8x8)"), this);
  connect(inspectPatternsAction, SIGNAL(triggered()), this, SLOT(showPatternInspector()));
  QAction* inspectChunksAction = new QAction(tr("Chunks (16x16)"), this);
  connect(inspectChunksAction, SIGNAL(triggered()), this, SLOT(showChunkInspector()));
  QAction* inspectBlocksAction = new QAction(tr("Blocks (128x128)"), this);
  connect(inspectBlocksAction, SIGNAL(triggered()), this, SLOT(showBlockInspector()));

  // build inspectors sub-menu
  m_inspectorsMenu = viewMenu->addMenu(tr("&Inspectors"));
  m_inspectorsMenu->setDisabled(true);
  m_inspectorsMenu->addAction(inspectPalettesAction);
  m_inspectorsMenu->addSeparator();
  m_inspectorsMenu->addAction(inspectPatternsAction);
  m_inspectorsMenu->addAction(inspectChunksAction);
  m_inspectorsMenu->addAction(inspectBlocksAction);
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

  m_levelSelectAction->setDisabled(false);

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

void Window::showOpenRomDialog()
{
  const QString fileName = QFileDialog::getOpenFileName(this, tr("Open ROM"), QString(), QString("*.bin"));
  if (!fileName.isEmpty()) {
    if (openRom(fileName)) {
      levelSelect();
    }
  }
}

void Window::levelSelect()
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

  if (m_level) {
    m_inspectorsMenu->setEnabled(true);
    m_mapEditor = new MapEditor(this, m_level);
    this->setCentralWidget(m_mapEditor);
  } else {
    showError(tr("Level Error"), tr("Failed to load level"));
  }
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

void Window::showError(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}
