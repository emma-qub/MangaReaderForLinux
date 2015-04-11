#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QProgressBar>

/////////////////////////////////////
///       DownloadManager         ///
/////////////////////////////////////
class DownloadManager: public QObject {
  Q_OBJECT

public:
  enum MessageStatus {
    Information,
    Success,
    Warning,
    Error,
    Download
  };

  DownloadManager(const QString& mangaName = "", QObject* parent = NULL);

  void append(const QStringList& urlList);
  QString saveFileName(void);
  virtual void clean(void);

  void stop(void);
  void pause(void);
  void resume(void);

  inline void setMangaName(const QString& mangaName) { _mangaName = mangaName; }

protected:
  void append(const QUrl& url);

protected slots:
  virtual void startNextDownload(void);
  virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  virtual void downloadFinished(void);
  virtual void downloadReadyRead(void);

signals:
  void done(void);
  void message(QString, DownloadManager::MessageStatus, bool newLine = true);
  void nbFilesDownloaded(int, int);

protected:
  QNetworkAccessManager manager;
  QQueue<QUrl> downloadQueue;
  QNetworkReply* currentDownload;
  QFile output;
  QTime downloadTime;
  QString _mangaName;
  QDir _scansDirectory;
  QDir _mangaDirectory;
  int downloadedCount;
  int totalCount;
  qint64 downloadSizeAtPause;
  bool isOnPause;
  QUrl currentUrl;
  QNetworkRequest currentRequest;
};

//////////////////////////////////////
///       DownloadHTMLManager      ///
//////////////////////////////////////
class DownloadHTMLManager: public DownloadManager {
  Q_OBJECT

public:
  DownloadHTMLManager(const QString& mangaName = "", QObject* parent = NULL);

  inline QStringList getHTMLPages(void) const { return _htmlPages; }
  inline void clearHTMLPages(void) { _htmlPages.clear(); }

  virtual void clean(void);

public slots:
  virtual void startNextDownload(void);
  virtual void downloadFinished(void);

protected:
  QStringList _htmlPages;
};

#endif
