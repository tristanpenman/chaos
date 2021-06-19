#include <QApplication>
#include <QCommandLineParser>

#include "Window.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.addPositionalArgument(QApplication::translate("main", "rom-file"),
                               QApplication::translate("main", "Path to ROM file"));

  const QString debugOpt("debug");
  parser.addOption(QCommandLineOption(debugOpt, QObject::tr("Enable debug mode")));

  const QString levelOpt("level");
  parser.addOption(QCommandLineOption(levelOpt, QObject::tr("Level to load"), QObject::tr("index")));

  parser.process(app);

  Window window;

  if (parser.isSet(debugOpt)) {
    window.setDebug(true);
  }

  bool okay = true;

  if (!parser.positionalArguments().empty()) {
    const QString filename = parser.positionalArguments().at(0);
    okay = window.openRom(filename);
  }

  if (okay && parser.isSet(levelOpt)) {
    window.openLevel(parser.value(levelOpt));
  }

  window.show();

  return app.exec();
}
