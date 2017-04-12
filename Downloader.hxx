#ifndef DOWNLOADER_HXX
#define DOWNLOADER_HXX

#include <QObject>
#include <QProcess>

class QStandardItem;

class Downloader: public QObject {
  Q_OBJECT

public:
  enum ChapterRoles {
    eChapterURLRole = Qt::UserRole,
    eChapterTitleInURLRole
  };

  explicit Downloader(QObject* p_parent = nullptr);

signals:
  void chaptersListfinished(int p_status, QProcess::ExitStatus p_exitStatus);
  void chaptersListStarted();
  void chaptersListUpdated(QStandardItem* p_newChapterItem);

public slots:
  void updateChaptersList(const QString& p_mangaName);
  void readyReadStandardOutput();

private:
  QProcess* m_getChaptersListProcess;
};

#endif // DOWNLOADER_HXX
