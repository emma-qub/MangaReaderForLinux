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

  DownloadManager(const QString& mangaName = "", QObject* parent = nullptr);

  void append(const QStringList& urlList);
  QString saveFileName();
  virtual void clean();

  void stop();
  void pause();
  void resume();

  inline void setMangaName(const QString& mangaName) { m_mangaName = mangaName; }

protected:
  void append(const QUrl& url);

protected slots:
  virtual void startNextDownload();
  virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  virtual void downloadFinished();
  virtual void downloadReadyRead();

signals:
  void done();
  void message(QString, DownloadManager::MessageStatus, bool newLine = true);
  void nbFilesDownloaded(int, int);

protected:
  QNetworkAccessManager manager;
  QQueue<QUrl> downloadQueue;
  QNetworkReply* currentDownload;
  QFile output;
  QTime downloadTime;
  QString m_mangaName;
  QDir m_scansDirectory;
  QDir m_mangaDirectory;
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
  DownloadHTMLManager(const QString& mangaName = "", QObject* parent = nullptr);

  inline QStringList getHTMLPages() const { return m_htmlPages; }
  inline void clearHTMLPages() { m_htmlPages.clear(); }

  virtual void clean();

public slots:
  virtual void startNextDownload();
  virtual void downloadFinished();

protected:
  QStringList m_htmlPages;
};

#endif
