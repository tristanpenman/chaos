#include <fstream>

#include <QAction>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>

#include "Window.h"

using namespace std;

Window::Window(QWidget * parent)
  : QMainWindow(parent)
{
  setWindowTitle("Chaos");

  QAction *openRomAction = new QAction(QString("&Open ROM..."), this);
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
  m_rom = std::make_shared<ifstream>(path.toStdString(), ios::binary);
  if (!m_rom->good()) {
    showError("Open ROM", "Failed to open ROM file");
    m_rom.reset();
  }

  if (!parseLevelData()) {
    showError("Open ROM", "Failed to parse level data");
    m_rom.reset();
  }
}

bool Window::parseLevelData()
{
  return false;
}

void Window::showError(const QString& title, const QString& text)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.exec();
}
