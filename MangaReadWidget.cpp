#include "MangaReadWidget.h"
#include "Utils.h"

MangaReadWidget::MangaReadWidget(QWidget* parent) :
  QWidget(parent),
  m_scansDirectory(Utils::getScansDirectory()),
  m_currentMangaDirectory(),
  m_currentChapterDirectory(),
  m_chapterStringList(),
  m_currentPageNumber(-1),
  m_nbPagesCurrCh(-1),
  m_mousePosition() {

  QStringList mangaList = Utils::dirList(m_scansDirectory);


  /// Select manga

  QLabel* selectMangaLabel = new QLabel("Select your manga:");

  m_selectLineEdit = new QLineEdit;
  QCompleter* completer = new QCompleter(mangaList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setCompletionMode(QCompleter::PopupCompletion);
  m_selectLineEdit->setCompleter(completer);

  m_mangasComboBox = new QComboBox;
  m_mangasComboBox->addItems(mangaList);
  m_mangasComboBox->setFixedWidth(250);
  m_mangasComboBox->setLineEdit(m_selectLineEdit);

  connect(m_mangasComboBox, SIGNAL(activated(QString)), this, SLOT(updateChaptersComboBox(QString)));
  connect(m_mangasComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateChaptersComboBox(QString)));


  /// Select chapter

  m_chaptersComboBox = new QComboBox;
  m_chaptersComboBox->setFixedWidth(300);
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));


  /// Select page

  m_nbPagesLabel = new QLabel;

  m_pagesComboBox = new QComboBox;
  m_pagesComboBox->setFixedWidth(80);
  connect(m_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));


  /// Select first manga in list if any

  if (m_mangasComboBox->count() > 0) {
    updateChaptersComboBox(m_mangasComboBox->itemText(0));
  }


  /// Zoom

  m_zoomButton = new QPushButton;
  m_zoomButton->setIcon(QIcon(Utils::getIconsPath()+"/zoom.png"));
  m_zoomButton->setCheckable(true);
  m_zoomButton->setChecked(true);
  m_zoomButton->setFixedWidth(37);

  m_zoomComboBox = new QComboBox;
  QStringList zoomValues;
  zoomValues << QString::number(2) << QString::number(4) << QString::number(8);
  m_zoomComboBox->addItems(zoomValues);
  m_zoomComboBox->setCurrentIndex(0);
  m_zoomComboBox->setFixedWidth(50);


  /// Manga form layout

  QHBoxLayout* chooseMangaLayout = new QHBoxLayout;
  chooseMangaLayout->addWidget(selectMangaLabel);
  chooseMangaLayout->addWidget(m_mangasComboBox);
  chooseMangaLayout->addWidget(m_chaptersComboBox);
  chooseMangaLayout->addWidget(m_pagesComboBox);
  chooseMangaLayout->addWidget(m_nbPagesLabel);
  chooseMangaLayout->addWidget(m_zoomButton);
  chooseMangaLayout->addWidget(m_zoomComboBox);
  chooseMangaLayout->addSpacing(500);


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

  QLabel* titleLabel = new QLabel("Manga Read");
  titleLabel->setFont(QFont("", 18, 99));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  mainLayout->addLayout(chooseMangaLayout);
  mainLayout->addWidget(m_scrollArea);

  setLayout(mainLayout);
}

void MangaReadWidget::updateChaptersComboBox(QString mangaName) {
  disconnect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  m_chaptersComboBox->clear();
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));

  if (mangaName.isEmpty())
    return;

  m_currentMangaDirectory = QDir(m_scansDirectory.path()+"/"+mangaName);

  QStringList chaptersList = Utils::dirList(m_currentMangaDirectory, true);

  if (chaptersList.size() == 0) {
    changeChapter("");
    return;
  }

  disconnect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  m_chaptersComboBox->addItems(chaptersList);
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
}

void MangaReadWidget::changeChapter(QString chapterName) {
  if (chapterName.isEmpty()) {
    m_currentPageLabel->setPixmap(QPixmap());
    updatePagesComboBox();
    return;
  }

  m_currentChapterDirectory = QDir(m_currentMangaDirectory.path() + "/" + chapterName);
  m_currentPageNumber = 0;
  updatePagesComboBox();
  Utils::updateChapterRead(m_mangasComboBox->currentText(), m_chaptersComboBox->currentText(), true);

  emit chapterSelected(m_mangasComboBox->currentText(), m_chaptersComboBox->currentText());
}

