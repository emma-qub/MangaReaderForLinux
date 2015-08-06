#include "MangaDownloadWidget.h"
#include "DownloadManager.h"
#include "Utils.h"


#include <iostream>
#define cerro(x) std::cerr << x << std::endl;


MangaDownloadWidget::MangaDownloadWidget(QWidget* parent):
  QWidget(parent),
  _scansDirectory(Utils::getScansDirectory()),
  _mangaList(Utils::dirList(_scansDirectory)),
  _currentMangaDirectory(),
  _currentChapter(),
  _downloadQueue(),
  _chaptersQueue(),
  _downloadedCount(0),
  _totalCount(0) {


  /// Select manga line edit
  _selectLineEdit = new QLineEdit;
  _selectLineEdit->setFixedWidth(250);
  connect(_selectLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkMangaNameExists(QString)));
//  connect(_selectLineEdit, SIGNAL(editingFinished(void)), this, SLOT(updateChaptersOnPCView(void)));
//  connect(_selectLineEdit, SIGNAL(returnPressed(void)), this, SLOT(updateChaptersOnPCView(void)));

  QCompleter* completer = new QCompleter(_mangaList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setCompletionMode(QCompleter::PopupCompletion);

  _selectLineEdit->setCompleter(completer);

  QFormLayout* chooseMangaLayout = new QFormLayout;
  chooseMangaLayout->addRow("Select your manga:", _selectLineEdit);


  /// Chapters on PC

  _chaptersOnPCModel = new QStringListModel;

  _chaptersOnPCView = new ChaptersOnPCView;
  _chaptersOnPCView->setModel(_chaptersOnPCModel);
  _chaptersOnPCView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // _chaptersOnPCView->setDragDropMode(QAbstractItemView::DropOnly);
  connect(_chaptersOnPCView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(goToRead(QModelIndex)));

  QVBoxLayout* chaptersOnPCLayout = new QVBoxLayout;
  chaptersOnPCLayout->addWidget(_chaptersOnPCView);

  QGroupBox* chaptersOnPCGroupBox = new QGroupBox("Chapters on your computer");
  chaptersOnPCGroupBox->setLayout(chaptersOnPCLayout);


  /// Chapters on web

  _chaptersOnWebModel = new QStandardItemModel;

  _chaptersOnWebView = new ChaptersOnWebView(this);
  _chaptersOnWebView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _chaptersOnWebView->setModel(_chaptersOnWebModel);
  _chaptersOnWebView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QVBoxLayout* chaptersOnWebLayout = new QVBoxLayout;
  chaptersOnWebLayout->addWidget(_chaptersOnWebView);

  QGroupBox* chaptersOnWebGroupBox = new QGroupBox("New chapters online");
  chaptersOnWebGroupBox->setLayout(chaptersOnWebLayout);


  /// Central buttons

  _updateButton = new QPushButton;
  _updateButton->setIcon(QIcon(Utils::getIconsPath()+"/reload.png"));
  _updateButton->setFixedWidth(37);
  connect(_updateButton, SIGNAL(clicked(void)), this, SLOT(updateChaptersList(void)));

  _downloadButton = new QPushButton;
  _downloadButton->setIcon(QIcon(Utils::getIconsPath()+"/leftArrow.gif"));
  _downloadButton->setFixedWidth(37);
  connect(_downloadButton, SIGNAL(clicked(void)), this, SLOT(downloadChapters(void)));

  _selectAllButton = new QPushButton;
  _selectAllButton->setIcon(QIcon(Utils::getIconsPath()+"/selectAll.gif"));
  _selectAllButton->setFixedWidth(37);
  connect(_selectAllButton, SIGNAL(clicked(void)), _chaptersOnWebView, SLOT(selectAll(void)));

  QVBoxLayout* buttonsWebLayout = new QVBoxLayout;
  buttonsWebLayout->addWidget(_updateButton);
  buttonsWebLayout->addWidget(_downloadButton);
  buttonsWebLayout->addWidget(_selectAllButton);
  buttonsWebLayout->setAlignment(Qt::AlignVCenter);


  /// Chapters on PC, central buttons and chapters on web layout

  QHBoxLayout* viewsLayout = new QHBoxLayout;
  viewsLayout->addWidget(chaptersOnPCGroupBox);
  viewsLayout->addLayout(buttonsWebLayout);
  viewsLayout->addWidget(chaptersOnWebGroupBox);


  /// Message output

  _messageView = new MessageListView;
  _messageItemDelegate = new MessageItemDelegate;
  _messageModel = new MessageListModel;
  _messageView->setItemDelegate(_messageItemDelegate);
  _messageView->setModel(_messageModel);


  /// Bottom buttons

  _clearTextEditButton = new QPushButton;
  _clearTextEditButton->setIcon(QIcon(Utils::getIconsPath()+"/clean.png"));
  _clearTextEditButton->setFixedWidth(37);
  connect(_clearTextEditButton, SIGNAL(clicked(void)), _messageModel, SLOT(clearMessages(void)));

  _stopButton = new QPushButton;
  _stopButton->setIcon(QIcon(Utils::getIconsPath()+"/stop.png"));
  _stopButton->setFixedWidth(37);
  _stopButton->setEnabled(false);
  connect(_stopButton, SIGNAL(clicked(void)), this, SLOT(stopDownload(void)));


  /// Progress bars

  _chaptersProgressBar = new QProgressBar;
  _chaptersProgressBar->setVisible(false);
  _chaptersProgressBar->setFixedSize(300, 20);

  _chaptersDownloadedLabel = new QLabel;

  _pagesProgressBar = new QProgressBar;
  _pagesProgressBar->setVisible(false);
  _pagesProgressBar->setFixedSize(300, 20);

  _pagesDownloadedLabel = new QLabel;


  /// Bottom buttons and progress bar layout

  QFormLayout* chaptersProgressLayout = new QFormLayout;
  chaptersProgressLayout->addRow(_chaptersProgressBar, _chaptersDownloadedLabel);
  chaptersProgressLayout->setAlignment(Qt::AlignLeft);

  QFormLayout* pagesProgressLayout = new QFormLayout;
  pagesProgressLayout->addRow(_pagesProgressBar, _pagesDownloadedLabel);
  pagesProgressLayout->setAlignment(Qt::AlignLeft);

  QVBoxLayout* progressLayout = new QVBoxLayout;
  progressLayout->addLayout(chaptersProgressLayout);
  progressLayout->addLayout(pagesProgressLayout);

  QHBoxLayout* messageButtonLayout = new QHBoxLayout;
  messageButtonLayout->addWidget(_clearTextEditButton);
  messageButtonLayout->addWidget(_stopButton);
  messageButtonLayout->addLayout(progressLayout);
  messageButtonLayout->setAlignment(Qt::AlignLeft);

  QVBoxLayout* messageLayout = new QVBoxLayout;
  messageLayout->addLayout(messageButtonLayout);
  messageLayout->addWidget(_messageView);
  messageLayout->setAlignment(Qt::AlignLeft);

  QGroupBox* messageGroupBox = new QGroupBox("Output");
  messageGroupBox->setLayout(messageLayout);

  QLabel* titleLabel = new QLabel("Manga Download");
  titleLabel->setFont(QFont("", 18, 99));


  /// Main layout

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  mainLayout->addLayout(chooseMangaLayout);
  mainLayout->addLayout(viewsLayout);
  mainLayout->addWidget(messageGroupBox);

  setLayout(mainLayout);


  /// Get chapters on web list process

  _getChaptersListProcess = new QProcess(this);
  connect(_getChaptersListProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(chaptersListUpdated(int,QProcess::ExitStatus)));
  connect(_getChaptersListProcess, SIGNAL(started(void)), this, SLOT(chaptersListUpdateStarted(void)));
  connect(_getChaptersListProcess, SIGNAL(readyReadStandardOutput(void)), this, SLOT(getChaptersListUpdated(void)));


  /// Download chapters process

  _downloadChapterProcess = new QProcess(this);
  connect(_downloadChapterProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(downloadFinished(int,QProcess::ExitStatus)));
  connect(_downloadChapterProcess, SIGNAL(started(void)), this, SLOT(nextDownloadHasStarted(void)));
  connect(_downloadChapterProcess, SIGNAL(readyReadStandardOutput(void)), this, SLOT(getDownloadInfo(void)));


  /// Style

  QString progressBarStyle(
  "QProgressBar {"
  "    border: 1px solid lightgrey;"
  "    border-radius: 5px;"
  "    text-align: center;"
  "    font-weight: bold;"
  "}"
  "QProgressBar::chunk {"
  "    background-color: #449D44;"
  "    width: 10px;"
//  "    margin: 0.5px;"
  "}"
  );
  setStyleSheet(styleSheet()+progressBarStyle);


}


