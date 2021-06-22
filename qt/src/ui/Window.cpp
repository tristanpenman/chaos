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
#include "../Rom.h"

#include "LevelSelect.h"
#include "Window.h"

using namespace std;

Window::Window(bool debug)
  : QMainWindow(nullptr)
  , m_debug(debug)
{
  setWindowTitle("Chaos");
  setMinimumSize(320, 240);

  // choose a nice default width and height, and center the window
  QRect geometry = QGuiApplication::primaryScreen()->geometry();
  const int width = geometry.height() * 0.75;
  const int height = geometry.height() * 0.5;
  setGeometry(0, 0, width, height);
  move(geometry.center() - rect().center());

  // write up Open ROM action
  QAction *openRomAction = new QAction(tr("&Open ROM..."), this);
  openRomAction->setShortcuts(QKeySequence::Open);
  connect(openRomAction, SIGNAL(triggered()), this, SLOT(showOpenModelDialog()));

  // track Level Select action so we can enable it later
  m_levelSelectAction = new QAction(tr("&Level Select..."), this);
  m_levelSelectAction->setDisabled(true);
  connect(m_levelSelectAction, SIGNAL(triggered()), this, SLOT(showLevelSelectDialog()));

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openRomAction);
  fileMenu->addAction(m_levelSelectAction);
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

void Window::showOpenModelDialog()
{
  const QString fileName = QFileDialog::getOpenFileName(this, tr("Open ROM"), QString(), QString("*.bin"));
  if (!fileName.isEmpty()) {
    if (openRom(fileName)) {
      showLevelSelectDialog();
    }
  }
}

void Window::showLevelSelectDialog()
{
  m_levelSelect = new LevelSelect(this, m_game);
  connect(m_levelSelect, SIGNAL(levelSelected(int)), this, SLOT(levelSelected(int)));
  connect(m_levelSelect, SIGNAL(finished(int)), this, SLOT(levelSelectFinished(int)));
  m_levelSelect->show();
}

void Window::levelSelected(int levelIdx)
{
  if (!m_rom) {
    showError(tr("Level Error"), tr("Cannot load level until ROM has been loaded"));
    return;
  }

  if (m_debug) {
    cout << "[Window] Loading level: " << levelIdx << " (" << m_game->getTitleCards()[levelIdx] << ")" << endl;
  }

  uint32_t palettesAddr = m_game->getPalettesAddr(levelIdx);
  uint32_t patternsAddr = m_game->getPatternsAddr(levelIdx);
  uint32_t chunksAddr = m_game->getChunksAddr(levelIdx);
  uint32_t blocksAddr = m_game->getBlocksAddr(levelIdx);
  uint32_t tilesAddr = m_game->getTilesAddr(levelIdx);

  if (m_debug) {
    cout << "[Window]  - Palettes: 0x" << hex << palettesAddr << " (" << dec << palettesAddr << ")" << endl;
    cout << "[Window]  - Patterns: 0x" << hex << patternsAddr << " (" << dec << patternsAddr << ")" << endl;
    cout << "[Window]  - Chunks: 0x" << hex << chunksAddr << " (" << dec << chunksAddr << ")" << endl;
    cout << "[Window]  - Blocks: 0x" << hex << blocksAddr << " (" << dec << blocksAddr << ")" << endl;
    cout << "[Window]  - Tiles: 0x" << hex << tilesAddr << " (" << dec << tilesAddr << ")" << endl;
  }
}

void Window::levelSelectFinished(int)
{
  delete m_levelSelect;
  m_levelSelect = nullptr;
}

void Window::showError(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}
