#ifndef ADDMANGADIALOG_H
#define ADDMANGADIALOG_H

#include <QDialog>
#include <QDir>
#include <QProcess>

class MessageItemDelegate;
class MessageListModel;
class MessageListView;

class QLineEdit;

class AddMangaDialog: public QDialog {
  Q_OBJECT

public:
  explicit AddMangaDialog(QWidget* p_parent = nullptr);

  bool addManga();
  QStringList extractMangaInfo();

signals:

public slots:
  void searchForManga();
  void getMangaListFinished(int, QProcess::ExitStatus);
  void getMangaListStarted();

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
};

#endif // ADDMANGADIALOG_H
