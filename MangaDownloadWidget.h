#ifndef MANGADOWNLOADWIDGET_H
#define MANGADOWNLOADWIDGET_H

#include <QtWidgets>
#include <QDir>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>

#include "DownloadManager.h"
#include "MessageListView.h"
#include "MessageListModel.h"
#include "MessageItemDelegate.h"
#include "ChaptersOnWebView.h"
#include "ChaptersOnPCView.h"

class MangaDownloadWidget: public QWidget {
  Q_OBJECT

public:
  MangaDownloadWidget(QWidget* parent = NULL);

protected:
  virtual void keyReleaseEvent(QKeyEvent* event);

public slots:
  void updateChapters(void);
  void chaptersUpdated(bool);
  void startNextDownload(void);
  void downloadChapters(void);
  void chapterDownloaded(bool);
  void pageDownloaded(void);
  void updateChaptersOnPCView(void);
  void searchForDownload(QString mangaName);
  void downloadFinished(void);
  void goToRead(QModelIndex modelIndex);
  void editMessage(QString message, DownloadManager::MessageStatus messageStatus, bool newLine = true);
  void clearMessage(void);
  void stopDownload(void);
  void pauseResumeDownload(bool check);
  void updateProgressBar(int donloadedCount, int totalCount);
  void updatedb(void);

signals:
  void initModelRequested(void);
  void downloadDone(QString, QString);
  void chapterSelected(QString, QString);
  void downloadProgress(int, int);

private:
  QDir _scansDirectory;
  DownloadManager _downloadManager;
  DownloadHTMLManager _downloadHTMLManager;
  QDir _currentMangaDirectory;
  QString _currentChapter;
  QQueue<QString> _downloadQueue;
  int _downloadedCount;
  int _totalCount;
  QLineEdit* _selectLineEdit;
  QPushButton* _updateButton;
  QPushButton* _downloadButton;
  QPushButton* _selectAllButton;
  QStringListModel* _chaptersOnPCModel;
  ChaptersOnPCView* _chaptersOnPCView;
  QStringListModel* _chaptersOnWebModel;
  ChaptersOnWebView* _chaptersOnWebView;
  MessageListModel* _messageModel;
  MessageListView* _messageView;
  MessageItemDelegate* _messageItemDelegate;
  QPushButton* _clearTextEditButton;
  QPushButton* _stopButton;
  QPushButton* _pauseResumeButton;
  QProgressBar* _progressBar;
  QLabel* _chaptersDownloadedLabel;
  QWebView* _chaptersWebView;
  QWebView* _pagesWebView;
};

#endif // MANGADOWNLOADWIDGET_H
