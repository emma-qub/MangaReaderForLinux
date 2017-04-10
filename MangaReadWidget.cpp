#include "MangaReadWidget.h"
#include "Utils.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QScrollBar>
#include <QKeyEvent>

MangaReadWidget::MangaReadWidget(QWidget* p_parent) :
  QWidget(p_parent),
  m_scansDirectory(Utils::getScansDirectory()),
  m_currentMangaDirectory(),
  m_currentChapterDirectory(),
  m_chapterStringList(),
  m_currentPageNumber(-1),
  m_nbPagesCurrCh(-1),
  m_mousePosition() {

  QFont buttonIconFont("FontAwesome", 12);

  /// Select chapter
  m_chaptersComboBox = new QComboBox;
  m_chaptersComboBox->setFixedWidth(300);
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString const&)), this, SLOT(changeChapter(QString const&)));

  /// Select page
  m_nbPagesLabel = new QLabel;

  /// Zoom
  m_zoomButton = new QPushButton("\uf00e");
  m_zoomButton->setFont(buttonIconFont);
  m_zoomButton->setCheckable(true);
  m_zoomButton->setChecked(true);
  m_zoomButton->setFixedWidth(37);

  /// Go to Read
  m_goToListButton = new QPushButton("\uf060");
  m_goToListButton->setFont(buttonIconFont);
  m_goToListButton->setFixedWidth(37);
  connect(m_goToListButton, &QPushButton::clicked, this, &MangaReadWidget::goToListRequested);

  /// Manga form layout
  QHBoxLayout* chooseMangaLayout = new QHBoxLayout;
  chooseMangaLayout->addWidget(m_goToListButton);
  chooseMangaLayout->addWidget(m_zoomButton);
  chooseMangaLayout->addWidget(m_chaptersComboBox);
  chooseMangaLayout->addWidget(m_nbPagesLabel);

  /// Page
  m_currentPageLabel = new QLabel;
  m_currentPageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_scrollArea = new QScrollArea;
  m_scrollArea->setBackgroundRole(QPalette::Background);
  m_scrollArea->setWidget(m_currentPageLabel);
  m_scrollArea->setWidgetResizable(true);
  m_scrollArea->setAlignment(Qt::AlignHCenter);
  m_zoomLabel = new QLabel(m_scrollArea);
  m_zoomLabel->hide();

  /// Main layout
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(chooseMangaLayout);
  mainLayout->addWidget(m_scrollArea);

  setLayout(mainLayout);
}

void MangaReadWidget::updateChaptersComboBox(const QString& p_mangaName) {
  disconnect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  m_chaptersComboBox->clear();
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));

  if (p_mangaName.isEmpty())
    return;

  m_currentMangaDirectory = QDir(m_scansDirectory.path()+"/"+p_mangaName);

  QStringList chaptersList = Utils::dirList(m_currentMangaDirectory, true);

  if (chaptersList.size() == 0) {
    changeChapter("");
    return;
  }

  disconnect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  m_chaptersComboBox->addItems(chaptersList);
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
}

void MangaReadWidget::changeChapter(const QString& p_chapterName) {
  if (p_chapterName.isEmpty()) {
    m_currentPageLabel->setPixmap(QPixmap());
    updatePagesComboBox();
    return;
  }

  m_currentChapterDirectory = QDir(m_currentMangaDirectory.path() + "/" + p_chapterName);
  m_currentPageNumber = 0;
  updatePagesComboBox();
  emit currentChapterChanged(p_chapterName);

  m_currentPageLabel->setFocus();
}

void MangaReadWidget::updatePagesComboBox() {
  m_nbPagesLabel->setText("/");

  QStringList nameFilters;
  nameFilters << "*.png" << "*.jpg" << "*.gif" << "*.jpeg" << "*.bmp";
  m_chapterStringList = Utils::filesList(m_currentChapterDirectory);
  m_nbPagesCurrCh = m_chapterStringList.size();

  updateCurrentPage();
}

void MangaReadWidget::changePage(int p_pageNumber) {
  if (p_pageNumber > -1 && p_pageNumber < m_nbPagesCurrCh) {
    m_currentPageNumber = p_pageNumber;
    updateCurrentPage();
  }
}

