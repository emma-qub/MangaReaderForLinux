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

  bool addManga(void);
  QStringList extractMangaInfo(void);

signals:

public slots:
  void searchForManga(void);
  void getMangaListFinished(int,QProcess::ExitStatus);
  void getMangaListStarted(void);
//  void checkMangaName(void);
//  void finish(void);
//  void downloadPreviewImage(void);

private:
  QProcess* _listMangaProcess;

  QDir _scansDirectory;

  QStringList _mangaList;

  QLineEdit* _mangaNameLineEdit;
  QPushButton* _addMangaButton;

  MessageItemDelegate* _messageItemDelegate;
  MessageListModel* _messageListModel;
  MessageListView* _messageListView;

  QPushButton* _finishButton;
  //  QPushButton* _cancelButton;

//  DownloadHTMLManager _downloadMangaListManager;
//  DownloadHTMLManager _downloadMangaInfoManager;
//  DownloadManager _downloadManager;

//  QTime _downloadTime;
//  QFile _output;
//  QNetworkReply* _networkReply;

};

#endif // ADDMANGADIALOG_H