/// Update chapters list

void MangaDownloadWidget::updateChaptersList(void) {
  if (_selectLineEdit->text().isEmpty()) {
    _messageModel->editMessageWarning("Warning: no manga name specified.");
    _messageModel->editMessageError("Abort.");
    return;
  }

  _chaptersOnWebModel->clear();

  updateChaptersOnPCView();
  _chaptersOnPCView->setFocus();

  QStringList arguments;
  arguments << _selectLineEdit->text();
  _getChaptersListProcess->start(Utils::getScriptsAbsolutePath()+"/updateChaptersList.sh", arguments);
}


/// Check if manga name exists

void MangaDownloadWidget::checkMangaNameExists(QString mangaName) {
  bool mangaNameExists = _mangaList.contains(mangaName);
  QString color;
  if (!mangaNameExists) {
    color = "#C9302C";
  } else {
    color = "#000000";
  }

  _selectLineEdit->setStyleSheet(_selectLineEdit->styleSheet()+"color: "+color+";");

  _downloadButton->setEnabled(mangaNameExists);
  _updateButton->setEnabled(mangaNameExists);
}


/// Search for download

void MangaDownloadWidget::searchForDownload(QString mangaName) {
  _selectLineEdit->setText(mangaName);
  updateChaptersList();
}


/// Update chapters on PC

