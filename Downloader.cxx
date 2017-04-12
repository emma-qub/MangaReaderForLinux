#include "Downloader.hxx"

#include "Utils.hxx"

#include <QStandardItem>

Downloader::Downloader(QObject *p_parent):
  QObject(p_parent) {

  /// Process to get chapters available for download
  m_getChaptersListProcess = new QProcess(this);
  connect(m_getChaptersListProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &Downloader::chaptersListfinished);
  connect(m_getChaptersListProcess, &QProcess::started, this, &Downloader::chaptersListStarted);
  connect(m_getChaptersListProcess, &QProcess::readyReadStandardOutput, this, &Downloader::readyReadStandardOutput);
}

void Downloader::updateChaptersList(QString const& p_mangaName) {
  if (p_mangaName.isEmpty()) {
    return;
  }

  QStringList arguments({p_mangaName});
  m_getChaptersListProcess->start(Utils::getScriptsAbsolutePath()+"/updateChaptersList.sh", arguments);
}

void Downloader::readyReadStandardOutput() {
  QString chaptersList(m_getChaptersListProcess->readAllStandardOutput());

  QStringList chaptersUrlAndTitleList = chaptersList.split("\n", QString::SkipEmptyParts);

  for (const QString& chapterUrlAndTitle: chaptersUrlAndTitleList) {
    QStringList chapterUrlAndTitleList = chapterUrlAndTitle.split(";");
    QStandardItem* chapterItem = nullptr;

    if (chapterUrlAndTitleList.size() == 2) {
      QString chapterTitle = chapterUrlAndTitleList.at(1).trimmed();
      QString chapterUrl = chapterUrlAndTitleList.at(0).trimmed();
      QString chapterTitleInUrl;
      if (chapterUrl.endsWith('/')) {
        chapterUrl.truncate(chapterUrl.length()-1);
      }
      chapterTitleInUrl = chapterUrl.split('/').last();


      chapterItem = new QStandardItem(chapterTitle);
      chapterItem->setData(chapterUrl, eChapterURLRole);
      chapterItem->setData(chapterTitleInUrl, eChapterTitleInURLRole);
    }

    emit chaptersListUpdated(chapterItem);
  }
}
