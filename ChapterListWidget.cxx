#include <QStringList>
#include <QDir>
#include <QMessageBox>
#include <QHeaderView>
#include <QKeyEvent>

#include "ChapterListWidget.hxx"
#include "MangaListDelegate.hxx"

ChapterListWidget::ChapterListWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_chaptersReadCount(0),
  m_allChaptersCount(0) {

  QFont titleFont("FontAwesome", 14, 99);
  m_chaptersTitleLabel = new QLabel;
  m_chaptersTitleLabel->setFont(titleFont);
  m_chaptersTitleLabel->setText("\uf0ca Chapters");
  m_chaptersTitleLabel->setAlignment(Qt::AlignVCenter);
  m_chaptersTitleLabel->setFixedHeight(80);
  m_chaptersTitleLabel->setContentsMargins(25, 20, 0, 0);
  m_frontCover = new FrontCoverOverlay;
  m_chaptersModel = new QStandardItemModel;
  m_chaptersView = new QTreeView;
  m_chaptersView->setModel(m_chaptersModel);
  m_chaptersView->setAlternatingRowColors(true);

  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_frontCover);
  mainLayout->addWidget(m_chaptersTitleLabel);
  mainLayout->addWidget(m_chaptersView);
  mainLayout->setSpacing(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  connect(m_chaptersView, &QTreeView::doubleClicked, this, [this](QModelIndex const& p_index) {
    emit chapterSelected(getChapterIndex(p_index));
    updateReadState(m_chaptersModel->item(p_index.row(), eChapterReadColumn), true);
    ++m_chaptersReadCount;
    setReadPercentage(m_chaptersReadCount, m_allChaptersCount);
  });
  connect(m_chaptersView, &QTreeView::clicked, this,  [this](QModelIndex const& p_index) {
    if (p_index.column() == eChapterReadColumn) {
      bool newChapterReadState = !p_index.data(eChapterReadRole).toBool();
      auto chapterIndex = m_chaptersModel->itemFromIndex(p_index);
      chapterIndex->setData(newChapterReadState, eChapterReadRole);
      updateReadState(chapterIndex, newChapterReadState);
      newChapterReadState ? ++m_chaptersReadCount : --m_chaptersReadCount;
      setReadPercentage(m_chaptersReadCount, m_allChaptersCount);
    }
  });

  connect(m_frontCover, &FrontCoverOverlay::downloadRequested, this, &ChapterListWidget::downloadRequested);

  setLayout(mainLayout);
}

void ChapterListWidget::setReadPercentage(int p_chaptersReadCount, int p_allChaptersCount) {
  float readPercentage = (100.0 * static_cast<float>(p_chaptersReadCount) / static_cast<float>(p_allChaptersCount));
  m_frontCover->setReadProgression(readPercentage);
}

void ChapterListWidget::setAvailableDownloadCount(QModelIndex const& p_index) {
  m_frontCover->setAvailableDownloadCount(p_index.data(MangaListDelegate::eAvailableChaptersRole).toInt());
}

void ChapterListWidget::changeManga(QModelIndex const& p_index)
{
  m_chaptersModel->setRowCount(0);
  m_chaptersReadCount = 0;

  auto mangaName = p_index.data().toString();
  QString currDirStr = Utils::getScansDirectory().path() + "/" + mangaName;

  QDir currDir(currDirStr);
  QStringList currDirsList = Utils::dirList(currDir, true);

  auto frontCover = QPixmap(currDirStr+"/frontCover.jpg");
  m_frontCover->setMangaName(mangaName);
  m_frontCover->setFrontCover(frontCover);
  setAvailableDownloadCount(p_index);

  QList<bool> areChaptersRead = Utils::areChaptersRead(mangaName);

  int k = 0;
  for (const QString& currChStr: currDirsList) {
    if (k >= areChaptersRead.size()) {
      QMessageBox::critical(this, "List error", "Error while tempting to edit manga read flags whithin MangaListWidget::initModel.");
      return;
    }
    bool isChapterRead = areChaptersRead.at(k);

    QStandardItem* currChItem = new QStandardItem(currChStr);
    currChItem->setEditable(false);

    auto stateItem = new QStandardItem;
    m_chaptersModel->appendRow({currChItem, stateItem});
    updateReadState(stateItem, isChapterRead);
    if (isChapterRead) {
      ++m_chaptersReadCount;
    }

    ++k;
  }
  m_allChaptersCount = m_chaptersModel->rowCount();
  setReadPercentage(m_chaptersReadCount, m_allChaptersCount);

  m_chaptersView->header()->setSectionResizeMode(eChapterNameColumn, QHeaderView::Stretch);
  m_chaptersView->header()->setSectionResizeMode(eChapterReadColumn, QHeaderView::Fixed);
  m_chaptersView->header()->resizeSection(eChapterReadColumn, 30);
  m_chaptersView->header()->setStretchLastSection(false);
  m_chaptersView->header()->hide();
}

void ChapterListWidget::updateReadState(QStandardItem* p_stateItem, bool p_isChapterRead) {
  QFont font;
  font.setFamily("FontAwesome");
  font.setPixelSize(16);
  QColor readIconColor;
  p_stateItem->setData(p_isChapterRead, eChapterReadRole);
  if (p_isChapterRead) {
    readIconColor = QColor::fromRgb(0x5c, 0xb8, 0x5c);
  } else {
    readIconColor = QColor::fromRgb(0x25, 0x28, 0x38);
  }
  p_stateItem->setData(readIconColor, Qt::ForegroundRole);
  p_stateItem->setFont(font);
  p_stateItem->setText("\uf06e");
}

QModelIndex ChapterListWidget::getChapterIndex(QModelIndex const& p_index) {
  return p_index.sibling(p_index.row(), 0);
}

void ChapterListWidget::keyReleaseEvent(QKeyEvent* p_event) {
  // Enter released
  if (p_event->key() == Qt::Key_Return || p_event->key() == Qt::Key_Enter) {
    auto currentIndex = m_chaptersView->currentIndex();
    emit chapterSelected(getChapterIndex(currentIndex));
    updateReadState(m_chaptersModel->item(currentIndex.row(), eChapterReadColumn), true);
    ++m_chaptersReadCount;
    setReadPercentage(m_chaptersReadCount, m_allChaptersCount);
  }
}