void MangaReadWidget::updatePagesComboBox() {
  disconnect(m_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));
  m_pagesComboBox->clear();
  connect(m_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));

  m_nbPagesLabel->setText("/");

  if (m_mangasComboBox->currentText().isEmpty() || m_chaptersComboBox->currentText().isEmpty())
    return;

  QStringList nameFilters;
  nameFilters << "*.png" << "*.jpg" << "*.gif" << "*.jpeg" << "*.bmp";
  m_chapterStringList = Utils::filesList(m_currentChapterDirectory);
  m_nbPagesCurrCh = m_chapterStringList.size();

  disconnect(m_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));
  for (int k = 1; k <= m_nbPagesCurrCh; k++) {
    m_pagesComboBox->addItem(QString::number(k));
  }
  connect(m_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));

  m_nbPagesLabel->setText("/"+QString::number(m_nbPagesCurrCh));
  updateCurrentPage();
}

void MangaReadWidget::changePage(int pageNumber) {
  if (pageNumber > -1 && pageNumber < m_nbPagesCurrCh) {
    m_currentPageNumber = pageNumber;
    updateCurrentPage();
  }
}

void MangaReadWidget::updateCurrentPage() {
  if (m_currentPageNumber >= m_nbPagesCurrCh) {
    QMessageBox::critical(this, "Page number error", "Error: cannot load page number " + QString::number(m_currentPageNumber+1) + " because chapter counts only " + m_chapterStringList.size() + " pages.");
    return;
  }
  m_currentPageLabel->setPixmap(QPixmap(m_currentChapterDirectory.path()+"/"+m_chapterStringList.at(m_currentPageNumber)));
  m_scrollArea->verticalScrollBar()->setValue(0);
}

void MangaReadWidget::switchManga(QString mangaName, QString chapterName) {
  m_mangasComboBox->setCurrentText(mangaName);
  disconnect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  m_chaptersComboBox->setCurrentText(chapterName);
  connect(m_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));

  /// Force to change chapter since there are messed up signals when switching from read/download tabs
  changeChapter(chapterName);

  /// Set focus to the page in order to let the user immediately read the chapter.
  /// Otherwise, the mangas combo box has the focus and using the arrows
  /// would end up updating the current manga.
  m_currentPageLabel->setFocus();

  /// For some reason, the previous setFocus mess up with the combo box.
  if (m_chaptersComboBox->currentText() != chapterName) {
    m_chaptersComboBox->setCurrentText(chapterName);
  }
}

void MangaReadWidget::keyReleaseEvent(QKeyEvent* event) {
  if (m_chaptersComboBox->currentText().isEmpty())
    return;

  switch (event->key()) {
  case Qt::Key_Right:
    if (event->modifiers() & Qt::ControlModifier) {
      if (m_chaptersComboBox->currentIndex() == 0)
        return;
      m_chaptersComboBox->setCurrentIndex(m_chaptersComboBox->currentIndex()-1);
    } else if (event->modifiers() & Qt::ShiftModifier) {
      return;
    } else {
      if (m_pagesComboBox->currentIndex() == m_nbPagesCurrCh-1)
        return;
      m_pagesComboBox->setCurrentIndex(m_currentPageNumber+1);
    }
    break;
  case Qt::Key_Left:
    if (event->modifiers() & Qt::ControlModifier) {
      if (m_chaptersComboBox->currentIndex() == m_chaptersComboBox->count()-1)
        return;
      m_chaptersComboBox->setCurrentIndex(m_chaptersComboBox->currentIndex()+1);
    } else if (event->modifiers() & Qt::ShiftModifier) {
      return;
    } else {
      if (m_pagesComboBox->currentIndex() == 0)
        return;
      m_pagesComboBox->setCurrentIndex(m_currentPageNumber-1);
    }
    break;
  default:
    return;
  }
}

void MangaReadWidget::mouseReleaseEvent(QMouseEvent* ) {
  m_zoomLabel->clear();
  m_zoomLabel->hide();
}

void MangaReadWidget::mouseMoveEvent(QMouseEvent* event) {
  m_zoomLabel->setVisible(m_zoomButton->isChecked() && event->button() == Qt::NoButton);


  if (m_zoomButton->isChecked()) {
    QPoint mousePosition = event->pos();
    // In order not to zoom outside of the box and to prevent from zooming when no page is displayed
    if (mousePosition.x() < 0 || mousePosition.y() < 0 || m_chaptersComboBox->currentIndex() < 1)
      return;

    int frameWidth = 400;
    int frameHeight = 300;
    int ratio = m_zoomComboBox->currentText().toInt();
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
