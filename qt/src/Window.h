#pragma once

#include <iosfwd>
#include <memory>

#include <QMainWindow>

class Rom;

class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window(QWidget* parent = 0);

  void setDebug(bool debug);

  bool openRom(const QString& path);
  void openLevel(const QString& level);

public slots:
  void showOpenModelDialog();

private:
  void showError(const QString& title, const QString& text);

  bool m_debug;

  std::shared_ptr<std::fstream> m_file;
  std::shared_ptr<Rom> m_rom;
};
