#include "Downloader.hxx"
#include "Utils.hxx"

#include <QStandardItem>

#include <QDebug>

Downloader::Downloader(QObject *p_parent):
  QObject(p_parent),
  m_chaptersStack(),
  m_downloadedCount(0),
  m_totalCount(0),
  m_currentItem(nullptr) {

  /// Process to fetch available chapters
  m_fetchChaptersListProcess = new QProcess(this);
  connect(m_fetchChaptersListProcess, &QProcess::started, this, &Downloader::chaptersListStarted);
  connect(m_fetchChaptersListProcess, &QProcess::readyReadStandardOutput, this, &Downloader::readyReadStandardOutput);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
  connect(m_fetchChaptersListProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &Downloader::chaptersListfinished);
#else
  connect<void(QProcess::*)(int, QProcess::ExitStatus)>(m_fetchChaptersListProcess, &QProcess::finished, this, &Downloader::chaptersListfinished);
#endif

  /// Process to download available chapters
  m_downloadChapterProcess = new QProcess(this);
//  connect(m_downloadChapterProcess, &QProcess::started, this, &Downloader::);
  connect(m_downloadChapterProcess, &QProcess::readyReadStandardOutput, this, &Downloader::getDownloadInfo);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
  connect(m_downloadChapterProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &Downloader::startNextDownload);
#else
  connect<void(QProcess::*)(int, QProcess::ExitStatus)>(m_downloadChapterProcess, &QProcess::finished, this, &Downloader::startNextDownload);
#endif
}

Downloader::~Downloader() {
  qDeleteAll(m_chaptersStack);
}

void Downloader::fetchChaptersList(QString const& p_mangaName) {
  if (p_mangaName.isEmpty()) {
    return;
  }

  m_currentMangaName = p_mangaName;
  QStringList arguments({p_mangaName.toLower().replace("-", "_")});
  m_fetchChaptersListProcess->start(Utils::getScriptsAbsolutePath()+"/updateChaptersList.sh", arguments);
}

void Downloader::readyReadStandardOutput() {
  QString chaptersList(m_fetchChaptersListProcess->readAllStandardOutput());

  QStringList chaptersUrlAndTitleList = chaptersList.split("\n", QString::SkipEmptyParts);

  for (const QString& chapterUrlAndTitle: chaptersUrlAndTitleList) {
    QStringList chapterUrlAndTitleList = chapterUrlAndTitle.split(";");
    QStandardItem* chapterItem = nullptr;

    if (chapterUrlAndTitleList.size() == 2) {
      //QString chapterTitle = chapterUrlAndTitleList.at(1).trimmed();
      QString chapterUrl = chapterUrlAndTitleList.at(0).trimmed();
      QString chapterTitleInUrl;
      if (chapterUrl.endsWith('/')) {
        chapterUrl.truncate(chapterUrl.length()-1);
      }
      chapterTitleInUrl = chapterUrl.split('/').last();

      chapterItem = new QStandardItem(chapterTitleInUrl);
      chapterItem->setData(chapterUrl, eChapterURLRole);
      chapterItem->setData(chapterTitleInUrl, eChapterTitleInURLRole);
    }

    emit chaptersListFetched(chapterItem);
  }
}

void Downloader::downloadAvailableChapters(QList<QStandardItem*> const& chapterItems) {
  for (auto chapterItem: chapterItems) {
    m_chaptersStack.push_back(chapterItem);
  }

  m_totalCount = m_chaptersStack.size();
  m_downloadedCount = 0;

  emit downloading(true);

  startNextDownload();
}

void Downloader::cleanCurrentChapterItem() {
  emit currentChapterItemAboutToBeDeleted(m_currentItem);
  delete m_currentItem;
  m_currentItem = nullptr;
}

void Downloader::startNextDownload() {
  if (m_chaptersStack.isEmpty()) {
    m_totalCount = 0;
    m_downloadedCount = 0;

    // Clean currentItem
    cleanCurrentChapterItem();

    //emit initModelRequested(m_selectLineEdit->text());
    emit downloading(false);
    return;
  }

  // Clean currentItem
  cleanCurrentChapterItem();

  m_currentItem = m_chaptersStack.pop();
  auto chapterTitle = m_currentItem->data(eChapterTitleInURLRole).toString();
  auto chapterURL = m_currentItem->data(eChapterURLRole).toString();

  QStringList arguments;
  QString pathToChapterScan = Utils::getScansDirectory().absolutePath()+"/"+m_currentMangaName;
  arguments << chapterURL << pathToChapterScan << chapterTitle;

  m_downloadChapterProcess->start(Utils::getScriptsAbsolutePath()+"/downloadChapter.sh", arguments);
}

void Downloader::getDownloadInfo() {
  QString downloadOutput(m_downloadChapterProcess->readAllStandardOutput());
  QStringList pageRatioList = downloadOutput.split("/");

  float ip = pageRatioList.at(0).toFloat();
  float np = pageRatioList.at(1).toFloat();

  int chapterAdvance = static_cast<int>(ip*100.f / np);
  emit chapterDownloadAdvanced(m_currentItem, chapterAdvance);

  if (chapterAdvance == 100) {
    updatedb();
  }
//  emit m_pagesDownloadedLabel->setText("Page "+downloadOutput);

//  float nc = static_cast<float>(m_totalCount);
//  int mangaStep = static_cast<int>(ip*100.f / (nc*np));

//  float ic = static_cast<float>(m_totalCount-m_downloadQueue.size()-1);
//  int chaptersAdvance = static_cast<int>(ic*100.f / nc);

//  m_chaptersProgressBar->setValue(chaptersAdvance + mangaStep);
}

void Downloader::updatedb() {
  Utils::addChapter(m_currentMangaName, m_currentItem->text());
}
