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
  void checkMangaDirectoryExists();

public slots:
  void switchToRead(QString mangaName, QString chapterName);
  void switchToDownload(QString mangaName);

signals:

private:
  MangaListWidget* m_mangaListWidget;
  MangaReadWidget* m_mangaReadWidget;
  MangaDownloadWidget* m_mangaDownloadWidget;
  QTabWidget* m_tabWidget;

  int m_chaptersDownloaded;
  int m_totalChaptersToDownload;
};

#endif // MAINWINDOW_H
