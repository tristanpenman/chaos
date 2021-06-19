#include <fstream>
#include <iostream>

#include <QAction>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>

#include "Rom.h"
#include "RomFactory.h"
#include "Window.h"

using namespace std;

Window::Window(QWidget * parent)
  : QMainWindow(parent)
  , m_debug(false)
{
  setWindowTitle("Chaos");

  QAction *openRomAction = new QAction(tr("&Open ROM..."), this);
  openRomAction->setShortcuts(QKeySequence::Open);
  connect(openRomAction, SIGNAL(triggered()), this, SLOT(showOpenModelDialog()));

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openRomAction);
}

void Window::setDebug(bool debug)
{
  m_debug = debug;
}

bool Window::openRom(const QString &path)
{
  m_file = std::make_shared<fstream>(path.toStdString(), ios::in | ios::out | ios::binary);
  if (!m_file->good()) {
    showError(tr("ROM Error"), tr("Failed to open ROM file"));
    m_file.reset();
    return false;
  }

  m_rom = RomFactory::build(m_file);
  if (!m_rom) {
    showError(tr("ROM Error"), tr("Failed to identify ROM"));
    m_file.reset();
    return false;
  }

  if (m_debug) {
    cout << "[Window] ROM identified" << endl;
    cout << "[Window] Domestic name: '" << m_rom->readDomesticName() << "'" << endl;
  }

  if (!m_rom->parseLevelData()) {
    showError(tr("ROM Error"), tr("Failed to parse level data"));
    m_rom.reset();
    m_file.reset();
    return false;
  }

  return true;
}

void Window::openLevel(const QString& level)
{
  if (!m_rom) {
    showError(tr("Level Error"), tr("Cannot load level until ROM has been loaded"));
    return;
  }

  bool parsed = false;
  unsigned int levelIdx = level.toUInt(&parsed);
  if (!parsed) {
    showError(tr("Level Error"), tr("Failed to parse level index"));
    return;
  }

  if (m_debug) {
    cout << "[Window] Loading level: " << levelIdx << endl;
  }

  uint32_t palettesAddr = m_rom->getPalettesAddr(levelIdx);
  uint32_t patternsAddr = m_rom->getPatternsAddr(levelIdx);
  uint32_t chunksAddr = m_rom->getChunksAddr(levelIdx);
  uint32_t blocksAddr = m_rom->getBlocksAddr(levelIdx);
  uint32_t tilesAddr = m_rom->getTilesAddr(levelIdx);

  if (m_debug) {
    cout << "[Window]  - Palettes: 0x" << hex << palettesAddr << " (" << dec << palettesAddr << ")" << endl;
    cout << "[Window]  - Patterns: 0x" << hex << patternsAddr << " (" << dec << patternsAddr << ")" << endl;
    cout << "[Window]  - Chunks: 0x" << hex << chunksAddr << " (" << dec << chunksAddr << ")" << endl;
    cout << "[Window]  - Blocks: 0x" << hex << blocksAddr << " (" << dec << blocksAddr << ")" << endl;
    cout << "[Window]  - Tiles: 0x" << hex << tilesAddr << " (" << dec << tilesAddr << ")" << endl;
  }
}

void Window::showOpenModelDialog()
{
  const QString fileName = QFileDialog::getOpenFileName(this, tr("Open ROM"), QString(), QString("*.bin"));
  if (!fileName.isEmpty()) {
    openRom(fileName);
  }
}

void Window::showError(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}
