#ifndef DOWNLOADER_HXX
#define DOWNLOADER_HXX

#include <QObject>
#include <QProcess>
#include <QStack>

class QStandardItem;

class Downloader: public QObject {
  Q_OBJECT

public:
  enum ChapterRoles {
    eChapterURLRole = Qt::UserRole,
    eChapterTitleInURLRole
  };

  explicit Downloader(QObject* p_parent = nullptr);
  ~Downloader() override;

signals:
  void chaptersListfinished(int p_status, QProcess::ExitStatus p_exitStatus);
  void chaptersListStarted();
  void chaptersListFetched(QStandardItem* p_newChapterItem);
  void downloading(bool p_downloading);
  void chapterDownloadAdvanced(QStandardItem* p_currentChapterItem, int p_advancement);
  void currentChapterItemAboutToBeDeleted(QStandardItem* p_currentChapterItem);

public slots:
  void fetchChaptersList(const QString& p_mangaName);
  void readyReadStandardOutput();
  void downloadAvailableChapters(QList<QStandardItem*> const& chapterIndexes);

protected:
  void updatedb();
  void cleanCurrentChapterItem();

protected slots:
  void startNextDownload();
  void getDownloadInfo();

private:
  QProcess* m_fetchChaptersListProcess;
  QProcess* m_downloadChapterProcess;
  QStack<QStandardItem*> m_chaptersStack;
  int m_totalCount;
  int m_downloadedCount;
  QString m_currentMangaName;
  QStandardItem* m_currentItem;
};

#endif // DOWNLOADER_HXX
