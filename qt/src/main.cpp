#include <QApplication>
#include <QCommandLineParser>

#include "ui/Window.h"

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

  Window window(parser.isSet(debugOpt));

  // attempt to load a rom if a positional argument is provided
  bool romLoaded = false;
  bool skipLevel = false;
  if (!parser.positionalArguments().empty()) {
    const QString filename = parser.positionalArguments().at(0);
    romLoaded = window.openRom(filename);
    if (!romLoaded) {
      skipLevel = true;
    }
  }

  // only parse level option if rom was loaded, or no positional argument provided
  // this is to prevent multiple error messages being shown
  if (parser.isSet(levelOpt) && !skipLevel) {
    window.openLevel(parser.value(levelOpt));
  } else if (romLoaded) {
    window.showLevelSelect();
  }

  window.show();

  return app.exec();
}
