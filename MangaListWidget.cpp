#include "MangaListWidget.h"
#include "Utils.h"
#include "AddMangaDialog.h"
#include "MangaListDelegate.hxx"


/// PUBLIC

MangaListWidget::MangaListWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_scansDirectory(Utils::getScansDirectory()),
  m_currentChaptersListOnWeb(),
  m_chaptersToCheck(),
  m_currentIndex() {

  /// Available chapters process
  m_checkAvailableChaptersProcess = new QProcess(this);
  connect(m_checkAvailableChaptersProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(checkAvailableChapterIsDone(int,QProcess::ExitStatus)));
  connect(m_checkAvailableChaptersProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));

  /// Check new chapters button
  QFont titleFont("FontAwesome", 14, 99);
  m_checkNewChaptersButton = new QPushButton;
  m_checkNewChaptersButton->setFlat(true);
  m_checkNewChaptersButton->setFont(titleFont);
  m_checkNewChaptersButton->setObjectName("m_checkNewChaptersButton");
  m_checkNewChaptersButton->setText("\uf021");
  connect(m_checkNewChaptersButton, &QPushButton::clicked, this, &MangaListWidget::checkAvailableDownloads);

  /// Manga model
  m_mangaModel = new QStandardItemModel;
  initModel();

  /// Manga proxy model
  m_mangaProxyModel = new QSortFilterProxyModel;
  m_mangaProxyModel->setSourceModel(m_mangaModel);

  /// Manga view
  m_mangaListView = new QListView;
  m_mangaListView->setModel(m_mangaProxyModel);
  connect(m_mangaListView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](QModelIndex const& p_index) {
    if (p_index.isValid()) {
      m_chapterListWidget->changeManga(p_index);
      m_mangaListView->setFocus();
    }
  });
  m_mangaListView->setItemDelegate(new MangaListDelegate);
  m_mangaListView->setEditTriggers(QListView::NoEditTriggers);

  /// Search line edit
  m_searchLineEdit = new QLineEdit;
  m_searchLineEdit->setPlaceholderText("Search...");
  m_searchLineEdit->setClearButtonEnabled(true);
  connect(m_searchLineEdit, &QLineEdit::textChanged, m_mangaProxyModel, &QSortFilterProxyModel::setFilterFixedString);

  /// Manga list sub widget
  auto mangaListSubWidget = new QWidget;
  mangaListSubWidget->setObjectName("mangaListSubWidget");
  auto searchLayout = new QHBoxLayout;
  searchLayout->addWidget(m_checkNewChaptersButton);
  searchLayout->addWidget(m_searchLineEdit);
  auto mangaListLayout = new QVBoxLayout;
  mangaListLayout->addLayout(searchLayout);
  mangaListLayout->addWidget(m_mangaListView);
  mangaListSubWidget->setLayout(mangaListLayout);

  /// Chapter List Widget
  m_chapterListWidget = new ChapterListWidget;

  /// Main layout
  m_mainSplitter = new QSplitter;
  m_mainSplitter->setFrameStyle(1);
  m_mainSplitter->addWidget(mangaListSubWidget);
  m_mainSplitter->addWidget(m_chapterListWidget);
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_mainSplitter);
  m_mainSplitter->setStretchFactor(0, 1);
  m_mainSplitter->setStretchFactor(1, 2);
  mainLayout->setSpacing(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  setLayout(mainLayout);

  /// Connect manga and chapter selections
  connect(m_chapterListWidget, &ChapterListWidget::chapterSelected, this, &MangaListWidget::goToRead);
  connect(m_chapterListWidget, &ChapterListWidget::downloadRequested, this, &MangaListWidget::goToDownload);
  connect(m_chapterListWidget, &ChapterListWidget::progressionChanged, this, &MangaListWidget::updateCurrentProgression);

  /// Schedule a new available manga check every thirty minutes
  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(checkAvailableDownloads()));
  timer->start(1000*60*30);

  /// Read style from css file
  QFile file("../MangaReaderForLinux/css/MangaListWidget.css");
  file.open(QFile::ReadOnly);
  QString newStyleSheet = QLatin1String(file.readAll());
  setStyleSheet(styleSheet() + newStyleSheet);

  /// Select first manga in list
  m_mangaListView->setCurrentIndex(m_mangaProxyModel->index(0, 0));

  /// Connections
  connect(this, &MangaListWidget::currentChapterChanged, m_chapterListWidget, &ChapterListWidget::markChapterAsRead);
}


