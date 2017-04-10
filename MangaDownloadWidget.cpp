#include "MangaDownloadWidget.h"

#include "Utils.h"
#include "MessageListView.h"
#include "MessageItemDelegate.h"
#include "MessageListModel.h"

#include <QLineEdit>
#include <QCompleter>
#include <QFormLayout>
#include <QStringListModel>
#include <QGroupBox>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QStandardItemModel>


MangaDownloadWidget::MangaDownloadWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_scansDirectory(Utils::getScansDirectory()),
  m_mangaList(Utils::dirList(m_scansDirectory)),
  m_currentMangaDirectory(),
  m_currentChapter(),
  m_downloadQueue(),
  m_chaptersQueue(),
  m_downloadedCount(0),
  m_totalCount(0) {


  /// Select manga line edit
  m_selectLineEdit = new QLineEdit;
  m_selectLineEdit->setFixedWidth(250);
  connect(m_selectLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkMangaNameExists(QString)));
//  connect(m_selectLineEdit, SIGNAL(editingFinished()), this, SLOT(updateChaptersOnPCView()));
//  connect(m_selectLineEdit, SIGNAL(returnPressed()), this, SLOT(updateChaptersOnPCView()));

  QCompleter* completer = new QCompleter(m_mangaList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setCompletionMode(QCompleter::PopupCompletion);

  m_selectLineEdit->setCompleter(completer);

  QFormLayout* chooseMangaLayout = new QFormLayout;
  chooseMangaLayout->addRow("Select your manga:", m_selectLineEdit);


  /// Chapters on PC

  m_chaptersOnPCModel = new QStringListModel;

  m_chaptersOnPCView = new ChaptersOnPCView;
  m_chaptersOnPCView->setModel(m_chaptersOnPCModel);
  m_chaptersOnPCView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // m_chaptersOnPCView->setDragDropMode(QAbstractItemView::DropOnly);
  connect(m_chaptersOnPCView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(goToRead(QModelIndex)));

  QVBoxLayout* chaptersOnPCLayout = new QVBoxLayout;
  chaptersOnPCLayout->addWidget(m_chaptersOnPCView);

  QGroupBox* chaptersOnPCGroupBox = new QGroupBox("Chapters on your computer");
  chaptersOnPCGroupBox->setLayout(chaptersOnPCLayout);


  /// Chapters on web

  m_chaptersOnWebModel = new QStandardItemModel;

  m_chaptersOnWebView = new ChaptersOnWebView(this);
  m_chaptersOnWebView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  m_chaptersOnWebView->setModel(m_chaptersOnWebModel);
  m_chaptersOnWebView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QVBoxLayout* chaptersOnWebLayout = new QVBoxLayout;
  chaptersOnWebLayout->addWidget(m_chaptersOnWebView);

  QGroupBox* chaptersOnWebGroupBox = new QGroupBox("New chapters online");
  chaptersOnWebGroupBox->setLayout(chaptersOnWebLayout);


  /// Central buttons

  m_updateButton = new QPushButton;
  m_updateButton->setIcon(QIcon(Utils::getIconsPath()+"/reload.png"));
  m_updateButton->setFixedWidth(37);
  connect(m_updateButton, SIGNAL(clicked()), this, SLOT(updateChaptersList()));

  m_downloadButton = new QPushButton;
  m_downloadButton->setIcon(QIcon(Utils::getIconsPath()+"/leftArrow.gif"));
  m_downloadButton->setFixedWidth(37);
  connect(m_downloadButton, SIGNAL(clicked()), this, SLOT(downloadChapters()));

  m_selectAllButton = new QPushButton;
  m_selectAllButton->setIcon(QIcon(Utils::getIconsPath()+"/selectAll.gif"));
  m_selectAllButton->setFixedWidth(37);
  connect(m_selectAllButton, SIGNAL(clicked()), m_chaptersOnWebView, SLOT(selectAll()));

  QVBoxLayout* buttonsWebLayout = new QVBoxLayout;
  buttonsWebLayout->addWidget(m_updateButton);
  buttonsWebLayout->addWidget(m_downloadButton);
  buttonsWebLayout->addWidget(m_selectAllButton);
  buttonsWebLayout->setAlignment(Qt::AlignVCenter);


  /// Chapters on PC, central buttons and chapters on web layout

  QHBoxLayout* viewsLayout = new QHBoxLayout;
  viewsLayout->addWidget(chaptersOnPCGroupBox);
  viewsLayout->addLayout(buttonsWebLayout);
  viewsLayout->addWidget(chaptersOnWebGroupBox);


  /// Message output

  m_messageView = new MessageListView;
  m_messageItemDelegate = new MessageItemDelegate;
  m_messageModel = new MessageListModel;
  m_messageView->setItemDelegate(m_messageItemDelegate);
  m_messageView->setModel(m_messageModel);


  /// Bottom buttons

  m_clearTextEditButton = new QPushButton;
  m_clearTextEditButton->setIcon(QIcon(Utils::getIconsPath()+"/clean.png"));
  m_clearTextEditButton->setFixedWidth(37);
  connect(m_clearTextEditButton, SIGNAL(clicked()), m_messageModel, SLOT(clearMessages()));

  m_stopButton = new QPushButton;
  m_stopButton->setIcon(QIcon(Utils::getIconsPath()+"/stop.png"));
  m_stopButton->setFixedWidth(37);
  m_stopButton->setEnabled(false);
  connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stopDownload()));


  /// Progress bars

  m_chaptersProgressBar = new QProgressBar;
  m_chaptersProgressBar->setVisible(false);
  m_chaptersProgressBar->setFixedSize(300, 20);

  m_chaptersDownloadedLabel = new QLabel;

  m_pagesProgressBar = new QProgressBar;
  m_pagesProgressBar->setVisible(false);
  m_pagesProgressBar->setFixedSize(300, 20);

  m_pagesDownloadedLabel = new QLabel;


  /// Bottom buttons and progress bar layout

  QFormLayout* chaptersProgressLayout = new QFormLayout;
  chaptersProgressLayout->addRow(m_chaptersProgressBar, m_chaptersDownloadedLabel);
  chaptersProgressLayout->setAlignment(Qt::AlignLeft);

  QFormLayout* pagesProgressLayout = new QFormLayout;
  pagesProgressLayout->addRow(m_pagesProgressBar, m_pagesDownloadedLabel);
  pagesProgressLayout->setAlignment(Qt::AlignLeft);

  QVBoxLayout* progressLayout = new QVBoxLayout;
  progressLayout->addLayout(chaptersProgressLayout);
  progressLayout->addLayout(pagesProgressLayout);

  QHBoxLayout* messageButtonLayout = new QHBoxLayout;
  messageButtonLayout->addWidget(m_clearTextEditButton);
  messageButtonLayout->addWidget(m_stopButton);
  messageButtonLayout->addLayout(progressLayout);
  messageButtonLayout->setAlignment(Qt::AlignLeft);

  QVBoxLayout* messageLayout = new QVBoxLayout;
  messageLayout->addLayout(messageButtonLayout);
  messageLayout->addWidget(m_messageView);
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

  m_getChaptersListProcess = new QProcess(this);
  connect(m_getChaptersListProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(chaptersListUpdated(int,QProcess::ExitStatus)));
  connect(m_getChaptersListProcess, SIGNAL(started()), this, SLOT(chaptersListUpdateStarted()));
  connect(m_getChaptersListProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(getChaptersListUpdated()));


  /// Download chapters process

  m_downloadChapterProcess = new QProcess(this);
  connect(m_downloadChapterProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(downloadFinished(int,QProcess::ExitStatus)));
  connect(m_downloadChapterProcess, SIGNAL(started()), this, SLOT(nextDownloadHasStarted()));
  connect(m_downloadChapterProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(getDownloadInfo()));


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

