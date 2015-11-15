#include "AddMangaDialog.h"

#include "Utils.h"

#include <cmath>
#include <iostream>

#include <QDebug>
//#include <QNetworkRequest>

#define cerro(x) std::cerr << x <<std::endl;

AddMangaDialog::AddMangaDialog(QWidget* parent):
  QDialog(parent),
  _mangaList()/*,
  _downloadMangaListManager(),
  _downloadMangaInfoManager(),
  _downloadManager(),
  _output()*/ {


  /// List manga process

  _listMangaProcess = new QProcess(this);
  connect(_listMangaProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(getMangaListFinished(int,QProcess::ExitStatus)));
  connect(_listMangaProcess, SIGNAL(started(void)), this, SLOT(getMangaListStarted(void)));


  /// Scan directory

  _scansDirectory = Utils::getScansDirectory();


  /// Manga name line edit

  _mangaNameLineEdit = new QLineEdit;
  _mangaNameLineEdit->setFixedWidth(250);


  /// Add manga button

  _addMangaButton = new QPushButton("Add");
  _addMangaButton->setFixedWidth(100);
  connect(_addMangaButton, SIGNAL(clicked()), this, SLOT(searchForManga()));

  QFormLayout* addMangaLayout = new QFormLayout;
  addMangaLayout->addRow(_mangaNameLineEdit, _addMangaButton);


  /// Output message

  QLabel* detailsLabel = new QLabel("Details");

  _messageItemDelegate = new MessageItemDelegate;
  _messageListModel = new MessageListModel(this);
  _messageListView = new MessageListView;
  _messageListView->setItemDelegate(_messageItemDelegate);
  _messageListView->setModel(_messageListModel);


  /// Finish button

  _finishButton = new QPushButton("Finish");
  _finishButton->setFixedWidth(100);
  connect(_finishButton, SIGNAL(clicked(void)), this, SLOT(accept(void)));

//  _cancelButton = new QPushButton("Cancel");
//  _cancelButton->setFixedWidth(100);
//  connect(_cancelButton, SIGNAL(clicked(void)), this, SLOT(reject(void)));

  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(_finishButton);
//  buttonsLayout->addWidget(_cancelButton);
  buttonsLayout->setAlignment(Qt::AlignRight);


  /// Main layout

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(addMangaLayout);
  mainLayout->addWidget(detailsLabel);
  mainLayout->addWidget(_messageListView);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->setAlignment(Qt::AlignLeft);

  setLayout(mainLayout);

//  connect(&_downloadMangaListManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(updateMessage(QString, DownloadManager::MessageStatus, bool)));
//  connect(&_downloadMangaListManager, SIGNAL(done(void)), this, SLOT(checkMangaName(void)));

//  connect(&_downloadMangaInfoManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(updateMessage(QString, DownloadManager::MessageStatus, bool)));
//  connect(&_downloadMangaInfoManager, SIGNAL(done(void)), this, SLOT(downloadPreviewImage(void)));

//  connect(&_downloadManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(updateMessage(QString, DownloadManager::MessageStatus, bool)));
//  connect(&_downloadManager, SIGNAL(done()), this, SLOT(finish()));

  resize(800, 500);
}

void AddMangaDialog::searchForManga(void) {

  QStringList arguments;
  arguments << _mangaNameLineEdit->text();
  _listMangaProcess->start(Utils::getScriptsAbsolutePath()+"/searchIfMangaExistsOnWeb.sh", arguments);


//  QStringList urls("http://eatmanga.com/Manga-Scan/");
//  _downloadMangaListManager.clean();
  //  _downloadMangaListManager.append(urls);
}

void AddMangaDialog::getMangaListFinished(int status, QProcess::ExitStatus exitStatus)
{
  cerro(status);
  return;
}

void AddMangaDialog::getMangaListStarted()
{

}


bool AddMangaDialog::addManga() {
  // Create database text file
  QFile dbTxtFile(_scansDirectory.path()+"/"+_mangaNameLineEdit->text()+"/db.txt");
  if (!dbTxtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    _messageListModel->editMessageError("Error: cannot create db.txt. Please retry.");
    return false;
  } else {
    _messageListModel->editMessageSuccess("Database text has been successfuly created.");
    dbTxtFile.close();
  }

  // Create manga info text file
  QFile mangaInfoTxtFile(_scansDirectory.path()+"/"+_mangaNameLineEdit->text()+"/mangaInfo.txt");
  if (!mangaInfoTxtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    _messageListModel->editMessageError("Error: cannot create mangaInfo.txt. Please retry.");
    return false;
  } else {
    // Extract manga information
    QStringList mangaInfoList = extractMangaInfo();

    if (mangaInfoList.size() == 0) {
      _messageListModel->editMessageWarning("Warning: no manga information found.\nYou will have to complete corresponding mangaInfo.txt manually.");
    } else {
      QTextStream inFile(&mangaInfoTxtFile);
      inFile << mangaInfoList.join("\n");
      _messageListModel->editMessageSuccess("Manga information text has been successfuly created.");
    }

    mangaInfoTxtFile.close();
  }

  return true;
}

