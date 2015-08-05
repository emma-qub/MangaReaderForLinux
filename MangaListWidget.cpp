#include <QDebug>

#include "MangaListWidget.h"
#include "Utils.h"
#include "AddMangaDialog.h"


MangaListWidget::MangaListWidget(QWidget* parent):
  QWidget(parent),
  _scansDirectory(Utils::getScansDirectory()) {

  _mangaPreviewLabel = new QLabel("Manga Preview");
  _mangaPreviewLabel->setFixedHeight(400);
  _mangaPreviewLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
  _mangaPreviewLabel->setStyleSheet("margin: 20px;");

  _genreLabel = new QLabel;
  _genreLabel->setFont(QFont("Monospace", 8));
  _authorLabel = new QLabel;
  _authorLabel->setFont(QFont("Monospace", 8));
  _artistLabel = new QLabel;
  _artistLabel->setFont(QFont("Monospace", 8));
  _publisherLabel = new QLabel;
  _publisherLabel->setFont(QFont("Monospace", 8));
  _magazineLabel = new QLabel;
  _magazineLabel->setFont(QFont("Monospace", 8));
  _startDateLabel = new QLabel;
  _startDateLabel->setFont(QFont("Monospace", 8));

  QFormLayout* genreLayout = new QFormLayout;
  genreLayout->addRow("Genre:", _genreLabel);
  QFormLayout* authorLayout = new QFormLayout;
  authorLayout->addRow("Author:", _authorLabel);
  QFormLayout* artistLayout = new QFormLayout;
  artistLayout->addRow("Artist:", _artistLabel);
  QFormLayout* publisherLayout = new QFormLayout;
  publisherLayout->addRow("Publisher:", _publisherLabel);
  QFormLayout* magazineLayout = new QFormLayout;
  magazineLayout->addRow("Magazine:", _magazineLabel);
  QFormLayout* startDateLayout = new QFormLayout;
  startDateLayout->addRow("Start date:", _startDateLabel);

  QVBoxLayout* mangaInfoLayout = new QVBoxLayout;
  mangaInfoLayout->addWidget(_mangaPreviewLabel);
  mangaInfoLayout->addLayout(genreLayout);
  mangaInfoLayout->addLayout(authorLayout);
  mangaInfoLayout->addLayout(artistLayout);
  mangaInfoLayout->addLayout(publisherLayout);
  mangaInfoLayout->addLayout(magazineLayout);
  mangaInfoLayout->addLayout(startDateLayout);
  mangaInfoLayout->setAlignment(Qt::AlignTop);

  QGroupBox* mangaInfoGroupBox = new QGroupBox("Information");
  mangaInfoGroupBox->setLayout(mangaInfoLayout);
  mangaInfoGroupBox->setFixedWidth(350);
  mangaInfoGroupBox->setAlignment(Qt::AlignLeft);

  _model = new QStandardItemModel;
  initModel();

  _chapterInfoWidget = new ChapterInfoWidget;

  _view = new QColumnView;
  QList<int> widths;
  widths << 300 << 400 << 400;
  _view->setColumnWidths(widths);
  _view->setResizeGripsVisible(false);
  _view->setFixedWidth(1102);
  _view->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _view->setModel(_model);
  _view->setPreviewWidget(_chapterInfoWidget);
  connect(_view, SIGNAL(updatePreviewWidget(QModelIndex)), this, SLOT(updateChaptersInfo(QModelIndex)));
  connect(_view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateMangaInfo(QModelIndex)));
  connect(_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(goToRead(QModelIndex)));

  QHBoxLayout* listLayout = new QHBoxLayout;
  listLayout->addWidget(mangaInfoGroupBox);
  listLayout->addWidget(_view);

  QStringList headerLabels;
  headerLabels << "Manga";
  _model->setHorizontalHeaderLabels(headerLabels);

  _markReadButton = new QPushButton;
  _markReadButton->setIcon(QIcon(Utils::getIconsPath()+"/check.gif"));
  _markReadButton->setFixedWidth(37);
  connect(_markReadButton, SIGNAL(clicked()), this, SLOT(markRead()));

  _markUnreadButton = new QPushButton;
  _markUnreadButton->setIcon(QIcon(Utils::getIconsPath()+"/uncheck.gif"));
  _markUnreadButton->setFixedWidth(37);
  connect(_markUnreadButton, SIGNAL(clicked()), this, SLOT(markUnread()));

  _downloadButton = new QPushButton;
  _downloadButton->setIcon(QIcon(Utils::getIconsPath()+"/download.gif"));
  _downloadButton->setFixedWidth(37);
  connect(_downloadButton, SIGNAL(clicked()), this, SLOT(goToDownload()));

  _addMangaButton = new QPushButton;
  _addMangaButton->setIcon(QIcon(Utils::getIconsPath()+"/plus.png"));
  _addMangaButton->setFixedWidth(37);
  connect(_addMangaButton, SIGNAL(clicked()), this, SLOT(addManga()));

  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(_markReadButton);
  buttonsLayout->addWidget(_markUnreadButton);
  buttonsLayout->addWidget(_downloadButton);
  buttonsLayout->addWidget(_addMangaButton);
  buttonsLayout->setAlignment(Qt::AlignHCenter);

  QLabel* titleLabel = new QLabel("Manga List");
  titleLabel->setFont(QFont("", 18, 99));

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(titleLabel);
  layout->addLayout(buttonsLayout);
  layout->addLayout(listLayout);

  setLayout(layout);
}

