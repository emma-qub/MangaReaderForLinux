#include "MangaReadWidget.h"
#include "Utils.h"

MangaReadWidget::MangaReadWidget(QWidget* parent) :
  QWidget(parent),
  _scansDirectory(Utils::getScansDirectory()),
  _currentMangaDirectory(),
  _currentChapterDirectory(),
  _chapterStringList(),
  _currentPageNumber(-1),
  _nbPagesCurrCh(-1),
  _mousePosition() {

  QStringList mangaList = Utils::dirList(_scansDirectory);


  /// Select manga

  QLabel* selectMangaLabel = new QLabel("Select your manga:");

  _selectLineEdit = new QLineEdit;
  QCompleter* completer = new QCompleter(mangaList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setCompletionMode(QCompleter::PopupCompletion);
  _selectLineEdit->setCompleter(completer);

  _mangasComboBox = new QComboBox;
  _mangasComboBox->addItems(mangaList);
  _mangasComboBox->setFixedWidth(250);
  _mangasComboBox->setLineEdit(_selectLineEdit);

  connect(_mangasComboBox, SIGNAL(activated(QString)), this, SLOT(updateChaptersComboBox(QString)));
  connect(_mangasComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateChaptersComboBox(QString)));


  /// Select chapter

  _chaptersComboBox = new QComboBox;
  _chaptersComboBox->setFixedWidth(300);
  connect(_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));


  /// Select page

  _nbPagesLabel = new QLabel;

  _pagesComboBox = new QComboBox;
  _pagesComboBox->setFixedWidth(80);
  connect(_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));


  /// Select first manga in list if any

  if (_mangasComboBox->count() > 0) {
    updateChaptersComboBox(_mangasComboBox->itemText(0));
  }


  /// Zoom

  _zoomButton = new QPushButton;
  _zoomButton->setIcon(QIcon(Utils::getIconsPath()+"/zoom.png"));
  _zoomButton->setCheckable(true);
  _zoomButton->setChecked(true);
  _zoomButton->setFixedWidth(37);

  _zoomComboBox = new QComboBox;
  QStringList zoomValues;
  zoomValues << QString::number(2) << QString::number(4) << QString::number(8);
  _zoomComboBox->addItems(zoomValues);
  _zoomComboBox->setCurrentIndex(0);
  _zoomComboBox->setFixedWidth(50);


  /// Manga form layout

  QHBoxLayout* chooseMangaLayout = new QHBoxLayout;
  chooseMangaLayout->addWidget(selectMangaLabel);
  chooseMangaLayout->addWidget(_mangasComboBox);
  chooseMangaLayout->addWidget(_chaptersComboBox);
  chooseMangaLayout->addWidget(_pagesComboBox);
  chooseMangaLayout->addWidget(_nbPagesLabel);
  chooseMangaLayout->addWidget(_zoomButton);
  chooseMangaLayout->addWidget(_zoomComboBox);
  chooseMangaLayout->addSpacing(500);


  /// Page

  _currentPageLabel = new QLabel;
  _currentPageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  _scrollArea = new QScrollArea;
  _scrollArea->setBackgroundRole(QPalette::Background);
  _scrollArea->setWidget(_currentPageLabel);
  _scrollArea->setWidgetResizable(true);
  _scrollArea->setAlignment(Qt::AlignHCenter);

  _zoomLabel = new QLabel(_scrollArea);
  _zoomLabel->hide();


  /// Main layout

  QLabel* titleLabel = new QLabel("Manga Read");
  titleLabel->setFont(QFont("", 18, 99));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  mainLayout->addLayout(chooseMangaLayout);
  mainLayout->addWidget(_scrollArea);

  setLayout(mainLayout);
}

void MangaReadWidget::updateChaptersComboBox(QString mangaName) {
  disconnect(_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  _chaptersComboBox->clear();
  connect(_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));

  if (mangaName.isEmpty())
    return;

  _currentMangaDirectory = QDir(_scansDirectory.path()+"/"+mangaName);

  QStringList chaptersList = Utils::dirList(_currentMangaDirectory, true);

  if (chaptersList.size() == 0) {
    changeChapter("");
    return;
  }

  disconnect(_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
  _chaptersComboBox->addItems(chaptersList);
  connect(_chaptersComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeChapter(QString)));
}

void MangaReadWidget::changeChapter(QString chapterName) {
  if (chapterName.isEmpty()) {
    _currentPageLabel->setPixmap(QPixmap());
    updatePagesComboBox();
    return;
  }

  _currentChapterDirectory = QDir(_currentMangaDirectory.path() + "/" + chapterName);
  _currentPageNumber = 0;
  updatePagesComboBox();
  Utils::updateChapterRead(_mangasComboBox->currentText(), _chaptersComboBox->currentText(), true);

  emit chapterSelected(_mangasComboBox->currentText(), _chaptersComboBox->currentText());
}

