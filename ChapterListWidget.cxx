#include "ChapterListWidget.hxx"
#include "MangaListDelegate.hxx"

#include <QVBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QLabel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QMessageBox>
#include <QHeaderView>

FrontCoverOverlay::FrontCoverOverlay(QWidget* p_parent):
  QWidget(p_parent),
  m_availableDownloadCount(0),
  m_percentageRead(0),
  m_enteredButton(false),
  m_hasToRepaint(false),
  m_textButtonColor(QColor::fromRgb(0x25, 0x28, 0x38)) {

  auto mainLayout = new QVBoxLayout;
  mainLayout->setSpacing(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  setFixedHeight(300);

  setMouseTracking(true);
}

void FrontCoverOverlay::setFrontCover(QPixmap const& p_frontCover) {
  if (!p_frontCover.isNull()) {
    m_frontCover = p_frontCover.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation).copy(0, 50, width(), 300);
  } else {
    m_frontCover = QPixmap(width(), 300);
    m_frontCover.fill(Qt::white);
  }
  repaint();
}

void FrontCoverOverlay::setMangaName(QString const& p_mangaName) {
  m_mangaName = p_mangaName;
  repaint();
}

void FrontCoverOverlay::setAvailableDownloadCount(int p_availableDownloadCount) {
  m_availableDownloadCount = p_availableDownloadCount;
  repaint();
}

void FrontCoverOverlay::setReadProgression(float p_percentageRead) {
  m_percentageRead = p_percentageRead;
  repaint();
}

void FrontCoverOverlay::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  QRect drawingRect(0, 0, width(), height());
  painter.drawPixmap(drawingRect, m_frontCover);
  painter.fillRect(drawingRect, QColor::fromRgb(0, 0, 0, 128));
  auto frontCoverFont = QApplication::font();
  frontCoverFont.setWeight(99);
  frontCoverFont.setPixelSize(50);
  painter.setFont(QFont(frontCoverFont));
  painter.setPen(QPen(Qt::white));
  painter.drawText(drawingRect, m_mangaName, QTextOption(Qt::AlignCenter));

  if (m_availableDownloadCount > 0) {
    /// Font
    QFont font;
    font.setFamily("FontAwesome");
    font.setPixelSize(18);
    font.setWeight(99);
    /// Text
    QString downloadText("Update ("+QString::number(m_availableDownloadCount)+") \uf019");
    /// Sizes
    QFontMetrics fm(font);
    int downloadTextWidth = fm.width(downloadText)+60;
    int downloadTextHeight = fm.height()+25;
    m_buttonRect = QRect(width()/2 - downloadTextWidth/2, height()/2+30, downloadTextWidth, downloadTextHeight);
    /// Draw button
    painter.setBrush(QBrush(Qt::white));
    painter.drawRoundedRect(m_buttonRect, 5, 5);
    /// Draw text
    painter.setPen(QPen(m_textButtonColor));
    painter.setFont(font);
    painter.drawText(m_buttonRect, downloadText, QTextOption(Qt::AlignCenter));
  } else {
    m_buttonRect = QRect();
  }

  /// Draw read progression
  auto progressionWidth = (width() * m_percentageRead) / 100;
  auto progressionHeight = 5;
  QRect progresionRect(drawingRect.left(), drawingRect.bottom()-progressionHeight, progressionWidth, progressionHeight);
  auto greenColor = QColor::fromRgb(0x5c, 0xb8, 0x5c);
  painter.setBrush(greenColor);
  painter.setPen(QPen(greenColor));
  painter.drawRect(progresionRect);
}

void FrontCoverOverlay::mousePressEvent(QMouseEvent* p_event) {
  if (m_buttonRect.contains(p_event->pos()) && p_event->button() == Qt::LeftButton) {
    emit downloadRequested();
  }
}

