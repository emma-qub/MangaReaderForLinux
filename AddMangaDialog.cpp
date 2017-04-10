#include "AddMangaDialog.h"

#include "Utils.h"
#include "MessageListModel.h"
#include "MessageListView.h"

#include <cmath>
#include <iostream>

#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QTextStream>

#define cerro(x) std::cerr << x <<std::endl;

AddMangaDialog::AddMangaDialog(QWidget* p_parent):
  QDialog(p_parent),
  m_mangaList() {

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

  m_messageListModel = new MessageListModel(this);
  m_messageListView = new MessageListView;
  m_messageListView->setModel(m_messageListModel);

  /// Finish button
  m_finishButton = new QPushButton("Finish");
  m_finishButton->setFixedWidth(100);
  connect(m_finishButton, SIGNAL(clicked()), this, SLOT(accept()));

  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(m_finishButton);
  buttonsLayout->setAlignment(Qt::AlignRight);

  /// Main layout
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(addMangaLayout);
  mainLayout->addWidget(detailsLabel);
  mainLayout->addWidget(m_messageListView);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->setAlignment(Qt::AlignLeft);

  setLayout(mainLayout);

  resize(800, 500);
}

void AddMangaDialog::searchForManga() {

  QStringList arguments;
  arguments << m_mangaNameLineEdit->text();
  m_listMangaProcess->start(Utils::getScriptsAbsolutePath()+"/searchIfMangaExistsOnWeb.sh", arguments);
}

void AddMangaDialog::getMangaListFinished(int status, QProcess::ExitStatus exitStatus)
{
  Q_UNUSED(exitStatus)
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

  return mangaInfoList;
}