QStringList AddMangaDialog::extractMangaInfo(void) {
  QStringList mangaInfoList;

//  QStringList mangaInfoPages = _downloadMangaInfoManager.getHTMLPages();
//  if (mangaInfoPages.size() == 0) {
//    _messageListModel->editMessageWarning("Warning: can't get source code, please retry. Maybe you are not connected to the Internet.");
//  } else if (mangaInfoPages.at(0).contains("Genre:")) {
//    QString mangaInfoPage = mangaInfoPages.at(0);

//    mangaInfoList << "Genre:"+mangaInfoPage.split("<th>Genre:</th>").at(1).split("<td>").at(1).split("</td>").at(0);
//    mangaInfoList << "Author:"+mangaInfoPage.split("<th>Author:</th>").at(1).split("<td>").at(1).split("</td>").at(0);
//    mangaInfoList << "Artist:"+mangaInfoPage.split("<th>Artist:</th>").at(1).split("<td>").at(1).split("</td>").at(0);
//    mangaInfoList << "Publisher:"+mangaInfoPage.split("<th>Publisher:</th>").at(1).split("<td>").at(1).split("</td>").at(0);
//    mangaInfoList << "Magazine:"+mangaInfoPage.split("<th>Magazine:</th>").at(1).split("<td>").at(1).split("</td>").at(0);
//    mangaInfoList << "Start Date:"+mangaInfoPage.split("<th>Start Date:</th>").at(1).split("<td>").at(1).split("</td>").at(0);
//  } else {
//    mangaInfoList << "Genre:";
//    mangaInfoList << "Author:";
//    mangaInfoList << "Artist:";
//    mangaInfoList << "Publisher:";
//    mangaInfoList << "Magazine:";
//    mangaInfoList << "Start Date:";
//  }

  return mangaInfoList;
}














//void AddMangaDialog::checkMangaName(void) {
//  QStringList htmlPages = _downloadMangaListManager.getHTMLPages();
//  if (htmlPages.size() == 0) {
//    _messageListModel->editMessageWarning("Warning: can't get source code, please retry. Maybe you are not connected to the Internet.");
//    return;
//  } else {
//    _messageListModel->editMessageSuccess("Manga name has been found.");
//    QString htmlPage = htmlPages.at(0);
//    if (!htmlPage.contains("href=\"/Manga-Scan/"+_mangaNameLineEdit->text())) {
//      _messageListModel->editMessageWarning("Warning: specified manga name is not on web site. Please check out your spelling and retry.");
//      return;
//    } else {
//      // Create new manga directory
//      if (!_scansDirectory.mkdir(_mangaNameLineEdit->text())) {
//        QMessageBox* mb = new QMessageBox(QMessageBox::Critical, "Directory error", "Error: cannot create "+_mangaNameLineEdit->text()+" directory.\nAbort.");
//        mb->setModal(true);
//        mb->setAttribute(Qt::WA_DeleteOnClose);
//        mb->show();
//        reject();
//      }
//      _messageListModel->editMessageSuccess("New manga directory has been successfuly created.");

//      QStringList urls("http://eatmanga.com/Manga-Scan/"+_mangaNameLineEdit->text());
//      _downloadMangaInfoManager.clean();
//      _downloadMangaInfoManager.append(urls);
//    }
//  }
//}

//void AddMangaDialog::downloadPreviewImage(void) {
//  _scansDirectory.mkdir("Manga-Scan");
//  _scansDirectory.cd("Manga-Scan");
//  _scansDirectory.mkdir(_mangaNameLineEdit->text());

//  QStringList urls("http://eatmanga.com/cache/mangas/Manga-Scan/"+_mangaNameLineEdit->text()+"/cover.jpg");

//  _downloadManager.clean();
//  _downloadManager.append(urls);
//}

//void AddMangaDialog::finish(void) {
//  _scansDirectory.cd(Utils::getScansDirectory().path());
//  _scansDirectory.rename(_scansDirectory.path()+"/Manga-Scan/"+_mangaNameLineEdit->text()+"/cover.jpg",
//                         _scansDirectory.path()+"/"+_mangaNameLineEdit->text()+"/cover.png");
//  _scansDirectory.rmdir("Manga-Scan/"+_mangaNameLineEdit->text());
//  _scansDirectory.rmdir("Manga-Scan");

//  if (addManga()) {
//    _messageListModel->editMessageSuccess(_mangaNameLineEdit->text()+" has been successfuly added to manga list.");
//  } else {
//    _scansDirectory.rmdir(_mangaNameLineEdit->text());
//  }
//}