void MangaListWidget::initModel(QString mangaSelected) {
  _scansDirectory.setFilter(QDir::Dirs);
  QStringList dirsList = Utils::dirList(_scansDirectory);

  _model->removeRows(0, _model->rowCount());

  QModelIndex indexMangaSelected;

  for (const QString& mangaName: dirsList) {
    QString currDirStr = _scansDirectory.path() + "/" + mangaName;

    QStandardItem* currItem = new QStandardItem(mangaName);
    currItem->setEditable(false);
    _model->appendRow(currItem);

    QDir currDir(currDirStr);
    QStringList currDirsList = Utils::dirList(currDir, true);

    QList<bool> areChaptersRead = Utils::areChaptersRead(mangaName);
    if (!Utils::isMangaRead(areChaptersRead))
      currItem->setFont(QFont("", -1, 99));
    else
      currItem->setFont(QFont());

    if (mangaName == mangaSelected) {
      indexMangaSelected = _model->indexFromItem(currItem);
    }

    int k = 0;
    for (const QString& currChStr: currDirsList) {
      if (k >= areChaptersRead.size()) {
        QMessageBox::critical(this, "List error", "Error while tempting to edit manga read flags whithin MangaListWidget::initModel.");
        return;
      }
      bool isChapterRead = areChaptersRead.at(k);

      QStandardItem* currChItem = new QStandardItem(currChStr);
      currChItem->setEditable(false);
      setTextAccordingToRead(currChItem, isChapterRead);

      currItem->appendRow(currChItem);

      k++;
    }
  }

  if (indexMangaSelected.isValid()) {
    _view->selectionModel()->setCurrentIndex(indexMangaSelected, QItemSelectionModel::Current);
  }
}

void MangaListWidget::setTextAccordingToRead(QStandardItem* item, bool read) {
  if (read) {
    item->setFont(QFont(""));
    item->setData(QColor("#eee"), Qt::BackgroundRole);
  } else {
    item->setFont(QFont("", -1, 99));
    item->setData(QColor("#fff"), Qt::BackgroundRole);
  }


}

void MangaListWidget::markRead(void) {
  markReadOrNot(true);
}

void MangaListWidget::markUnread(void) {
  markReadOrNot(false);
}

