#include <fstream>

#include <QApplication>
#include <QCommandLineParser>

#ifdef Q_OS_MAC
#include "os/macos.h"
#endif

#include "Logger.h"

#include "ui/Window.h"

#define LOG Logger("main")

std::fstream log_file;

int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
  removeMacosSpecificMenuItems();
#endif

  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.addPositionalArgument(QApplication::translate("main", "rom-file"),
                               QApplication::translate("main", "Path to ROM file"));

  const QString logOpt("log");
  parser.addOption(QCommandLineOption(logOpt, QObject::tr("Enable logging"), QObject::tr("target")));

  const QString levelOpt("level");
  parser.addOption(QCommandLineOption(levelOpt, QObject::tr("Level to load"), QObject::tr("index")));

  parser.process(app);

  if (parser.isSet(logOpt)) {
    const auto target = parser.value(logOpt);
    if (target.compare("--") == 0) {
      Logger::configure();
    } else {
      log_file.open(target.toStdString(), std::ios::out);
      if (log_file.good()) {
        Logger::configure(log_file);
        LOG << "Begin log";
      } else {
        Logger::configure();
        LOG << "Failed to open log file; falling back to STDOUT";
      }
    }
  }

  Window window;

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
    window.levelSelect();
  }

  window.show();

  return app.exec();
}
