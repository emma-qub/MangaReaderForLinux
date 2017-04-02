#ifndef MANGALISTWIDGET_H
#define MANGALISTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QDir>
#include <QSplitter>
#include <QListView>
#include <QProcess>
#include <QQueue>
#include <QModelIndex>
#include <QPushButton>

#include "ChapterListWidget.hxx"


class MangaListWidget: public QWidget {
  Q_OBJECT

public:
  MangaListWidget(QWidget* p_parent = nullptr);

public slots:
  void initModel(QString mangaSelected = "");
  void addManga();

protected:
  void setRemainingChaptersToRead(QStandardItem* p_mangaItem, int p_remainingChaptersCount, int p_chaptersCount);
  void setTextAccordingToRead(QStandardItem* p_mangaItem, bool p_read);
  void updateMangaData(QStandardItem* p_mangaItem);
  void updateReadProgression(QModelIndex const& p_index);
  void startNextCheck();
  void setDownloadButtonDisabled(bool b);

protected slots:
  void checkAvailableDownloads();
  void goToRead(QModelIndex const& p_modelIndex);
  void goToDownload();
  void checkAvailableChapterIsDone(int,QProcess::ExitStatus);
  void readStandardOutput();

signals:
  void chapterSelected(QString mangaName, QString chapterName);
  void mangaSelected(QString mangaName);

private:
  QDir m_scansDirectory;
  QStandardItemModel* m_mangaModel;
  QSortFilterProxyModel* m_mangaProxyModel;
  QListView* m_mangaListView;
  ChapterListWidget* m_chapterListWidget;

  QSplitter* m_mainSplitter;

  QProcess* m_checkAvailableChaptersProcess;
  QString m_currentChaptersListOnWeb;
  QQueue<QModelIndex> m_chaptersToCheck;
  QModelIndex m_currentIndex;

  QPushButton* m_checkNewChaptersButton;
  QLineEdit* m_searchLineEdit;
};

#endif // MANGALISTWIDGET_H
