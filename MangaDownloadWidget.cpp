#include "MangaDownloadWidget.h"
#include "DownloadManager.h"
#include "Utils.h"

MangaDownloadWidget::MangaDownloadWidget(QWidget* parent):
  QWidget(parent),
  _scansDirectory(Utils::getScansDirectory()),
  _downloadManager(),
  _downloadHTMLManager(),
  _currentMangaDirectory(),
  _currentChapter(),
  _downloadQueue(),
  _downloadedCount(0),
  _totalCount(0) {

  QStringList mangaList = Utils::dirList(_scansDirectory);

  QLabel* selectMangaLabel = new QLabel("Select your manga:");

  _selectLineEdit = new QLineEdit(this);
  _selectLineEdit->setFixedWidth(250);
  connect(_selectLineEdit, SIGNAL(editingFinished()), this, SLOT(updateChaptersOnPCView()));
  connect(_selectLineEdit, SIGNAL(returnPressed()), this, SLOT(updateChaptersOnPCView()));

  QCompleter* completer = new QCompleter(mangaList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setCompletionMode(QCompleter::InlineCompletion);

  _selectLineEdit->setCompleter(completer);

  QHBoxLayout* chooseMangaLayout = new QHBoxLayout;
  chooseMangaLayout->addWidget(selectMangaLabel);
  chooseMangaLayout->addWidget(_selectLineEdit);
  chooseMangaLayout->addSpacing(1000);
  chooseMangaLayout->setAlignment(Qt::AlignTop);

  _chaptersOnPCModel = new QStringListModel;
  _chaptersOnPCView = new ChaptersOnPCView;
  _chaptersOnPCView->setModel(_chaptersOnPCModel);

  // _chaptersOnPCView->setDragDropMode(QAbstractItemView::DropOnly);
  connect(_chaptersOnPCView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(goToRead(QModelIndex)));

  QVBoxLayout* chaptersOnPCLayout = new QVBoxLayout;
  chaptersOnPCLayout->addWidget(_chaptersOnPCView);

  QGroupBox* chaptersOnPCGroupBox = new QGroupBox("Chapters on your computer");
  chaptersOnPCGroupBox->setLayout(chaptersOnPCLayout);

  _updateButton = new QPushButton;
  _updateButton->setIcon(QIcon(Utils::getIconsPath()+"/reload.png"));
  _updateButton->setFixedWidth(37);
  connect(_updateButton, SIGNAL(clicked(void)), this, SLOT(updateChapters(void)));

  _downloadButton = new QPushButton;
  _downloadButton->setIcon(QIcon(Utils::getIconsPath()+"/leftArrow.gif"));
  _downloadButton->setFixedWidth(37);
  connect(_downloadButton, SIGNAL(clicked(void)), this, SLOT(downloadChapters(void)));

  _selectAllButton = new QPushButton;
  _selectAllButton->setIcon(QIcon(Utils::getIconsPath()+"/selectAll.gif"));
  _selectAllButton->setFixedWidth(37);

  QVBoxLayout* buttonsWebLayout = new QVBoxLayout;
  buttonsWebLayout->addWidget(_updateButton);
  buttonsWebLayout->addWidget(_downloadButton);
  buttonsWebLayout->addWidget(_selectAllButton);
  buttonsWebLayout->setAlignment(Qt::AlignVCenter);

  _chaptersOnWebModel = new QStringListModel;

  _chaptersOnWebView = new ChaptersOnWebView(this);
  _chaptersOnWebView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _chaptersOnWebView->setModel(_chaptersOnWebModel);
  connect(_selectAllButton, SIGNAL(clicked(void)), _chaptersOnWebView, SLOT(selectAll(void)));

  QVBoxLayout* chaptersOnWebLayout = new QVBoxLayout;
  chaptersOnWebLayout->addWidget(_chaptersOnWebView);

  QGroupBox* chaptersOnWebGroupBox = new QGroupBox("New chapters online");
  chaptersOnWebGroupBox->setLayout(chaptersOnWebLayout);

  QHBoxLayout* viewsLayout = new QHBoxLayout;
  viewsLayout->addWidget(chaptersOnPCGroupBox);
  viewsLayout->addLayout(buttonsWebLayout);
  viewsLayout->addWidget(chaptersOnWebGroupBox);

  _clearTextEditButton = new QPushButton;
  _clearTextEditButton->setIcon(QIcon(Utils::getIconsPath()+"/clean.png"));
  _clearTextEditButton->setFixedWidth(37);
  connect(_clearTextEditButton, SIGNAL(clicked(void)), this, SLOT(clearMessage(void)));

  _stopButton = new QPushButton;
  _stopButton->setIcon(QIcon(Utils::getIconsPath()+"/stop.png"));
  _stopButton->setFixedWidth(37);
  connect(_stopButton, SIGNAL(clicked(void)), this, SLOT(stopDownload(void)));

  _pauseResumeButton = new QPushButton;
  _pauseResumeButton->setIcon(QIcon(Utils::getIconsPath()+"/pause.png"));
  _pauseResumeButton->setFixedWidth(37);
  _pauseResumeButton->setCheckable(true);
  connect(_pauseResumeButton, SIGNAL(toggled(bool)), this, SLOT(pauseResumeDownload(bool)));

  _progressBar = new QProgressBar;
  _progressBar->setVisible(false);
  _progressBar->setFixedWidth(300);

  _chaptersDownloadedLabel = new QLabel;

  QFormLayout* progressLayout = new QFormLayout;
  progressLayout->addRow(_progressBar, _chaptersDownloadedLabel);
  progressLayout->setAlignment(Qt::AlignLeft);

  QHBoxLayout* messageButtonLayout = new QHBoxLayout;
  messageButtonLayout->addWidget(_clearTextEditButton);
  messageButtonLayout->addWidget(_stopButton);
  messageButtonLayout->addWidget(_pauseResumeButton);
  messageButtonLayout->addLayout(progressLayout);
  messageButtonLayout->setAlignment(Qt::AlignLeft);

  _messageView = new MessageListView;
  _messageItemDelegate = new MessageItemDelegate;
  _messageModel = new MessageListModel;
  _messageView->setItemDelegate(_messageItemDelegate);
  _messageView->setModel(_messageModel);
  _messageView->setFixedHeight(300);

  QVBoxLayout* messageLayout = new QVBoxLayout;
  messageLayout->addLayout(messageButtonLayout);
  messageLayout->addWidget(_messageView);
  messageLayout->setAlignment(Qt::AlignLeft);

  QGroupBox* messageGroupBox = new QGroupBox("Output");
  messageGroupBox->setLayout(messageLayout);

  QLabel* titleLabel = new QLabel("Manga Download");
  titleLabel->setFont(QFont("", 18, 99));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  mainLayout->addLayout(chooseMangaLayout);
  mainLayout->addLayout(viewsLayout);
  mainLayout->addWidget(messageGroupBox);

  setLayout(mainLayout);

  connect(&_downloadManager, SIGNAL(done()), this, SLOT(downloadFinished()));
  connect(&_downloadManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(editMessage(QString, DownloadManager::MessageStatus, bool)));
  connect(&_downloadManager, SIGNAL(nbFilesDownloaded(int,int)), this, SLOT(updateProgressBar(int, int)));
  connect(&_downloadHTMLManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(editMessage(QString, DownloadManager::MessageStatus, bool)));
  connect(&_downloadHTMLManager, SIGNAL(nbFilesDownloaded(int,int)), this, SLOT(updateProgressBar(int, int)));
  connect(&_downloadHTMLManager, SIGNAL(done()), this, SLOT(pageDownloaded()));

  _chaptersWebView = new QWebView;
  connect(_chaptersWebView, SIGNAL(loadFinished(bool)), this, SLOT(chaptersUpdated(bool)));
  connect(_chaptersWebView, SIGNAL(loadProgress(int)), _progressBar, SLOT(setValue(int)));

  _pagesWebView = new QWebView;
  connect(_pagesWebView, SIGNAL(loadFinished(bool)), this, SLOT(chapterDownloaded(bool)));
  connect(_pagesWebView, SIGNAL(loadProgress(int)), _progressBar, SLOT(setValue(int)));
}

void MangaDownloadWidget::updateChapters(void) {
  _progressBar->setVisible(true);

  if (_selectLineEdit->text().isEmpty()) {
    editMessage("Warning: no manga name specified.", DownloadManager::Warning);
    editMessage("Abort.", DownloadManager::Error);
    return;
  }

  editMessage("Searching for new chapters...", DownloadManager::Information);

  _chaptersWebView->load(QUrl("http://eatmanga.com/Manga-Scan/"+_selectLineEdit->text()));
}

void MangaDownloadWidget::chaptersUpdated(bool) {
  QString htmlPage = _chaptersWebView->page()->currentFrame()->toHtml();

  QStringList everyChaptersOnWebList;
  QStringList chaptersStringList = htmlPage.split("<table id=\"series\">", QString::SkipEmptyParts);
  if (chaptersStringList.size() < 2) {
    editMessage("Error: cannot find chapters list, please retry. Maybe you are not connected to the Internet.", DownloadManager::Error);
    return;
  }
  QString chaptersString = chaptersStringList.at(1);
  QStringList rawChapterNames = chaptersString.split("href=\"/Manga-Scan/"+_selectLineEdit->text()+"/");
  rawChapterNames.removeFirst();
  for (const QString& chapterName: rawChapterNames) {
    QStringList urlArgs = chapterName.split("/");
    if (urlArgs.size() < 1) {
      editMessage("Warning: cannot find chapter name. Skip it.", DownloadManager::Warning);
      break;
    }
    everyChaptersOnWebList << urlArgs.at(0);
  }

  _currentMangaDirectory = QDir(_scansDirectory.path()+"/"+_selectLineEdit->text());
  QStringList everyChaptersOnPCList = Utils::dirList(_currentMangaDirectory);

  QStringList newChaptersList;
  for (const QString& chapter: everyChaptersOnWebList) {
    if (!everyChaptersOnPCList.contains(chapter))
      newChaptersList << chapter;
  }

  if (newChaptersList.size() == 0)
    _chaptersOnWebModel->setStringList(QStringList("Already up to date"));
  else
    _chaptersOnWebModel->setStringList(newChaptersList);

  _progressBar->setVisible(false);
  _progressBar->setValue(0);

  editMessage("Available chapters list updated.", DownloadManager::Success);
}

void MangaDownloadWidget::downloadChapters(void) {
  _chaptersOnWebView->setEnabled(false);
  _progressBar->setVisible(true);
  _progressBar->setValue(0);

  if (_selectLineEdit->text().isEmpty()) {
    editMessage("Warning: no manga name specified.", DownloadManager::Warning);
    editMessage("Abort.", DownloadManager::Error);
    return;
  }

  editMessage("Searching for pages to download...", DownloadManager::Information);
  QModelIndexList chaptersSelectedIndexes = _chaptersOnWebView->selectionModel()->selectedIndexes();
  for (const QModelIndex& chapterIndex: chaptersSelectedIndexes) {
    QString currChapter = _chaptersOnWebModel->data(chapterIndex, Qt::DisplayRole).toString();
    _downloadQueue.enqueue("http://eatmanga.com/Manga-Scan/"+_selectLineEdit->text()+"/"+currChapter);
  }

  _totalCount = _downloadQueue.size();
  _downloadedCount = 0;
  startNextDownload();
}

void MangaDownloadWidget::startNextDownload(void) {
  if (_downloadQueue.isEmpty()) {
    _chaptersDownloadedLabel->setText("");
    QString downloadReport = QString::number(_downloadedCount)+"/"+QString::number(_totalCount)+" chapter(s) downloaded successfully.";
    if (_downloadedCount == _totalCount) {
      editMessage(downloadReport, DownloadManager::Success);
    } else {
      editMessage("Only "+downloadReport, DownloadManager::Warning);
      editMessage("Please check out each issue and retry if necessary.", DownloadManager::Warning);
    }
    _totalCount = 0;
    _downloadedCount = 0;
    _chaptersOnWebView->setEnabled(true);
    updateChapters();
    updateChaptersOnPCView();

    emit initModelRequested();
    emit downloadDone("Download finished", downloadReport);
    return;
  }

  editMessage("Next download will begin shortly...", DownloadManager::Information);

  QString urlString = _downloadQueue.dequeue();
  _currentChapter = urlString.split("/").last();

  _pagesWebView->load(QUrl(urlString));
  _chaptersDownloadedLabel->setText("Chapter "+QString::number(_totalCount-_downloadQueue.size())+"/"+QString::number(_totalCount));
}

void MangaDownloadWidget::chapterDownloaded(bool) {
  _progressBar->setVisible(false);
  _downloadHTMLManager.clean();

  QStringList htmlPages;
  QString htmlPage = _pagesWebView->page()->currentFrame()->toHtml();
  QStringList selectStringListDirty = htmlPage.split("<select id=\"pages\" onchange=\"javascript:window.location=this.value;\"");
  if (selectStringListDirty.size() < 2) {
    editMessage("Error: cannot find pages from selected chapter (any).", DownloadManager::Error);
    editMessage("Abort.", DownloadManager::Error);
    return;
  }

  QStringList selectStringList = selectStringListDirty.at(1).split("</select>");
  if (selectStringList.size() < 1) {
    editMessage("Error: cannot find pages from selected chapter (corrupted).", DownloadManager::Error);
    editMessage("Abort.", DownloadManager::Error);
    return;
  }

  QString selectString = selectStringList.at(0);

  QStringList rawPageNames = selectString.split("value=\"/Manga-Scan/"+_selectLineEdit->text()+"/");
  rawPageNames.removeFirst();
  for (const QString& pageName: rawPageNames) {
    QString urlBaseName = "http://eatmanga.com/Manga-Scan/"+_selectLineEdit->text()+"/";
    QStringList currUrlStringList = pageName.split("\"");
    if (currUrlStringList.size() < 1) {
      editMessage("Warning: cannot find current page from selected chapter. Skip it.", DownloadManager::Warning);
      break;
    }

    htmlPages << urlBaseName+currUrlStringList.at(0);
  }

  _downloadHTMLManager.append(htmlPages);
}

void MangaDownloadWidget::pageDownloaded(void) {
  QStringList htmlPagesStringList = _downloadHTMLManager.getHTMLPages();

  QStringList urls;
  for (const QString& htmlPage: htmlPagesStringList) {
    QStringList imgStringListDirty = htmlPage.split("<img id=\"eatmanga_image");
    if (imgStringListDirty.size() < 2) {
      editMessage("Warning: cannot find current page from selected chapter (any). Skip it", DownloadManager::Warning);
      break;
    }

    QStringList srcStringList = imgStringListDirty.at(1).split("src=\"");
    if (srcStringList.size() < 2) {
      editMessage("Warning: cannot find current page from selected chapter (corrupted). Skip it", DownloadManager::Warning);
      break;
    }

    QStringList imgStringList = srcStringList.at(1).split("\"");
    if (srcStringList.size() < 1) {
      editMessage("Warning: cannot find current page from selected chapter (corrupted). Skip it", DownloadManager::Warning);
      break;
    }

    urls << imgStringList.at(0);
  }

  _currentMangaDirectory.mkdir(_currentChapter);

  _downloadManager.append(urls);
}

void MangaDownloadWidget::updateChaptersOnPCView(void) {
  if (_selectLineEdit->text().isEmpty())
    return;

  _currentMangaDirectory = QDir(_scansDirectory.path()+"/"+_selectLineEdit->text());
  _chaptersOnPCModel->setStringList(Utils::dirList(_currentMangaDirectory, true));
  _chaptersOnWebModel->setStringList(QStringList());

  _chaptersOnPCView->setFocus();
}

void MangaDownloadWidget::searchForDownload(QString mangaName) {
  _selectLineEdit->setText(mangaName);
  _selectLineEdit->setFocus();
  _chaptersOnPCView->setFocus();
  updateChapters();
}

void MangaDownloadWidget::downloadFinished(void) {
  updatedb();
  ++_downloadedCount;
  startNextDownload();
}

void MangaDownloadWidget::goToRead(QModelIndex modelIndex) {
  QString mangaName = _selectLineEdit->text();
  QString chapterName = _chaptersOnPCModel->data(modelIndex, Qt::DisplayRole).toString();

  emit chapterSelected(mangaName, chapterName);
}

void MangaDownloadWidget::editMessage(QString message, DownloadManager::MessageStatus messageStatus, bool newLine) {
  QStandardItem* item = new QStandardItem;

  switch(messageStatus) {
  case DownloadManager::Information:
    item->setData(message, MessageItemDelegate::InformationRole);
    break;
  case DownloadManager::Success:
    item->setData(message, MessageItemDelegate::SuccessRole);
    break;
  case DownloadManager::Warning:
    item->setData(message, MessageItemDelegate::WarningRole);
    break;
  case DownloadManager::Error:
    item->setData(message, MessageItemDelegate::ErrorRole);
    break;
  case DownloadManager::Download:
    item->setData(message, MessageItemDelegate::DownloadRole);
    break;
  default:
    item->setData(message, MessageItemDelegate::DownloadRole);
    break;
  }

  _messageModel->append(item, newLine);
  _messageView->scrollToBottom();
}

void MangaDownloadWidget::clearMessage(void) {
  _messageModel->clear();
}

void MangaDownloadWidget::stopDownload(void) {
  _downloadManager.stop();

  _totalCount = 0;
  _downloadedCount = 0;
  _chaptersOnWebView->setEnabled(true);
  _progressBar->setValue(0);
  _progressBar->setVisible(false);
  _chaptersDownloadedLabel->setText("");
  _downloadQueue.clear();
  _downloadManager.clean();
  _downloadHTMLManager.clean();

  updateChapters();
  updateChaptersOnPCView();

  emit initModelRequested();
  emit downloadDone("Download stopped", "Download has been stopped.");
}

void MangaDownloadWidget::pauseResumeDownload(bool check) {
  _pauseResumeButton->setChecked(check);
  if (_pauseResumeButton->isChecked()) {
    _pauseResumeButton->setIcon(QIcon(Utils::getIconsPath()+"/resume.png"));
    _downloadManager.pause();
  } else {
    _pauseResumeButton->setIcon(QIcon(Utils::getIconsPath()+"/pause.png"));
    _downloadManager.resume();
  }
}

void MangaDownloadWidget::updateProgressBar(int donloadedCount, int totalCount) {
  _progressBar->setValue(donloadedCount * 100 / totalCount);
  if (_progressBar->value() < 100)
    _progressBar->setVisible(true);
  else
    _progressBar->setVisible(false);
}

void MangaDownloadWidget::updatedb(void) {
  Utils::addChapter(_selectLineEdit->text(), _currentChapter);
}

void MangaDownloadWidget::keyReleaseEvent(QKeyEvent* event) {
  // Enter released
  if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
    QModelIndex index = _chaptersOnPCView->currentIndex();
    if (index.isValid())
      goToRead(index);
  }
}