void MangaDownloadWidget::updateChaptersOnPCView(void) {
  if (_selectLineEdit->text().isEmpty())
    return;

  _currentMangaDirectory = QDir(_scansDirectory.path()+"/"+_selectLineEdit->text());
  _chaptersOnPCModel->setStringList(Utils::dirList(_currentMangaDirectory, true));

  _chaptersOnPCView->setFocus();
}


/// Update chapters list SLOTS

void MangaDownloadWidget::chaptersListUpdated(int status, QProcess::ExitStatus exitStatus) {
  switch (exitStatus) {
  case QProcess::CrashExit: {
    _messageModel->editMessageWarning("Warning: Chapters list update failed. Status code: "+QString::number(status));
    break;
  }
  case QProcess::NormalExit: {
    if (status == 1) {
      _messageModel->editMessageWarning("Warning: "+_getChaptersListProcess->readAllStandardError());
      _messageModel->editMessageError("Abort.");
    } else if (status == 0) {
      _messageModel->editMessageSuccess("Chapters list updated.");
    }
    break;
  }
  }

  if (_chaptersOnWebModel->rowCount() == 0) {
    QStandardItem* upToDateItem = new QStandardItem;
    upToDateItem->setData("Already up-to-date.", Qt::DisplayRole);
    upToDateItem->setEditable(false);
    upToDateItem->setSelectable(false);
    _chaptersOnWebModel->appendRow(upToDateItem);
  }

  _selectLineEdit->setEnabled(true);
  _updateButton->setEnabled(true);
  _downloadButton->setEnabled(true);
  _selectAllButton->setEnabled(true);
}

void MangaDownloadWidget::chaptersListUpdateStarted(void) {
  _selectLineEdit->setEnabled(false);
  _updateButton->setEnabled(false);
  _downloadButton->setEnabled(false);
  _selectAllButton->setEnabled(false);
  _messageModel->editMessageInformation("Updating chapters list...");
}