void MangaReadWidget::updateCurrentPage() {
  m_nbPagesLabel->setText(QString::number(m_currentPageNumber+1)+"/"+QString::number(m_nbPagesCurrCh));

  if (m_currentPageNumber >= m_nbPagesCurrCh) {
    QMessageBox::critical(this, "Page number error", "Error: cannot load page number " + QString::number(m_currentPageNumber+1) + " because chapter counts only " + m_chapterStringList.size() + " pages.");
    return;
  }
  m_currentPageLabel->setPixmap(QPixmap(m_currentChapterDirectory.path()+"/"+m_chapterStringList.at(m_currentPageNumber)));
  m_scrollArea->verticalScrollBar()->setValue(0);
}

void MangaReadWidget::switchManga(const QString& p_mangaName, const QString& p_chapterName) {
  updateChaptersComboBox(p_mangaName);

  disconnect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  m_chaptersComboBox->setCurrentText(p_chapterName);
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));

  changeChapter(p_chapterName);
}

void MangaReadWidget::keyReleaseEvent(QKeyEvent* p_event) {
  if (m_chaptersComboBox->currentText().isEmpty())
    return;

  switch (p_event->key()) {
  case Qt::Key_Right:
    if (p_event->modifiers() & Qt::ControlModifier) {
      if (m_chaptersComboBox->currentIndex() == 0)
        return;
      m_chaptersComboBox->setCurrentIndex(m_chaptersComboBox->currentIndex()-1);
    } else if (p_event->modifiers() & Qt::ShiftModifier) {
      return;
    } else {
      if (m_currentPageNumber == m_nbPagesCurrCh-1)
        return;
      changePage(m_currentPageNumber+1);
    }
    break;
  case Qt::Key_Left:
    if (p_event->modifiers() & Qt::ControlModifier) {
      if (m_chaptersComboBox->currentIndex() == m_chaptersComboBox->count()-1)
        return;
      m_chaptersComboBox->setCurrentIndex(m_chaptersComboBox->currentIndex()+1);
    } else if (p_event->modifiers() & Qt::ShiftModifier) {
      return;
    } else {
      if (m_currentPageNumber == 0)
        return;
      changePage(m_currentPageNumber-1);
    }
    break;
  default:
    return;
  }
}

void MangaReadWidget::mouseReleaseEvent(QMouseEvent* p_event) {
  Q_UNUSED(p_event)
  m_zoomLabel->clear();
  m_zoomLabel->hide();
}

void MangaReadWidget::mouseMoveEvent(QMouseEvent* p_event) {
  m_zoomLabel->setVisible(m_zoomButton->isChecked() && p_event->button() == Qt::NoButton);


  if (m_zoomButton->isChecked()) {
    QPoint mousePosition = p_event->pos();
    // In order not to zoom outside of the box and to prevent from zooming when no page is displayed
    if (mousePosition.x() < 0 || mousePosition.y() < 0 || m_chaptersComboBox->currentIndex() < 1)
      return;

    int frameWidth = 400;
    int frameHeight = 300;
    int ratio = 2;
    int frameWidthRatio = qRound(static_cast<double>(frameWidth)/static_cast<double>(ratio));
    int frameHeightRatio = qRound(static_cast<double>(frameHeight)/static_cast<double>(ratio));

    mousePosition.rx() -= m_scrollArea->geometry().x();
    mousePosition.ry() -= m_scrollArea->geometry().y();

    int topShift = 0;
    if (m_currentPageLabel->visibleRegion().boundingRect().top()+mousePosition.y() > m_currentPageLabel->visibleRegion().boundingRect().bottom() - frameHeight)
      topShift = frameHeight;

    QPoint rawTopLeft = mousePosition - m_currentPageLabel->geometry().topLeft();
    QPoint topLeft(qMin(m_currentPageLabel->width()-frameWidthRatio, rawTopLeft.x()), rawTopLeft.y()-topShift/ratio);
    QPoint bottomRight = topLeft+QPoint(frameWidthRatio, frameHeightRatio);

    if (mousePosition.x() < m_currentPageLabel->geometry().left()
        || mousePosition.x() > m_currentPageLabel->geometry().right()
        || mousePosition.y() < m_currentPageLabel->geometry().top()
        || mousePosition.y() > m_currentPageLabel->geometry().bottom())
      m_zoomLabel->hide();

    QRect frame(topLeft, bottomRight);

    QPixmap zoomedPixmap = m_currentPageLabel->pixmap()->copy(frame).scaled(frameWidth, frameHeight);
    m_zoomLabel->setPixmap(zoomedPixmap);

    m_zoomLabel->move(mousePosition+QPoint(0, -topShift));
    m_zoomLabel->resize(frameWidth, frameHeight);
  }
}
