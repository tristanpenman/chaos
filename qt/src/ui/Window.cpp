#include <fstream>
#include <iostream>

#include <QAction>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>

// base
#include "../Game.h"
#include "../GameFactory.h"
#include "../Rom.h"

// ui
#include "LevelSelect.h"
#include "PaletteInspector.h"
#include "PatternInspector.h"
#include "Window.h"

using namespace std;

Window::Window(bool debug)
  : QMainWindow(nullptr)
  , m_paletteInspector(nullptr)
  , m_patternInspector(nullptr)
  , m_debug(debug)
  , m_rom(nullptr)
  , m_game(nullptr)
  , m_level(nullptr)
{
  setWindowTitle("Chaos");
  setMinimumSize(320, 240);

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
  connect(m_levelSelectAction, SIGNAL(triggered()), this, SLOT(showLevelSelect()));

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
  QAction* inspectPalettesAction = new QAction(tr("&Palettes"), this);
  connect(inspectPalettesAction, SIGNAL(triggered()), this, SLOT(showPaletteInspector()));
  QAction* inspectPatternsAction = new QAction(tr("P&atterns"), this);
  connect(inspectPatternsAction, SIGNAL(triggered()), this, SLOT(showPatternInspector()));
  QAction* inspectChunksAction = new QAction(tr("&Chunks"), this);
  inspectChunksAction->setDisabled(true);
  QAction* inspectBlocksAction = new QAction(tr("&Blocks"), this);
  inspectBlocksAction->setDisabled(true);

  // build inspectors sub-menu
  m_inspectorsMenu = viewMenu->addMenu(tr("&Inspectors"));
  m_inspectorsMenu->setDisabled(true);
  m_inspectorsMenu->addAction(inspectPalettesAction);
  m_inspectorsMenu->addAction(inspectPatternsAction);
  m_inspectorsMenu->addSeparator();
  m_inspectorsMenu->addAction(inspectChunksAction);
  m_inspectorsMenu->addAction(inspectBlocksAction);
}

bool Window::openRom(const QString &path)
{
  m_rom = std::make_shared<Rom>();
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

  if (m_debug) {
    cout << "[Window] ROM identified" << endl;
    cout << "[Window] Domestic name: '" << m_rom->readDomesticName() << "'" << endl;
  }

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
      showLevelSelect();
    }
  }
}

void Window::showLevelSelect()
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

  m_levelSelect = new LevelSelect(this, m_game);
  connect(m_levelSelect, SIGNAL(levelSelected(int)), this, SLOT(levelSelected(int)));
  connect(m_levelSelect, SIGNAL(finished(int)), this, SLOT(levelSelectFinished(int)));
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
  }

  m_level.reset();

  if (!m_rom) {
    showError(tr("Level Error"), tr("Cannot load level until ROM has been loaded"));
    return;
  }

  if (m_debug) {
    cout << "[Window] Loading level: " << levelIdx << " (" << m_game->getTitleCards()[levelIdx] << ")" << endl;
  }

  m_level = m_game->loadLevel(levelIdx);

  if (m_level) {
    m_inspectorsMenu->setEnabled(true);
  } else {
    showError(tr("Level Error"), tr("Failed to load level"));
  }
}

void Window::levelSelectFinished(int)
{
  delete m_levelSelect;
  m_levelSelect = nullptr;
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

void Window::showError(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}