void FrontCoverOverlay::mouseMoveEvent(QMouseEvent* p_event) {
  if (m_enteredButton != m_buttonRect.contains(p_event->pos())) {
    m_enteredButton = m_buttonRect.contains(p_event->pos());
    m_hasToRepaint = true;
  }

  if (m_hasToRepaint) {
    if (m_enteredButton) {
      m_textButtonColor = QColor::fromRgb(0x5c, 0xb8, 0x5c);
    } else {
      m_textButtonColor = QColor::fromRgb(0x25, 0x28, 0x38);
    }
    repaint();
    m_hasToRepaint = false;
  }
}






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



  /// Progressbar style
  //  QString progressBarStyle(
  //  "QProgressBar {"
  //  "    border: 1px solid lightgrey;"
  //  "    border-radius: 5px;"
  //  "    text-align: center;"
  //  "    font-weight: bold;"
  //  "}"
  //  "QProgressBar::chunk {"
  //  "    background-color: #449D44;"
  //  "    width: 10px;"
  ////  "    margin: 0.5px;"
  //  "}"
}

void ChapterListWidget::setReadPercentage(int p_chaptersReadCount, int p_allChaptersCount) {
  if (p_allChaptersCount == 0)
  {
    return;
  }
  float readPercentage = (100.0 * static_cast<float>(p_chaptersReadCount) / static_cast<float>(p_allChaptersCount));
  m_frontCover->setReadProgression(readPercentage);
  emit progressionChanged(m_allChaptersCount - m_chaptersReadCount);
}

void ChapterListWidget::setAvailableDownloadCount(QModelIndex const& p_index) {
  m_frontCover->setAvailableDownloadCount(p_index.data(MangaListDelegate::eAvailableChaptersRole).toInt());
}

void ChapterListWidget::changeManga(QModelIndex const& p_index)
{
  m_chaptersModel->setRowCount(0);
  m_chaptersReadCount = 0;

  m_currentMangaName = p_index.data().toString();
  QString currDirStr = Utils::getScansDirectory().path() + "/" + m_currentMangaName;

  QDir currDir(currDirStr);
  QStringList currDirsList = Utils::dirList(currDir, true);

  auto frontCover = QPixmap(currDirStr+"/frontCover.jpg");
  m_frontCover->setMangaName(m_currentMangaName);
  m_frontCover->setFrontCover(frontCover);
  setAvailableDownloadCount(p_index);

  QList<bool> areChaptersRead = Utils::areChaptersRead(m_currentMangaName);

  int k = 0;
  for (const QString& currChStr: currDirsList) {
    if (k >= areChaptersRead.size()) {
      QMessageBox::critical(this, "List error", "Error while tempting to edit manga read flags whithin ChapterListWidget::changeManga.");
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

void ChapterListWidget::markChapterAsRead(const QString& p_chapterName)
{
  auto possibleChapters = m_chaptersModel->findItems(p_chapterName);
  if (possibleChapters.size() != 1) {
    return;
  }

  auto currentChapterItem = possibleChapters.at(0);
  m_chaptersView->setCurrentIndex(currentChapterItem->index());
  updateReadState(currentChapterItem, true);
}

void ChapterListWidget::updateReadState(QStandardItem* p_stateItem, bool p_isChapterRead) {
  m_chaptersView->setFocus();

  /// Get items
  auto currentRow = p_stateItem->row();
  auto chapterTextItem = m_chaptersModel->item(currentRow, eChapterNameColumn);
  auto chapterReadItem = m_chaptersModel->item(currentRow, eChapterReadColumn);

  /// Fonts
  QFont font;
  font.setFamily("FontAwesome");
  font.setPixelSize(16);
  QColor readIconColor;
  auto updatedFont = chapterTextItem->font();

  /// Update state according to read or not
  chapterReadItem->setData(p_isChapterRead, eChapterReadRole);
  if (p_isChapterRead) {
    updatedFont.setBold(false);
    readIconColor = QColor::fromRgb(0x5c, 0xb8, 0x5c);
    chapterReadItem->setText("\uf06e");
  } else {
    updatedFont.setBold(true);
    readIconColor = QColor::fromRgb(0x25, 0x28, 0x38);
    chapterReadItem->setText("\uf070");
  }
  chapterTextItem->setFont(updatedFont);
  chapterReadItem->setFont(font);
  chapterReadItem->setData(readIconColor, Qt::ForegroundRole);

  /// Update db
  Utils::updateChapterRead(m_currentMangaName, chapterTextItem->text(), p_isChapterRead);
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
