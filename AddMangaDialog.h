#ifndef ADDMANGADIALOG_H
#define ADDMANGADIALOG_H

#include <QtWidgets>
#include <QNetworkReply>

#include "MessageItemDelegate.h"
#include "MessageListModel.h"
#include "MessageListView.h"
#include "DownloadManager.h"

class AddMangaDialog: public QDialog {
  Q_OBJECT

public:
  explicit AddMangaDialog(QWidget* parent = nullptr);

  bool addManga();
  QStringList extractMangaInfo();

signals:

public slots:
  void searchForManga();
  void getMangaListFinished(int,QProcess::ExitStatus);
  void getMangaListStarted();
//  void checkMangaName();
//  void finish();
//  void downloadPreviewImage();

private:
  QProcess* m_listMangaProcess;

  QDir m_scansDirectory;

  QStringList m_mangaList;

  QLineEdit* m_mangaNameLineEdit;
  QPushButton* m_addMangaButton;

  MessageItemDelegate* m_messageItemDelegate;
  MessageListModel* m_messageListModel;
  MessageListView* m_messageListView;

  QPushButton* m_finishButton;
  //  QPushButton* m_cancelButton;

//  DownloadHTMLManager m_downloadMangaListManager;
//  DownloadHTMLManager m_downloadMangaInfoManager;
//  DownloadManager m_downloadManager;

//  QTime m_downloadTime;
//  QFile m_output;
//  QNetworkReply* m_networkReply;

};

#endif // ADDMANGADIALOG_H