void MangaDownloadWidget::updateChaptersList() {
  if (m_selectLineEdit->text().isEmpty()) {
    m_messageModel->editMessageWarning("Warning: no manga name specified.");
    m_messageModel->editMessageError("Abort.");
    return;
  }

  m_chaptersOnWebModel->clear();

  updateChaptersOnPCView();
  m_chaptersOnPCView->setFocus();

  QStringList arguments;
  arguments << m_selectLineEdit->text();
  m_getChaptersListProcess->start(Utils::getScriptsAbsolutePath()+"/updateChaptersList.sh", arguments);
}


/// Check if manga name exists

void MangaDownloadWidget::checkMangaNameExists(QString mangaName) {
  bool mangaNameExists = m_mangaList.contains(mangaName);
  QString color;
  if (!mangaNameExists) {
    color = "#C9302C";
  } else {
    color = "#000000";
  }

  m_selectLineEdit->setStyleSheet(m_selectLineEdit->styleSheet()+"color: "+color+";");

  m_downloadButton->setEnabled(mangaNameExists);
  m_updateButton->setEnabled(mangaNameExists);
}


/// Search for download

void MangaDownloadWidget::searchForDownload(QString mangaName) {
  m_selectLineEdit->setText(mangaName);
  updateChaptersList();
}


