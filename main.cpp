#include <QApplication>

#include "MainWindow.h"
#include "BrowseDirectoryDialog.h"
#include "Utils.h"


void checkScansDirectoryExists(){
  QSettings settings("ValentinMicheletINC", "MangaReader");

  if (settings.value("ScansDirectory").toString().isEmpty()) {
    bool ok;
    QString mangaDirectory;
    BrowseDirectoryDialog* browseDialog = new BrowseDirectoryDialog;
    do {
      ok = (browseDialog->exec() == QDialog::Accepted);
      mangaDirectory = browseDialog->getScansDirectory();
    } while (!ok || mangaDirectory.isEmpty());
    settings.setValue("ScansDirectory", mangaDirectory);
    Utils::m_scansDirectory = QDir(mangaDirectory);
    delete browseDialog;
  }
}


int main(int argc, char** argv) {
  QApplication app(argc, argv);

  checkScansDirectoryExists();

  MainWindow window;
  window.show();

  return app.exec();
}
