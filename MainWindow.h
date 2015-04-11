#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "MangaListWidget.h"
#include "MangaReadWidget.h"
#include "MangaDownloadWidget.h"

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QMainWindow* parent = NULL);

private:
  void checkMangaDirectoryExists(void);

public slots:
  void switchToRead(QString mangaName, QString chapterName);
  void switchToDownload(QString mangaName);

signals:
  void toReadSwitched(QString mangaName, QString chapterName);
  void toDownloadSwitched(QString mangaName);

private:
  MangaListWidget* _mangaListWidget;
  MangaReadWidget* _mangaReadWidget;
  MangaDownloadWidget* _mangaDownloadWidget;
  QTabWidget* _tabWidget;
};

#endif // MAINWINDOW_H