/// PUBLIC SLOTS

void MangaListWidget::initModel(QString p_mangaSelected) {
  /// Get every manga name
  m_scansDirectory.setFilter(QDir::Dirs);
  QStringList dirsList = Utils::dirList(m_scansDirectory);

  /// Clear model
  m_mangaModel->setRowCount(0);

  /// List every manga
  QModelIndex indexMangaSelected;
  for (const QString& mangaName: dirsList) {
    QStandardItem* currItem = new QStandardItem(mangaName);
    currItem->setData(QColor(Qt::white), Qt::ForegroundRole);
    m_mangaModel->appendRow(currItem);
    updateMangaData(currItem);

    if (mangaName == p_mangaSelected) {
      indexMangaSelected = m_mangaModel->indexFromItem(currItem);
    }
  }

  /// Set right color according to manga location in list
  for (int k = 0; k < m_mangaModel->rowCount(); ++k) {
    auto currItem = m_mangaModel->item(k, 0);
    auto hue = k*(360/m_mangaModel->rowCount());
    auto satValue = 256-96;
    currItem->setData(QColor::fromHsv(hue, satValue, satValue), MangaListDelegate::eColorRole);
  }

  /// Select current manga
  if (indexMangaSelected.isValid())
    m_mangaListView->setCurrentIndex(indexMangaSelected);

  checkAvailableDownloads();
}

void MangaListWidget::addManga() {
  AddMangaDialog* addMangadialog = new AddMangaDialog(this);
  switch(addMangadialog->exec()) {
  case QDialog::Accepted:
    initModel();
    break;
  case QDialog::Rejected:
    break;
  }
}


/// PROTECTED

void MangaListWidget::setRemainingChaptersToRead(QStandardItem* p_mangaItem, int p_remainingChaptersCount, int p_chaptersCount) {
  p_mangaItem->setData(p_remainingChaptersCount, MangaListDelegate::eChaptersToReadRole);
  p_mangaItem->setData(p_chaptersCount, MangaListDelegate::eChaptersCountRole);
}

void MangaListWidget::updateMangaData(QStandardItem* p_mangaItem) {
  QList<bool> areChaptersRead = Utils::areChaptersRead(p_mangaItem->text());
  setRemainingChaptersToRead(p_mangaItem, Utils::countChaptersToRead(areChaptersRead), areChaptersRead.size());
}

void MangaListWidget::updateReadProgression(QModelIndex const& p_index) {
  auto remainingChaptersCount = p_index.data(MangaListDelegate::eChaptersToReadRole).toInt();
  auto chaptersCount = p_index.data(MangaListDelegate::eChaptersCountRole).toInt();
  m_chapterListWidget->setReadPercentage(chaptersCount - remainingChaptersCount, chaptersCount);
}

void MangaListWidget::startNextCheck() {
  m_currentChaptersListOnWeb.clear();
  m_currentIndex = QModelIndex();

  if (m_chaptersToCheck.isEmpty()) {
    m_checkNewChaptersButton->setEnabled(true);
    return;
  }

  m_currentIndex = m_chaptersToCheck.dequeue();
  QStringList arguments;
  QStandardItem* currentMangaItem = m_mangaModel->itemFromIndex(m_currentIndex);
  currentMangaItem->setData(QColor("#286090"), Qt::ForegroundRole);
  arguments << currentMangaItem->text();
  m_checkAvailableChaptersProcess->start(Utils::getScriptsAbsolutePath()+"/updateChaptersList.sh", arguments);
}