/// Update chapters on PC

void MangaDownloadWidget::updateChaptersOnPCView() {
  if (m_selectLineEdit->text().isEmpty())
    return;

  m_currentMangaDirectory = QDir(m_scansDirectory.path()+"/"+m_selectLineEdit->text());
  m_chaptersOnPCModel->setStringList(Utils::dirList(m_currentMangaDirectory, true));

  m_chaptersOnPCView->setFocus();
}


/// Update chapters list SLOTS

void MangaDownloadWidget::chaptersListUpdated(int status, QProcess::ExitStatus exitStatus) {
  switch (exitStatus) {
  case QProcess::CrashExit: {
    m_messageModel->editMessageWarning("Warning: Chapters list update failed. Status code: "+QString::number(status));
    break;
  }
  case QProcess::NormalExit: {
    if (status == 1) {
      m_messageModel->editMessageWarning("Warning: "+m_getChaptersListProcess->readAllStandardError());
      m_messageModel->editMessageError("Abort.");
    } else if (status == 0) {
      m_messageModel->editMessageSuccess("Chapters list updated.");
    }
    break;
  }
  }

  if (m_chaptersOnWebModel->rowCount() == 0) {
    QStandardItem* upToDateItem = new QStandardItem;
    upToDateItem->setData("Already up-to-date.", Qt::DisplayRole);
    upToDateItem->setEditable(false);
    upToDateItem->setSelectable(false);
    m_chaptersOnWebModel->appendRow(upToDateItem);
  }

  m_selectLineEdit->setEnabled(true);
  m_updateButton->setEnabled(true);
  m_downloadButton->setEnabled(true);
  m_selectAllButton->setEnabled(true);
}

void MangaDownloadWidget::chaptersListUpdateStarted() {
  m_selectLineEdit->setEnabled(false);
  m_updateButton->setEnabled(false);
  m_downloadButton->setEnabled(false);
  m_selectAllButton->setEnabled(false);
  m_messageModel->editMessageInformation("Updating chapters list...");
}

void MangaDownloadWidget::getChaptersListUpdated() {
  QString chaptersList(m_getChaptersListProcess->readAllStandardOutput());

  QStringList chaptersUrlAndTitleList = chaptersList.split("\n", QString::SkipEmptyParts);

  for (const QString& chapterUrlAndTitle: chaptersUrlAndTitleList) {
    QStringList chapterUrlAndTitleList = chapterUrlAndTitle.split(";");

    if (chapterUrlAndTitleList.size() != 2) {
      m_messageModel->editMessageError("Error: enable to build chapters list.");
      m_messageModel->editMessageError("Abort.");
      m_chaptersOnWebModel->clear();
      return;
    }

    QString chapterTitle = chapterUrlAndTitleList.at(1).trimmed();
    QString chapterUrl = chapterUrlAndTitleList.at(0).trimmed();
    QString chapterTitleInUrl;
    if (chapterUrl.endsWith('/')) {
      chapterUrl.truncate(chapterUrl.length()-1);
    }
    chapterTitleInUrl = chapterUrl.split('/').last();

    if (!m_chaptersOnPCModel->stringList().contains(chapterTitleInUrl)) {
      QStandardItem* chapterItem = new QStandardItem;
      chapterItem->setData(chapterTitle, Qt::DisplayRole);
      chapterItem->setData(chapterUrl, Qt::UserRole);
      chapterItem->setData(chapterTitleInUrl, Qt::UserRole+1);
      m_chaptersOnWebModel->appendRow(chapterItem);
    }
  }
}


