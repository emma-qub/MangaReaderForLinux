#include "DownloadManager.h"
#include "Utils.h"

#include <cmath>

#include <QFileInfo>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QDebug>
#include <QSettings>

#include <iostream>


/////////////////////////////////////
///       DownloadManager         ///
/////////////////////////////////////

DownloadManager::DownloadManager(const QString& mangaName, QObject* parent) :
  QObject(parent),
  _mangaName(mangaName),
  _scansDirectory(Utils::getScansDirectory()),
  downloadedCount(0),
  totalCount(0),
  downloadSizeAtPause(0),
  isOnPause(false),
  currentUrl() {

  _mangaDirectory = QDir(_scansDirectory.path()+"/"+_mangaName+"/");
}

void DownloadManager::append(const QStringList& urlList) {
  foreach (const QString& url, urlList)
    append(QUrl::fromEncoded(url.toLocal8Bit()));

  emit message("Adding "+QString::number(urlList.size())+" files to download queue...", Information);

  //    if (downloadQueue.isEmpty())
  //        QTimer::singleShot(0, this, SIGNAL(done()));
}

void DownloadManager::append(const QUrl& url) {
  if (downloadQueue.isEmpty())
    QTimer::singleShot(0, this, SLOT(startNextDownload()));

  downloadQueue.enqueue(url);
  ++totalCount;
}

QString DownloadManager::saveFileName(void) {
  QString basename = QFileInfo(currentUrl.path()).fileName();

  if (basename.isEmpty()) {
    basename = "download";
    emit message("Warning: file name empty within DownloadManager::saveFileName", Warning);
  }

  QString mangaName;
  QString chapterName;
  QString pathName = _mangaDirectory.path() + "/";

  QStringList directories = currentUrl.path().split("/");

  if (directories.size() > 2) {
    mangaName = directories.at(directories.size()-3);
    chapterName = directories.at(directories.size()-2);
    pathName += mangaName+"/"+chapterName+"/"+basename;
  } else {
    pathName += "Misc/";
    emit message("Warning: file will be downloaded in "+pathName, Warning);
  }

  return pathName;
}

void DownloadManager::clean(void) {
  totalCount = 0;
  downloadedCount = 0;
}

void DownloadManager::startNextDownload(void) {
  downloadSizeAtPause = 0;

  emit nbFilesDownloaded(downloadedCount, totalCount);

  if (downloadQueue.isEmpty()) {
    QString downloadReport = QString::number(downloadedCount)+"/"+QString::number(totalCount)+" file(s) downloaded successfully.";
    if (downloadedCount == totalCount) {
      emit message(downloadReport, Success);
    } else {
      emit message("Only "+downloadReport, Warning);
      emit message("Please check out each issue and retry if necessary.", Warning);
    }

    clean();
    emit done();
    return;
  }

  if (!isOnPause) {
    currentUrl = downloadQueue.dequeue();
    currentRequest = QNetworkRequest(currentUrl);
    QString fileName = saveFileName();
    output.setFileName(fileName);
  } else
    isOnPause = false;

  QString fileName = output.fileName();
  if (!output.open(QIODevice::Append)) {
    QString errorStatus = "Error: problem opening save file " + fileName + " for download ";
    errorStatus += currentUrl.toEncoded().constData();
    errorStatus += ": " + output.errorString();
    emit message(errorStatus, DownloadManager::Error);
    startNextDownload();
    return;
  }

  currentDownload = manager.get(currentRequest);
  connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
  connect(currentDownload, SIGNAL(finished()), this, SLOT(downloadFinished()));
  connect(currentDownload, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));

  // prepare the output
  QString status = "Downloading ";
  status += currentUrl.toEncoded().constData();
  status += "...";
  emit message(status, Information);

  // Do not erase: empty message in order to start download information messages
  emit message("", DownloadManager::Information);

  downloadTime.start();
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
  // calculate the download speed
  double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
  QString unit;
  if (speed < 1024) {
    unit = " B/s";
  } else if (speed < 1024*1024) {
    speed /= 1024;
    unit = "kB/s";
  } else {
    speed /= 1024*1024;
    unit = "MB/s";
  }

  bytesReceived += downloadSizeAtPause;

  // Compute how many char # and - needed
  if (bytesTotal != 0) {
    int percent = bytesReceived * 100 / bytesTotal;
    int nbSharp = percent/4;

    // Create message to display while downloading
    int nbSpaces = -1;

    // Current/Total
    QString downloadInfo = QString::number(downloadedCount+1)+"/"+QString::number(totalCount);

    // Spaces
    nbSpaces = 7;
    if (bytesTotal != 0)
      nbSpaces = 7-std::floor(std::log10(bytesReceived/1024))-std::floor(std::log10(bytesTotal/1024));
    for (int k = 0; k < nbSpaces; k++) {
      downloadInfo += " ";
    }

    // Progress in xB/s
    downloadInfo += QString::number(bytesReceived/1024)+"/"+QString::number(bytesTotal/1024)+"kB";

    // Spaces
    nbSpaces = 5-std::floor(std::log10(speed));
    for (int k = 0; k < nbSpaces; k++) {
      downloadInfo += " ";
    }

    downloadInfo += QString::number(static_cast<int>(speed))+unit+"   [";

    // Progress bar
    for (int k = 0; k < 25; k++) {
      if (k < nbSharp) {
        downloadInfo += "#";
      } else {
        downloadInfo += "-";
      }
    }
    downloadInfo += "]";

    // Spaces
    nbSpaces = 5;
    if (percent != 0)
      nbSpaces -= std::floor(std::log10(percent));
    for (int k = 0; k < nbSpaces; k++) {
      downloadInfo += " ";
    }

    // Percent
    downloadInfo += QString::number(percent)+"%";

    // Send message
    emit message(downloadInfo, Download, false);
  }
}