void MangaReadWidget::updatePagesComboBox(void) {
  disconnect(_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));
  _pagesComboBox->clear();
  connect(_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));

  _nbPagesLabel->setText("/");

  if (_mangasComboBox->currentText().isEmpty() || _chaptersComboBox->currentText().isEmpty())
    return;

  QStringList nameFilters;
  nameFilters << "*.png" << "*.jpg" << "*.gif" << "*.jpeg" << "*.bmp";
  _chapterStringList = Utils::filesList(_currentChapterDirectory);
  _nbPagesCurrCh = _chapterStringList.size();

  disconnect(_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));
  for (int k = 1; k <= _nbPagesCurrCh; k++) {
    _pagesComboBox->addItem(QString::number(k));
  }
  connect(_pagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePage(int)));

  _nbPagesLabel->setText("/"+QString::number(_nbPagesCurrCh));
  updateCurrentPage();
}

void MangaReadWidget::changePage(int pageNumber) {
  if (pageNumber > -1 && pageNumber < _nbPagesCurrCh) {
    _currentPageNumber = pageNumber;
    updateCurrentPage();
  }
}

void MangaReadWidget::updateCurrentPage(void) {
  if (_currentPageNumber >= _nbPagesCurrCh) {
    QMessageBox::critical(this, "Page number error", "Error: cannot load page number " + QString::number(_currentPageNumber+1) + " because chapter counts only " + _chapterStringList.size() + " pages.");
    return;
  }
  _currentPageLabel->setPixmap(QPixmap(_currentChapterDirectory.path()+"/"+_chapterStringList.at(_currentPageNumber)));
  _scrollArea->verticalScrollBar()->setValue(0);
}

void MangaReadWidget::switchManga(QString mangaName, QString chapterName) {
  _mangasComboBox->setCurrentText(mangaName);
  _chaptersComboBox->setCurrentText(chapterName);

  _currentPageLabel->setFocus();
  if (_chaptersComboBox->currentText() != chapterName) {
    _chaptersComboBox->setCurrentText(chapterName);
  }
}

void MangaReadWidget::keyReleaseEvent(QKeyEvent* event) {
  if (_chaptersComboBox->currentText().isEmpty())
    return;

  switch (event->key()) {
  case Qt::Key_Right:
    if (event->modifiers() & Qt::ControlModifier) {
      if (_chaptersComboBox->currentIndex() == 0)
        return;
      _chaptersComboBox->setCurrentIndex(_chaptersComboBox->currentIndex()-1);
    } else if (event->modifiers() & Qt::ShiftModifier) {
      return;
    } else {
      if (_pagesComboBox->currentIndex() == _nbPagesCurrCh-1)
        return;
      _pagesComboBox->setCurrentIndex(_currentPageNumber+1);
    }
    break;
  case Qt::Key_Left:
    if (event->modifiers() & Qt::ControlModifier) {
      if (_chaptersComboBox->currentIndex() == _chaptersComboBox->count()-1)
        return;
      _chaptersComboBox->setCurrentIndex(_chaptersComboBox->currentIndex()+1);
    } else if (event->modifiers() & Qt::ShiftModifier) {
      return;
    } else {
      if (_pagesComboBox->currentIndex() == 0)
        return;
      _pagesComboBox->setCurrentIndex(_currentPageNumber-1);
    }
    break;
  default:
    return;
  }
}

void MangaReadWidget::mouseReleaseEvent(QMouseEvent *) {
  _zoomLabel->clear();
  _zoomLabel->hide();
}

void MangaReadWidget::mouseMoveEvent(QMouseEvent* event) {
  _zoomLabel->setVisible(_zoomButton->isChecked() && event->button() == Qt::NoButton);


  if (_zoomButton->isChecked()) {
    QPoint mousePosition = event->pos();
    // In order not to zoom outside of the box and to prevent from zooming when no page is displayed
    if (mousePosition.x() < 0 || mousePosition.y() < 0 || _chaptersComboBox->currentIndex() < 1)
      return;

    int frameWidth = 400;
    int frameHeight = 300;
    int ratio = _zoomComboBox->currentText().toInt();
    int frameWidthRatio = qRound(static_cast<double>(frameWidth)/static_cast<double>(ratio));
    int frameHeightRatio = qRound(static_cast<double>(frameHeight)/static_cast<double>(ratio));

    mousePosition.rx() -= _scrollArea->geometry().x();
    mousePosition.ry() -= _scrollArea->geometry().y();

    int topShift = 0;
    if (_currentPageLabel->visibleRegion().boundingRect().top()+mousePosition.y() > _currentPageLabel->visibleRegion().boundingRect().bottom() - frameHeight)
      topShift = frameHeight;

    QPoint rawTopLeft = mousePosition - _currentPageLabel->geometry().topLeft();
    QPoint topLeft(qMin(_currentPageLabel->width()-frameWidthRatio, rawTopLeft.x()), rawTopLeft.y()-topShift/ratio);
    QPoint bottomRight = topLeft+QPoint(frameWidthRatio, frameHeightRatio);

    if (mousePosition.x() < _currentPageLabel->geometry().left()
        || mousePosition.x() > _currentPageLabel->geometry().right()
        || mousePosition.y() < _currentPageLabel->geometry().top()
        || mousePosition.y() > _currentPageLabel->geometry().bottom())
      _zoomLabel->hide();

    QRect frame(topLeft, bottomRight);

    QPixmap zoomedPixmap = _currentPageLabel->pixmap()->copy(frame).scaled(frameWidth, frameHeight);
    _zoomLabel->setPixmap(zoomedPixmap);

    _zoomLabel->move(mousePosition+QPoint(0, -topShift));
    _zoomLabel->resize(frameWidth, frameHeight);
  }
}