void MangaDownloadWidget::getChaptersListUpdated(void) {
  QString chaptersList(_getChaptersListProcess->readAllStandardOutput());

  QStringList chaptersUrlAndTitleList = chaptersList.split("\n", QString::SkipEmptyParts);

  for (const QString& chapterUrlAndTitle: chaptersUrlAndTitleList) {
    QStringList chapterUrlAndTitleList = chapterUrlAndTitle.split(";");

    if (chapterUrlAndTitleList.size() != 2) {
      for (const QString& s: chapterUrlAndTitleList) {
        cerro(s.toStdString())
      }

      _messageModel->editMessageError("Error: enable to build chapters list.");
      _messageModel->editMessageError("Abort.");
      _chaptersOnWebModel->clear();
      return;
    }

    QString chapterTitle = chapterUrlAndTitleList.at(1).trimmed();
    QString chapterUrl = chapterUrlAndTitleList.at(0).trimmed();
    QString chapterTitleInUrl;
    if (chapterUrl.endsWith('/')) {
      chapterUrl.truncate(chapterUrl.length()-1);
    }
    chapterTitleInUrl = chapterUrl.split('/').last();

    if (!_chaptersOnPCModel->stringList().contains(chapterTitleInUrl)) {
      QStandardItem* chapterItem = new QStandardItem;
      chapterItem->setData(chapterTitle, Qt::DisplayRole);
      chapterItem->setData(chapterUrl, Qt::UserRole);
      chapterItem->setData(chapterTitleInUrl, Qt::UserRole+1);
      _chaptersOnWebModel->appendRow(chapterItem);
    }
  }
}


/// Download process SLOTS

void MangaDownloadWidget::downloadFinished(int status, QProcess::ExitStatus exitStatus) {
  switch (exitStatus) {
  case QProcess::CrashExit: {
    _messageModel->editMessageWarning("Warning: Downlaod failed. Status code: "+QString::number(status));
    break;
  }
  case QProcess::NormalExit: {
    ++_downloadedCount;
    _chaptersOnWebModel->removeRow(_chaptersQueue.takeFirst().row());
    updatedb();
    updateChaptersOnPCView();
    _messageModel->editMessageSuccess("Downlaod succeeded. Status code: "+QString::number(status));
    break;
  }
  }

  startNextDownload();
}

void MangaDownloadWidget::nextDownloadHasStarted(void) {
  _messageModel->editMessageInformation("Downloading "+_currentChapter+"...");
  _chaptersProgressBar->show();
  _chaptersProgressBar->setValue(static_cast<int>(static_cast<float>(_downloadedCount)/static_cast<float>(_totalCount)*100));

  _pagesProgressBar->show();
  _pagesProgressBar->setValue(0);
}

void MangaDownloadWidget::downloadChapters(void) {
  if (_selectLineEdit->text().isEmpty()) {
    _messageModel->editMessageWarning("Warning: no manga name specified.");
    _messageModel->editMessageError("Abort.");
    return;
  }

  _selectLineEdit->setEnabled(false);
  _chaptersOnWebView->setEnabled(false);
  _selectAllButton->setEnabled(false);
  _downloadButton->setEnabled(false);
  _updateButton->setEnabled(false);
  _stopButton->setEnabled(true);
  _chaptersProgressBar->show();
  _chaptersProgressBar->setValue(0);
  _chaptersDownloadedLabel->show();
  _chaptersDownloadedLabel->setText("Chapter");
  _pagesProgressBar->show();
  _pagesProgressBar->setValue(0);
  _pagesDownloadedLabel->show();
  _pagesDownloadedLabel->setText("Page");

  _messageModel->editMessageInformation("Gathering information on chapters to downlaod...");
  QModelIndexList chaptersSelectedIndexes = _chaptersOnWebView->selectionModel()->selectedIndexes();
  for (const QModelIndex& chapterIndex: chaptersSelectedIndexes) {
    _chaptersQueue.enqueue(chapterIndex);
    QString currChapterUrl = _chaptersOnWebModel->data(chapterIndex, Qt::UserRole).toString();
    QString currChapterTitle = _chaptersOnWebModel->data(chapterIndex, Qt::UserRole+1).toString();
    _downloadQueue.enqueue(QPair<QString, QString>(currChapterTitle, currChapterUrl));
  }

  _totalCount = _downloadQueue.size();
  _downloadedCount = 0;

  emit downloading(true);

  startNextDownload();
}

