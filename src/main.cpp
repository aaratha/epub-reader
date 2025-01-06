#include "parser.hpp"
#include "window.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  App mainWindow;
  mainWindow.show();

  EpubParser parser;
  QString epubPath = "/Users/aaratha/projects/qt-demo/don_quixote.epub";

  if (parser.open(epubPath)) {
    // Get the first chapter content (index 0)
    QString firstChapterText = parser.getChapterContent(0);
    QString subString =
        firstChapterText.mid(0, 1000); // subString contains "is"
    if (!firstChapterText.isEmpty()) {
      mainWindow.setChapterContent(subString);
    } else {
      mainWindow.setChapterContent("Failed to load chapter content.");
    }
  } else {
    mainWindow.setChapterContent("Failed to open the specified ePub file.");
  }
  return app.exec();
}
