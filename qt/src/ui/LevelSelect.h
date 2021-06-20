#pragma once

#include <memory>

#include <QDialog>
#include <QItemSelection>

class QListView;
class QPushButton;

class Game;

class LevelSelect : public QDialog
{
  Q_OBJECT

public:
  LevelSelect(std::shared_ptr<Game>& game);

signals:
  void levelSelected(int levelIdx);

private:
  std::shared_ptr<Game> m_game;
  QListView* m_listView;
  QPushButton* m_okButton;

private slots:
  void ok(bool);
  void cancel(bool);
  void selectionChanged(QItemSelection selection);
};
