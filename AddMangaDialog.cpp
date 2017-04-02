#include "AddMangaDialog.h"

#include "Utils.h"

#include <cmath>
#include <iostream>

#include <QDebug>
//#include <QNetworkRequest>

#define cerro(x) std::cerr << x <<std::endl;

AddMangaDialog::AddMangaDialog(QWidget* parent):
  QDialog(parent),
  m_mangaList()/*,
  m_downloadMangaListManager(),
  m_downloadMangaInfoManager(),
  m_downloadManager(),
  m_output()*/ {


  /// List manga process

  m_listMangaProcess = new QProcess(this);
  connect(m_listMangaProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(getMangaListFinished(int,QProcess::ExitStatus)));
  connect(m_listMangaProcess, SIGNAL(started()), this, SLOT(getMangaListStarted()));


  /// Scan directory

  m_scansDirectory = Utils::getScansDirectory();


  /// Manga name line edit

  m_mangaNameLineEdit = new QLineEdit;
  m_mangaNameLineEdit->setFixedWidth(250);


  /// Add manga button

  m_addMangaButton = new QPushButton("Add");
  m_addMangaButton->setFixedWidth(100);
  connect(m_addMangaButton, SIGNAL(clicked()), this, SLOT(searchForManga()));

  QFormLayout* addMangaLayout = new QFormLayout;
  addMangaLayout->addRow(m_mangaNameLineEdit, m_addMangaButton);


  /// Output message

  QLabel* detailsLabel = new QLabel("Details");

  m_messageItemDelegate = new MessageItemDelegate;
  m_messageListModel = new MessageListModel(this);
  m_messageListView = new MessageListView;
  m_messageListView->setItemDelegate(m_messageItemDelegate);
  m_messageListView->setModel(m_messageListModel);


  /// Finish button

  m_finishButton = new QPushButton("Finish");
  m_finishButton->setFixedWidth(100);
  connect(m_finishButton, SIGNAL(clicked()), this, SLOT(accept()));

//  m_cancelButton = new QPushButton("Cancel");
//  m_cancelButton->setFixedWidth(100);
//  connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(m_finishButton);
//  buttonsLayout->addWidget(m_cancelButton);
  buttonsLayout->setAlignment(Qt::AlignRight);


  /// Main layout

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(addMangaLayout);
  mainLayout->addWidget(detailsLabel);
  mainLayout->addWidget(m_messageListView);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->setAlignment(Qt::AlignLeft);

  setLayout(mainLayout);

//  connect(&m_downloadMangaListManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(updateMessage(QString, DownloadManager::MessageStatus, bool)));
//  connect(&m_downloadMangaListManager, SIGNAL(done()), this, SLOT(checkMangaName()));

//  connect(&m_downloadMangaInfoManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(updateMessage(QString, DownloadManager::MessageStatus, bool)));
//  connect(&m_downloadMangaInfoManager, SIGNAL(done()), this, SLOT(downloadPreviewImage()));

//  connect(&m_downloadManager, SIGNAL(message(QString, DownloadManager::MessageStatus, bool)), this, SLOT(updateMessage(QString, DownloadManager::MessageStatus, bool)));
//  connect(&m_downloadManager, SIGNAL(done()), this, SLOT(finish()));

  resize(800, 500);
}