void MangaDownloadWidget::startNextDownload(void) {
  if (_downloadQueue.isEmpty()) {
    QString downloadReport = QString::number(_downloadedCount)+"/"+QString::number(_totalCount)+" chapter(s) downloaded successfully.";
    if (_downloadedCount == _totalCount) {
      _messageModel->editMessageSuccess(downloadReport);
    } else {
      _messageModel->editMessageWarning("Only "+downloadReport);
      _messageModel->editMessageWarning("Please check out each issue and retry if necessary.");
    }

    updateChaptersList();

    _totalCount = 0;
    _downloadedCount = 0;

    _selectLineEdit->setEnabled(true);
    _chaptersOnWebView->setEnabled(true);
    _selectAllButton->setEnabled(true);
    _downloadButton->setEnabled(true);
    _updateButton->setEnabled(true);
    _stopButton->setEnabled(false);

    _chaptersProgressBar->setValue(100);
    QTimer::singleShot(1000, _chaptersProgressBar, SLOT(close(void)));
    QTimer::singleShot(1000, _chaptersDownloadedLabel, SLOT(hide(void)));

    _pagesProgressBar->setValue(100);
    QTimer::singleShot(1000, _pagesProgressBar, SLOT(close(void)));
    QTimer::singleShot(1000, _pagesDownloadedLabel, SLOT(hide(void)));

    emit initModelRequested(_selectLineEdit->text());
    emit downloading(false);
    return;
  }

  QPair<QString, QString> titleAndUrl = _downloadQueue.dequeue();
  _currentChapter = titleAndUrl.first;
  QString urlString = titleAndUrl.second;

  _messageModel->editMessageInformation("Next download will begin shortly...");
  _chaptersDownloadedLabel->setText("Chapter "+QString::number(_totalCount-_downloadQueue.size())+"/"+QString::number(_totalCount));

  QStringList arguments;
  QString pathToChapterScan = Utils::getScansDirectory().absolutePath()+"/"+_selectLineEdit->text();
  arguments << urlString << pathToChapterScan << _currentChapter;

  _downloadChapterProcess->start(Utils::getScriptsAbsolutePath()+"/downloadChapter.sh", arguments);
}

void MangaDownloadWidget::getDownloadInfo(void) {
  QString downloadOutput(_downloadChapterProcess->readAllStandardOutput());
  QStringList pageRatioList = downloadOutput.split("/");

  float ip = pageRatioList.at(0).toFloat();
  float np = pageRatioList.at(1).toFloat();

  int chapterAdvance = static_cast<int>(ip*100.f / np);
  _pagesProgressBar->setValue(chapterAdvance);
  _pagesDownloadedLabel->setText("Page "+downloadOutput);

  float nc = static_cast<float>(_totalCount);
  int mangaStep = static_cast<int>(ip*100.f / (nc*np));

  float ic = static_cast<float>(_totalCount-_downloadQueue.size()-1);
  int chaptersAdvance = static_cast<int>(ic*100.f / nc);

  _chaptersProgressBar->setValue(chaptersAdvance + mangaStep);

}


/// Go to read

void MangaDownloadWidget::goToRead(QModelIndex modelIndex) {
  QString mangaName = _selectLineEdit->text();
  QString chapterName = _chaptersOnPCModel->data(modelIndex, Qt::DisplayRole).toString();

  emit chapterSelected(mangaName, chapterName);
}


/// Update database

void MangaDownloadWidget::updatedb(void) {
  Utils::addChapter(_selectLineEdit->text(), _currentChapter);
}


/// Stop download

void MangaDownloadWidget::stopDownload(void) {

//  _downloadChapterProcess->close();

//  _totalCount = 0;
//  _downloadedCount = 0;
//  _chaptersOnWebView->setEnabled(true);
//  _progressBar->setValue(0);
//  _progressBar->setVisible(false);
//  _chaptersDownloadedLabel->setText("");
//  _downloadQueue.clear();

//  updatedb();
//  updateChapters();
//  updateChaptersOnPCView();

//  emit initModelRequested();
}


/// Key release event

void MangaDownloadWidget::keyReleaseEvent(QKeyEvent* event) {
  // Enter released
  if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
    QModelIndex index = _chaptersOnPCView->currentIndex();
    if (index.isValid())
      goToRead(index);
  }
}
