#include <QApplication>
#include <QCommandLineParser>

#include "Window.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.addPositionalArgument(QApplication::translate("main", "rom-file"),
                               QApplication::translate("main", "Path to ROM file"));
  parser.process(app);

  Window window;

  if (!parser.positionalArguments().empty()) {
    const QString filename = parser.positionalArguments().at(0);
    window.openRom(filename);
  }

  window.show();

  return app.exec();
}