void AddMangaDialog::searchForManga() {

  QStringList arguments;
  arguments << m_mangaNameLineEdit->text();
  m_listMangaProcess->start(Utils::getScriptsAbsolutePath()+"/searchIfMangaExistsOnWeb.sh", arguments);


//  QStringList urls("http://eatmanga.com/Manga-Scan/");
//  m_downloadMangaListManager.clean();
  //  m_downloadMangaListManager.append(urls);
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
  QFile dbTxtFile(m_scansDirectory.path()+"/"+m_mangaNameLineEdit->text()+"/db.txt");
  if (!dbTxtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    m_messageListModel->editMessageError("Error: cannot create db.txt. Please retry.");
    return false;
  } else {
    m_messageListModel->editMessageSuccess("Database text has been successfuly created.");
    dbTxtFile.close();
  }

  // Create manga info text file
  QFile mangaInfoTxtFile(m_scansDirectory.path()+"/"+m_mangaNameLineEdit->text()+"/mangaInfo.txt");
  if (!mangaInfoTxtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    m_messageListModel->editMessageError("Error: cannot create mangaInfo.txt. Please retry.");
    return false;
  } else {
    // Extract manga information
    QStringList mangaInfoList = extractMangaInfo();

    if (mangaInfoList.size() == 0) {
      m_messageListModel->editMessageWarning("Warning: no manga information found.\nYou will have to complete corresponding mangaInfo.txt manually.");
    } else {
      QTextStream inFile(&mangaInfoTxtFile);
      inFile << mangaInfoList.join("\n");
      m_messageListModel->editMessageSuccess("Manga information text has been successfuly created.");
    }

    mangaInfoTxtFile.close();
  }

  return true;
}

QStringList AddMangaDialog::extractMangaInfo() {
  QStringList mangaInfoList;

//  QStringList mangaInfoPages = m_downloadMangaInfoManager.getHTMLPages();
//  if (mangaInfoPages.size() == 0) {
//    m_messageListModel->editMessageWarning("Warning: can't get source code, please retry. Maybe you are not connected to the Internet.");
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














//void AddMangaDialog::checkMangaName() {
//  QStringList htmlPages = m_downloadMangaListManager.getHTMLPages();
//  if (htmlPages.size() == 0) {
//    m_messageListModel->editMessageWarning("Warning: can't get source code, please retry. Maybe you are not connected to the Internet.");
//    return;
//  } else {
//    m_messageListModel->editMessageSuccess("Manga name has been found.");
//    QString htmlPage = htmlPages.at(0);
//    if (!htmlPage.contains("href=\"/Manga-Scan/"+m_mangaNameLineEdit->text())) {
//      m_messageListModel->editMessageWarning("Warning: specified manga name is not on web site. Please check out your spelling and retry.");
//      return;
//    } else {
//      // Create new manga directory
//      if (!m_scansDirectory.mkdir(m_mangaNameLineEdit->text())) {
//        QMessageBox* mb = new QMessageBox(QMessageBox::Critical, "Directory error", "Error: cannot create "+m_mangaNameLineEdit->text()+" directory.\nAbort.");
//        mb->setModal(true);
//        mb->setAttribute(Qt::WA_DeleteOnClose);
//        mb->show();
//        reject();
//      }
//      m_messageListModel->editMessageSuccess("New manga directory has been successfuly created.");

//      QStringList urls("http://eatmanga.com/Manga-Scan/"+m_mangaNameLineEdit->text());
//      m_downloadMangaInfoManager.clean();
//      m_downloadMangaInfoManager.append(urls);
//    }
//  }
//}

//void AddMangaDialog::downloadPreviewImage() {
//  m_scansDirectory.mkdir("Manga-Scan");
//  m_scansDirectory.cd("Manga-Scan");
//  m_scansDirectory.mkdir(m_mangaNameLineEdit->text());

//  QStringList urls("http://eatmanga.com/cache/mangas/Manga-Scan/"+m_mangaNameLineEdit->text()+"/cover.jpg");

//  m_downloadManager.clean();
//  m_downloadManager.append(urls);
//}

//void AddMangaDialog::finish() {
//  m_scansDirectory.cd(Utils::getScansDirectory().path());
//  m_scansDirectory.rename(m_scansDirectory.path()+"/Manga-Scan/"+m_mangaNameLineEdit->text()+"/cover.jpg",
//                         m_scansDirectory.path()+"/"+m_mangaNameLineEdit->text()+"/cover.png");
//  m_scansDirectory.rmdir("Manga-Scan/"+m_mangaNameLineEdit->text());
//  m_scansDirectory.rmdir("Manga-Scan");

//  if (addManga()) {
//    m_messageListModel->editMessageSuccess(m_mangaNameLineEdit->text()+" has been successfuly added to manga list.");
//  } else {
//    m_scansDirectory.rmdir(m_mangaNameLineEdit->text());
//  }
//}