void DownloadManager::downloadFinished(void) {
  output.close();

  if (currentDownload->error()) {
    emit message("Failed: "+currentDownload->errorString(), Error);
  } else {
    ++downloadedCount;
  }

  currentDownload->deleteLater();
  startNextDownload();
}

void DownloadManager::downloadReadyRead(void) {
  output.write(currentDownload->readAll());
}

void DownloadManager::stop(void) {
  if (!currentDownload)
    return;

  emit message("Download aborted.", DownloadManager::Error);

  disconnect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
  disconnect(currentDownload, SIGNAL(finished()), this, SLOT(downloadFinished()));
  disconnect(currentDownload, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));

  QStringList directories = output.fileName().split("/");
  directories.removeLast();
  QString chapterName = directories.last();
  QString chapterPath = directories.join("/");

  currentDownload->abort();
  output.close();
  currentDownload = NULL;
  downloadQueue.clear();
  downloadedCount = 0;
  totalCount = 0;
  downloadSizeAtPause = 0;

  Utils::removeDirectory(chapterPath);

  emit message("You will have to redownload chapter "+chapterName+".", DownloadManager::Warning);
}

void DownloadManager::pause(void) {
  if (!currentDownload)
    return;

  emit message("Download is on pause.", DownloadManager::Warning);

  isOnPause = true;

  disconnect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
  disconnect(currentDownload, SIGNAL(finished()), this, SLOT(downloadFinished()));
  disconnect(currentDownload, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));

  currentDownload->abort();
  output.write(currentDownload->readAll());
  output.close();
  currentDownload = NULL;
}

void DownloadManager::resume(void) {
  emit message("Resume download.", DownloadManager::Warning);

  downloadSizeAtPause = output.size();

  QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(downloadSizeAtPause) + "-";
  currentRequest.setRawHeader("Range", rangeHeaderValue);

  startNextDownload();
}



//////////////////////////////////////
///       DownloadHTMLManager      ///
//////////////////////////////////////
DownloadHTMLManager::DownloadHTMLManager(const QString& mangaName, QObject* parent) :
  DownloadManager(mangaName, parent),
  _htmlPages() {
}

void DownloadHTMLManager::startNextDownload(void) {
  emit nbFilesDownloaded(downloadedCount, totalCount);

  if (downloadQueue.isEmpty()) {
    emit message("Every information collected.", Success);
    emit done();
    return;
  }

  emit message("Analyzing file "+QString::number(downloadedCount+1)+"/"+QString::number(totalCount)+"...", Information, false);

  QUrl url = downloadQueue.dequeue();

  QNetworkRequest request(url);
  currentDownload = manager.get(request);
  connect(currentDownload, SIGNAL(finished()), SLOT(downloadFinished()));

  downloadTime.start();
}

void DownloadHTMLManager::clean(void) {
  DownloadManager::clean();
  _htmlPages.clear();
}

void DownloadHTMLManager::downloadFinished(void) {
  if (currentDownload->error()) {
    emit message("Failed: "+currentDownload->errorString(), Error);
  } else {
    ++downloadedCount;

    _htmlPages << currentDownload->readAll();
  }

  currentDownload->deleteLater();
  startNextDownload();
}
