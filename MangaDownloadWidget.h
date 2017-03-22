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
  void updateChaptersList(void);

  void checkMangaNameExists(QString);

  void searchForDownload(QString);

  void updateChaptersOnPCView(void);

  void chaptersListUpdated(int,QProcess::ExitStatus);
  void chaptersListUpdateStarted(void);
  void getChaptersListUpdated(void);

  void downloadFinished(int,QProcess::ExitStatus);
  void nextDownloadHasStarted(void);
  void downloadChapters(void);
  void startNextDownload(void);
  void getDownloadInfo(void);

  void goToRead(QModelIndex modelIndex);

  void updatedb(void);

  void stopDownload(void);

signals:
  void initModelRequested(QString);
  void chapterSelected(QString, QString);
  void downloading(bool);

protected:
  virtual void keyReleaseEvent(QKeyEvent* event);

private:
  QDir _scansDirectory;
  QStringList _mangaList;
  QDir _currentMangaDirectory;
  QString _currentChapter;
  QQueue<QPair<QString, QString>> _downloadQueue;
  QQueue<QModelIndex> _chaptersQueue;
  int _downloadedCount;
  int _totalCount;

  QLineEdit* _selectLineEdit;

  QStringListModel* _chaptersOnPCModel;
  ChaptersOnPCView* _chaptersOnPCView;

  QPushButton* _updateButton;
  QPushButton* _downloadButton;
  QPushButton* _selectAllButton;

  QStandardItemModel* _chaptersOnWebModel;
  ChaptersOnWebView* _chaptersOnWebView;

  QPushButton* _clearTextEditButton;
  QPushButton* _stopButton;
  QProgressBar* _chaptersProgressBar;
  QLabel* _chaptersDownloadedLabel;
  QProgressBar* _pagesProgressBar;
  QLabel* _pagesDownloadedLabel;

  MessageListModel* _messageModel;
  MessageListView* _messageView;
  MessageItemDelegate* _messageItemDelegate;

  QProcess* _getChaptersListProcess;
  QProcess* _downloadChapterProcess;
};

#endif // MANGADOWNLOADWIDGET_H
