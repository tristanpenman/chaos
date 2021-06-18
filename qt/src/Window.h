#pragma once

#include <iosfwd>
#include <memory>

#include <QMainWindow>

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

  std::shared_ptr<std::istream> m_rom;
};