/// Download process SLOTS

void MangaDownloadWidget::downloadFinished(int status, QProcess::ExitStatus exitStatus) {
  switch (exitStatus) {
  case QProcess::CrashExit: {
    m_messageModel->editMessageWarning("Warning: Downlaod failed. Status code: "+QString::number(status));
    break;
  }
  case QProcess::NormalExit: {
    ++m_downloadedCount;
    m_chaptersOnWebModel->removeRow(m_chaptersQueue.takeFirst().row());
    updatedb();
    updateChaptersOnPCView();
    m_messageModel->editMessageSuccess("Downlaod succeeded. Status code: "+QString::number(status));
    break;
  }
  }

  startNextDownload();
}

void MangaDownloadWidget::nextDownloadHasStarted() {
  m_messageModel->editMessageInformation("Downloading "+m_currentChapter+"...");
  m_chaptersProgressBar->show();
  m_chaptersProgressBar->setValue(static_cast<int>(static_cast<float>(m_downloadedCount)/static_cast<float>(m_totalCount)*100));

  m_pagesProgressBar->show();
  m_pagesProgressBar->setValue(0);
}

void MangaDownloadWidget::downloadChapters() {
  if (m_selectLineEdit->text().isEmpty()) {
    m_messageModel->editMessageWarning("Warning: no manga name specified.");
    m_messageModel->editMessageError("Abort.");
    return;
  }

  m_selectLineEdit->setEnabled(false);
  m_chaptersOnWebView->setEnabled(false);
  m_selectAllButton->setEnabled(false);
  m_downloadButton->setEnabled(false);
  m_updateButton->setEnabled(false);
  m_stopButton->setEnabled(true);
  m_chaptersProgressBar->show();
  m_chaptersProgressBar->setValue(0);
  m_chaptersDownloadedLabel->show();
  m_chaptersDownloadedLabel->setText("Chapter");
  m_pagesProgressBar->show();
  m_pagesProgressBar->setValue(0);
  m_pagesDownloadedLabel->show();
  m_pagesDownloadedLabel->setText("Page");

  m_messageModel->editMessageInformation("Gathering information on chapters to downlaod...");
  QModelIndexList chaptersSelectedIndexes = m_chaptersOnWebView->selectionModel()->selectedIndexes();
  for (const QModelIndex& chapterIndex: chaptersSelectedIndexes) {
    m_chaptersQueue.enqueue(chapterIndex);
    QString currChapterUrl = m_chaptersOnWebModel->data(chapterIndex, Qt::UserRole).toString();
    QString currChapterTitle = m_chaptersOnWebModel->data(chapterIndex, Qt::UserRole+1).toString();
    m_downloadQueue.enqueue(QPair<QString, QString>(currChapterTitle, currChapterUrl));
  }

  m_totalCount = m_downloadQueue.size();
  m_downloadedCount = 0;

  emit downloading(true);

  startNextDownload();
}

