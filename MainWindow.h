#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "SlidingStackedWidget.hxx"
#include "MangaListWidget.h"
#include "MangaReadWidget.h"
#include "MangaDownloadWidget.h"
#include "NotificationDialog.h"

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  enum MangaPage {
    eList,
    eRead
  };

  MainWindow(QMainWindow* parent = nullptr);

protected slots:
  void switchToRead(QString const& p_mangaName, QString const& p_chapterName);
  void switchToDownload(QString const& p_mangaName);
  void switchToList();

private:
  MangaListWidget* m_mangaListWidget;
  MangaReadWidget* m_mangaReadWidget;
  MangaDownloadWidget* m_mangaDownloadWidget;
  SlidingStackedWidget* m_stackedWidget;
  QTabWidget* m_tabWidget;
};

#endif // MAINWINDOW_H
