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

  void openRom(const QString& path);

public slots:
  void showOpenModelDialog();

private:
  bool parseLevelData();

  void showError(const QString& title, const QString& text);

  std::shared_ptr<std::fstream> m_file;
  std::shared_ptr<Rom> m_rom;
};
