#include <fstream>

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
{
  setWindowTitle("Chaos");

  QAction *openRomAction = new QAction(tr("&Open ROM..."), this);
  openRomAction->setShortcuts(QKeySequence::Open);
  connect(openRomAction, SIGNAL(triggered()), this, SLOT(showOpenModelDialog()));

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openRomAction);
}

void Window::showOpenModelDialog()
{
  const QString fileName = QFileDialog::getOpenFileName(this, tr("Open ROM"), QString(), QString("*.bin"));
  if (!fileName.isEmpty()) {
    openRom(fileName);
  }
}

void Window::openRom(const QString &path)
{
  m_file = std::make_shared<fstream>(path.toStdString(), ios::binary);
  if (!m_file->good()) {
    showError(tr("ROM Error"), tr("Failed to open ROM file"));
    m_rom.reset();
  }

  m_rom = RomFactory::build(m_file);
  if (!m_rom) {
    showError(tr("ROM Error"), tr("Failed to identify ROM"));
    m_file.reset();
  }

  if (!m_rom->parseLevelData()) {
    showError(tr("ROM Error"), tr("Failed to parse level data"));
    m_rom.reset();
    m_file.reset();
  }
}

void Window::showError(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}
