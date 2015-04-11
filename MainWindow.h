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
  MainWindow(QMainWindow* parent = NULL);

private:
  void checkMangaDirectoryExists(void);

public slots:
  void switchToRead(QString mangaName, QString chapterName);
  void switchToDownload(QString mangaName);
  void notifyDownload(QString title, QString message);

signals:
  void toReadSwitched(QString mangaName, QString chapterName);
  void toDownloadSwitched(QString mangaName);

private:
  MangaListWidget* _mangaListWidget;
  MangaReadWidget* _mangaReadWidget;
  MangaDownloadWidget* _mangaDownloadWidget;
  QTabWidget* _tabWidget;
  NotificationDialog* _notificationDialog;
};

#endif // MAINWINDOW_H
