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
  void checkMangaName(void);
  void finish(void);
  void downloadPreviewImage(void);
  void updateMessage(QString message, DownloadManager::MessageStatus messageStatus, bool newLine = true);

private:
  QLineEdit* _mangaNameLineEdit;
  QPushButton* _addMangaButton;
  QPushButton* _cancelButton;
  QPushButton* _finishButton;

  MessageItemDelegate* _messageItemDelegate;
  MessageListModel* _messageListModel;
  MessageListView* _messageListView;

  DownloadHTMLManager _downloadMangaListManager;
  DownloadHTMLManager _downloadMangaInfoManager;
  DownloadManager _downloadManager;

  QTime _downloadTime;
  QFile _output;
  QNetworkReply* _networkReply;

  QDir _scansDirectory;
};

#endif // ADDMANGADIALOG_H
