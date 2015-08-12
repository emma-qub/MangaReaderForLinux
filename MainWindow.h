#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "MangaListWidget.h"
#include "MangaReadWidget.h"
#include "MangaDownloadWidget.h"
#include "NotificationDialog.h"

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QMainWindow* parent = nullptr);

private:
  void checkMangaDirectoryExists(void);

public slots:
  void switchToRead(QString mangaName, QString chapterName);
  void switchToDownload(QString mangaName);

signals:

private:
  MangaListWidget* _mangaListWidget;
  MangaReadWidget* _mangaReadWidget;
  MangaDownloadWidget* _mangaDownloadWidget;
  QTabWidget* _tabWidget;

  int _chaptersDownloaded;
  int _totalChaptersToDownload;
};

#endif // MAINWINDOW_H