void MangaListWidget::markReadOrNot(bool read) {
  for (int k = 0; k < _model->rowCount(); k++) {
    QStandardItem* currManga = _model->itemFromIndex(_model->index(k, 0));
    if (_view->selectionModel()->isSelected(_model->indexFromItem(currManga))) {
      for (int i = 0; i < currManga->rowCount(); i++) {
        QStandardItem* currChapter = currManga->child(i, 0);
        updateChapterRead(currChapter, read);
      }
      setTextAccordingToRead(currManga, read);
    } else {
      for (int i = 0; i < currManga->rowCount(); i++) {
        QStandardItem* currChapter = currManga->child(i, 0);
        if (_view->selectionModel()->isSelected(_model->indexFromItem(currChapter))) {
          updateChapterRead(currChapter, read);
        }
      }
    }
  }

  checkIfMangaAreRead();
}

void MangaListWidget::checkIfMangaAreRead(void) {
  for (int k = 0; k < _model->rowCount(); k++) {
    QStandardItem* currManga = _model->itemFromIndex(_model->index(k, 0));

    QList<bool> areChaptersRead = Utils::areChaptersRead(currManga->text());
    if (!Utils::isMangaRead(areChaptersRead))
      currManga->setFont(QFont("", -1, 99));
    else
      currManga->setFont(QFont());
  }
}

void MangaListWidget::updateChapterRead(QStandardItem* chapterItem, bool read) {
  setTextAccordingToRead(chapterItem, read);
  Utils::updateChapterRead(chapterItem->parent()->text(), chapterItem->text(), read);
}

void MangaListWidget::goToRead(QModelIndex modelIndex) {
  QStandardItem* currentItem = _model->itemFromIndex(modelIndex);
  QStandardItem* currentItemParent = currentItem->parent();

  if (currentItemParent == NULL)
    return;

  markRead();

  emit chapterSelected(currentItemParent->text(), currentItem->text());
}

void MangaListWidget::goToDownload(void) {
  QStandardItem* currentItem = _model->itemFromIndex(_view->currentIndex());

  if (!currentItem) {
    QMessageBox::warning(this, "Warning", "No manga selected.");
    return;
  }

  QString mangaName;
  currentItem->parent() == NULL ? mangaName = currentItem->text() : mangaName = currentItem->parent()->text();

  emit mangaSelected(mangaName);
}

void MangaListWidget::addManga(void) {
  AddMangaDialog* addMangandialog = new AddMangaDialog(this);
  switch(addMangandialog->exec()) {
  case QDialog::Accepted:
    initModel();
    break;
  case QDialog::Rejected:
    break;
  }
}

void MangaListWidget::updateChaptersInfo(QModelIndex index) {
  QStandardItem* currentItem = _model->itemFromIndex(index);
  QDir chapterDir = _scansDirectory;

  if (!currentItem->parent())
    return;

  chapterDir.cd(currentItem->parent()->text());
  chapterDir.cd(currentItem->text());

  QStringList pagesList = Utils::filesList(chapterDir);

  if (pagesList.size() == 0)
    return;

  qint64 totalSize = 0;
  for (const QString& pageName: pagesList) {
    QFile currFile(chapterDir.path()+"/"+pageName);
    totalSize += currFile.size();
  }
  double chapterSize = totalSize/(1024.*1024.);

  QFileInfo chapterDirInfo(chapterDir.path());

  _chapterInfoWidget->setNumberFiles(QString::number(pagesList.size()));
  _chapterInfoWidget->setTotalSize(QString::number(chapterSize, 'f', 2)+"MB");
  _chapterInfoWidget->setLastModification(chapterDirInfo.lastModified().toString("ddd dd MMM yyyy"));
  _chapterInfoWidget->setLastRead(chapterDirInfo.lastRead().toString("ddd dd MMM yyyy"));

  if (pagesList.size() > 0)
    _chapterInfoWidget->setImgPreview(QPixmap(chapterDir.path()+"/"+pagesList.at(0)));
  else
    _chapterInfoWidget->setImgPreview(QPixmap());
}