void MangaDownloadWidget::startNextDownload() {
  if (m_downloadQueue.isEmpty()) {
    QString downloadReport = QString::number(m_downloadedCount)+"/"+QString::number(m_totalCount)+" chapter(s) downloaded successfully.";
    if (m_downloadedCount == m_totalCount) {
      m_messageModel->editMessageSuccess(downloadReport);
    } else {
      m_messageModel->editMessageWarning("Only "+downloadReport);
      m_messageModel->editMessageWarning("Please check out each issue and retry if necessary.");
    }

    updateChaptersList();

    m_totalCount = 0;
    m_downloadedCount = 0;

    m_selectLineEdit->setEnabled(true);
    m_chaptersOnWebView->setEnabled(true);
    m_selectAllButton->setEnabled(true);
    m_downloadButton->setEnabled(true);
    m_updateButton->setEnabled(true);
    m_stopButton->setEnabled(false);

    m_chaptersProgressBar->setValue(100);
    QTimer::singleShot(1000, m_chaptersProgressBar, SLOT(close()));
    QTimer::singleShot(1000, m_chaptersDownloadedLabel, SLOT(hide()));

    m_pagesProgressBar->setValue(100);
    QTimer::singleShot(1000, m_pagesProgressBar, SLOT(close()));
    QTimer::singleShot(1000, m_pagesDownloadedLabel, SLOT(hide()));

    emit initModelRequested(m_selectLineEdit->text());
    emit downloading(false);
    return;
  }

  QPair<QString, QString> titleAndUrl = m_downloadQueue.dequeue();
  m_currentChapter = titleAndUrl.first;
  QString urlString = titleAndUrl.second;

  m_messageModel->editMessageInformation("Next download will begin shortly...");
  m_chaptersDownloadedLabel->setText("Chapter "+QString::number(m_totalCount-m_downloadQueue.size())+"/"+QString::number(m_totalCount));

  QStringList arguments;
  QString pathToChapterScan = Utils::getScansDirectory().absolutePath()+"/"+m_selectLineEdit->text();
  arguments << urlString << pathToChapterScan << m_currentChapter;

  m_downloadChapterProcess->start(Utils::getScriptsAbsolutePath()+"/downloadChapter.sh", arguments);
}

void MangaDownloadWidget::getDownloadInfo() {
  QString downloadOutput(m_downloadChapterProcess->readAllStandardOutput());
  QStringList pageRatioList = downloadOutput.split("/");

  float ip = pageRatioList.at(0).toFloat();
  float np = pageRatioList.at(1).toFloat();

  int chapterAdvance = static_cast<int>(ip*100.f / np);
  m_pagesProgressBar->setValue(chapterAdvance);
  m_pagesDownloadedLabel->setText("Page "+downloadOutput);

  float nc = static_cast<float>(m_totalCount);
  int mangaStep = static_cast<int>(ip*100.f / (nc*np));

  float ic = static_cast<float>(m_totalCount-m_downloadQueue.size()-1);
  int chaptersAdvance = static_cast<int>(ic*100.f / nc);

  m_chaptersProgressBar->setValue(chaptersAdvance + mangaStep);

}


/// Go to read

void MangaDownloadWidget::goToRead(QModelIndex modelIndex) {
  QString mangaName = m_selectLineEdit->text();
  QString chapterName = m_chaptersOnPCModel->data(modelIndex, Qt::DisplayRole).toString();

  emit chapterSelected(mangaName, chapterName);
}


/// Update database

void MangaDownloadWidget::updatedb() {
  Utils::addChapter(m_selectLineEdit->text(), m_currentChapter);
}


/// Stop download

void MangaDownloadWidget::stopDownload() {

//  m_downloadChapterProcess->close();

//  m_totalCount = 0;
//  m_downloadedCount = 0;
//  m_chaptersOnWebView->setEnabled(true);
//  m_progressBar->setValue(0);
//  m_progressBar->setVisible(false);
//  m_chaptersDownloadedLabel->setText("");
//  m_downloadQueue.clear();

//  updatedb();
//  updateChapters();
//  updateChaptersOnPCView();

//  emit initModelRequested();
}


/// Key release event

void MangaDownloadWidget::keyReleaseEvent(QKeyEvent* p_event) {
  // Enter released
  if ((p_event->key() == Qt::Key_Enter) || (p_event->key() == Qt::Key_Return)) {
    QModelIndex index = m_chaptersOnPCView->currentIndex();
    if (index.isValid())
      goToRead(index);
  }
}