void MangaListWidget::setDownloadButtonDisabled(bool b) {
//  m_downloadButton->setDisabled(b);
}


/// PROTECTED SLOTS

void MangaListWidget::checkAvailableDownloads() {
  m_chaptersToCheck.clear();
  m_checkNewChaptersButton->setEnabled(false);

  for (int k = 0; k < m_mangaModel->rowCount(); ++k) {
    m_chaptersToCheck.enqueue(m_mangaModel->item(k)->index());
  }

  startNextCheck();
}

void MangaListWidget::goToRead(QModelIndex const& p_modelIndex) {
  auto chapterName = p_modelIndex.data().toString();
  auto currentMangaIndex = m_mangaProxyModel->mapToSource(m_mangaListView->currentIndex());
  auto mangaName = currentMangaIndex.data().toString();

  if (chapterName.isEmpty() || mangaName.isEmpty())
    return;

  emit chapterSelected(mangaName, chapterName);
  updateReadProgression(currentMangaIndex);
}

void MangaListWidget::goToDownload() {
  QStandardItem* currentItem = m_mangaModel->itemFromIndex(m_mangaProxyModel->mapToSource(m_mangaListView->currentIndex()));

  if (!currentItem) {
    QMessageBox::warning(this, "Warning", "No manga selected.");
    return;
  }

  QString mangaName;
  currentItem->parent() == nullptr ? mangaName = currentItem->text() : mangaName = currentItem->parent()->text();

  emit mangaSelected(mangaName);
}

void MangaListWidget::checkAvailableChapterIsDone(int, QProcess::ExitStatus exitStatus) {
  m_mangaModel->itemFromIndex(m_currentIndex)->setData(QColor(Qt::white), Qt::ForegroundRole);

  switch (exitStatus) {
  case QProcess::CrashExit: {
    break;
  }
  case QProcess::NormalExit: {
    bool isNotUpToDate = false;
    int newChaptersAvailableCount = 0;

    QString currChapterDirStr = m_scansDirectory.path() + "/" + m_mangaModel->itemFromIndex(m_currentIndex)->text();
    QDir currentChaptersDir = QDir(currChapterDirStr);
    QStringList chaptersListOnPC = Utils::dirList(currentChaptersDir);

    QStringList chaptersUrlAndTitleOnWebList = m_currentChaptersListOnWeb.split("\n", QString::SkipEmptyParts);
    for (const QString& titleAndUrl: chaptersUrlAndTitleOnWebList) {
      QString tempUrl = titleAndUrl.split(";").first();
      tempUrl.trimmed();
      if (tempUrl.endsWith('/')) {
        tempUrl.truncate(tempUrl.length()-1);
      }
      QString currentTitle = tempUrl.split('/').last();
      if (!chaptersListOnPC.contains(currentTitle)) {
        isNotUpToDate = true;
        ++newChaptersAvailableCount;
      }
    }

    m_mangaModel->itemFromIndex(m_currentIndex)->setData(newChaptersAvailableCount, MangaListDelegate::eAvailableChaptersRole);
    if (isNotUpToDate) {
      m_mangaModel->itemFromIndex(m_currentIndex)->setToolTip(QString("New chapters available (%1)").arg(newChaptersAvailableCount));
      if (m_currentIndex == m_mangaProxyModel->mapToSource(m_mangaListView->currentIndex()))
      {
        m_chapterListWidget->setAvailableDownloadCount(m_currentIndex);
      }
    }
    break;
  }
  }

  startNextCheck();
}

void MangaListWidget::readStandardOutput() {
  m_currentChaptersListOnWeb += m_checkAvailableChaptersProcess->readAllStandardOutput();
}

void MangaListWidget::updateCurrentProgression(int p_currentRemainingToRead) {
  auto currentItem = m_mangaModel->itemFromIndex(m_mangaProxyModel->mapToSource(m_mangaListView->currentIndex()));
  currentItem->setData(p_currentRemainingToRead, MangaListDelegate::eChaptersToReadRole);
}