void MangaListWidget::updateMangaInfo(QModelIndex index) {
  QStandardItem* currentItem = _model->itemFromIndex(index);

  if (!currentItem->parent()) {
    QDir mangaDirectory(_scansDirectory.path()+"/"+currentItem->text());

    if (mangaDirectory.exists("cover.png")) {
      QPixmap pixmap(mangaDirectory.path()+"/cover.png");
      pixmap.scaled(_mangaPreviewLabel->width(), _mangaPreviewLabel->height(), Qt::KeepAspectRatio);
      _mangaPreviewLabel->setPixmap(pixmap);
    }

    QFile file(mangaDirectory.filePath("mangaInfo.txt"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Can't open" << file.fileName() << "in read only whithin MangaListWidget::updateMangaInfo.";
      _genreLabel->setText("");
      _authorLabel->setText("");
      _artistLabel->setText("");
      _publisherLabel->setText("");
      _magazineLabel->setText("");
      _startDateLabel->setText("");
      return;
    }

    QTextStream inFile(&file);
    inFile.setCodec("UTF-8");

    QStringList infosList = inFile.readAll().split("\n", QString::SkipEmptyParts);
    for (const QString& info: infosList) {
      if (info.isEmpty())
        break;

      QStringList newInfoList = info.split(":");
      if (newInfoList.size() < 2) {
        qDebug() << "Check" << mangaDirectory.filePath("mangaInfo.txt") << "file may be corrupted.";
        break;
      }

      QString newInfo = newInfoList.at(1);

      QStringList resultList;
      if (info.startsWith("Genre")) {
        resultList = newInfo.split(" ");

        while (resultList.size() > 6)
          resultList.removeLast();
        if (resultList.size() > 3) {
          resultList.insert(3, "\n");
          _genreLabel->setText(resultList.join(" "));
        } else {
          _genreLabel->setText(newInfo);
        }
      } else if (info.startsWith("Author"))
        _authorLabel->setText(newInfo);
      else if (info.startsWith("Artist"))
        _artistLabel->setText(newInfo);
      else if (info.startsWith("Publisher"))
        _publisherLabel->setText(newInfo);
      else if (info.startsWith("Magazine")) {
        resultList = newInfo.split(" ");

        if (resultList.size() > 3) {
          resultList.insert(3, "\n");
          _magazineLabel->setText(resultList.join(" "));
        } else {
          _magazineLabel->setText(newInfo);
        }
      } else if (info.startsWith("Start Date"))
        _startDateLabel->setText(newInfo);
    }
  }
}

void MangaListWidget::updateReadChapter(QString mangaName, QString chapterName) {
  for (int k = 0; k < _model->rowCount(); k++) {
    QStandardItem* currManga = _model->itemFromIndex(_model->index(k, 0));
    if (currManga->text() == mangaName) {
      for (int i = 0; i < currManga->rowCount(); i++) {
        QStandardItem* currChapter = currManga->child(i, 0);
        if (currChapter->text() == chapterName) {
          setTextAccordingToRead(currChapter, true);
          break;
        }
      }
    }
  }
}

void MangaListWidget::setDownloadButtonDisabled(bool b) {
  _downloadButton->setDisabled(b);
}

void MangaListWidget::keyReleaseEvent(QKeyEvent* event) {
  // Enter released
  if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
    QModelIndex index = _view->currentIndex();
    QStandardItem* currentItem = _model->itemFromIndex(index);

    if (currentItem && currentItem->parent())
      goToRead(index);
  }

  // Up arrow or Down arrow released
  if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
    QModelIndex index = _view->currentIndex();
    QStandardItem* currentItem = _model->itemFromIndex(index);

    if (currentItem && !currentItem->parent()) {
      updateMangaInfo(index);
    }
  }
}
