#ifndef MANGADOWNLOADWIDGET_H
#define MANGADOWNLOADWIDGET_H

#include <QtWidgets>
#include <QDir>
#include <QtWebView>

#include "DownloadManager.h"
#include "MessageListView.h"
#include "MessageListModel.h"
#include "MessageItemDelegate.h"
#include "ChaptersOnWebView.h"
#include "ChaptersOnPCView.h"

class MangaDownloadWidget: public QWidget {
  Q_OBJECT

public:
  MangaDownloadWidget(QWidget* parent = nullptr);

public slots:
  void updateChaptersList();

  void checkMangaNameExists(QString);

  void searchForDownload(QString);

  void updateChaptersOnPCView();

  void chaptersListUpdated(int,QProcess::ExitStatus);
  void chaptersListUpdateStarted();
  void getChaptersListUpdated();

  void downloadFinished(int,QProcess::ExitStatus);
  void nextDownloadHasStarted();
  void downloadChapters();
  void startNextDownload();
  void getDownloadInfo();

  void goToRead(QModelIndex modelIndex);

  void updatedb();

  void stopDownload();

signals:
  void initModelRequested(QString);
  void chapterSelected(QString, QString);
  void downloading(bool);

protected:
  virtual void keyReleaseEvent(QKeyEvent* event);

private:
  QDir m_scansDirectory;
  QStringList m_mangaList;
  QDir m_currentMangaDirectory;
  QString m_currentChapter;
  QQueue<QPair<QString, QString>> m_downloadQueue;
  QQueue<QModelIndex> m_chaptersQueue;
  int m_downloadedCount;
  int m_totalCount;

  QLineEdit* m_selectLineEdit;

  QStringListModel* m_chaptersOnPCModel;
  ChaptersOnPCView* m_chaptersOnPCView;

  QPushButton* m_updateButton;
  QPushButton* m_downloadButton;
  QPushButton* m_selectAllButton;

  QStandardItemModel* m_chaptersOnWebModel;
  ChaptersOnWebView* m_chaptersOnWebView;

  QPushButton* m_clearTextEditButton;
  QPushButton* m_stopButton;
  QProgressBar* m_chaptersProgressBar;
  QLabel* m_chaptersDownloadedLabel;
  QProgressBar* m_pagesProgressBar;
  QLabel* m_pagesDownloadedLabel;

  MessageListModel* m_messageModel;
  MessageListView* m_messageView;
  MessageItemDelegate* m_messageItemDelegate;

  QProcess* m_getChaptersListProcess;
  QProcess* m_downloadChapterProcess;
};

#endif // MANGADOWNLOADWIDGET_H
