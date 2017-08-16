#ifndef CHAPTERLISTWIDGET_HXX
#define CHAPTERLISTWIDGET_HXX

#include <QWidget>

class FrontCoverOverlay: public QWidget {
  Q_OBJECT

public:
  enum DownloadState {
    eDownloadAvailable,
    eUpdatingChapterList,
    eDownloadingChapters,
    eUpToDate
  };

  explicit FrontCoverOverlay(QWidget* p_parent = nullptr);

  void setFrontCover(QPixmap const& p_frontCover);
  void setMangaName(QString const& p_mangaName);
  void setAvailableDownloadCount(int p_availableDownloadCount);
  void setReadProgression(float p_percentageRead);
  void chapterUpdateStarted();

protected:
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent* p_event) override;
  void mouseMoveEvent(QMouseEvent* p_event) override;

signals:
  void downloadRequested();

private:
  int m_availableDownloadCount;
  float m_percentageRead;
  bool m_enteredButton;
  bool m_hasToRepaint;
  QColor m_textButtonColor;
  QString m_mangaName;
  QPixmap m_frontCover;
  QRect m_buttonRect;
};





#include "Utils.hxx"

class QStandardItem;
class QStandardItemModel;
class QTreeView;
class QLabel;

class Downloader;

class ChapterListWidget: public QWidget {
  Q_OBJECT

public:
  enum ChapterListColumns {
    eChapterNameColumn = 0,
    eChapterProgressBarColumn,
    eChapterReadColumn
  };
  enum ChapterDataRole {
    eChapterReadRole = Qt::UserRole
  };

  ChapterListWidget(QWidget* p_parent = nullptr);
  void setReadPercentage(int p_chaptersReadCount, int p_allChaptersCount);
  void setAvailableDownloadCount(const QModelIndex& p_index);

public slots:
  void changeManga(const QModelIndex& p_index);
  void markChapterAsRead(QString const& p_chapterName);
  void updateChapterAdvancement(QStandardItem* p_item, int p_advancement);

signals:
  void chapterSelected(QModelIndex const& p_chapterIndex);
  void downloadRequested();
  void progressionChanged(int p_remainingChaptersToRead);

protected:
  void updateReadState(QStandardItem* p_stateItem, bool p_isChapterRead, bool p_hasToUpdatedb);
  QModelIndex getChapterIndex(QModelIndex const& p_index);
  void keyReleaseEvent(QKeyEvent* p_event) override;
  void updateChapters();
  void updateTitle();

protected slots:
  void fetchChaptersList();
  void updateChaptersList(QStandardItem* p_chapterItem);
  void startDownload();
  void cleanCurrentChapterItem(QStandardItem* p_currentChapterItem);

private:
  int m_chaptersReadCount;
  int m_allChaptersCount;
  QString m_currentMangaName;
  FrontCoverOverlay* m_frontCover;
  QStandardItemModel* m_chaptersModel;
  QTreeView* m_chaptersView;
  QLabel* m_chaptersTitleLabel;
  Downloader* m_downloader;
  QList<QStandardItem*> m_chaptersToDownloadList;
};

#endif // CHAPTERLISTWIDGET_HXX
