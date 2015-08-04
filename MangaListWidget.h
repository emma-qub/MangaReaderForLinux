#ifndef MANGALISTWIDGET_H
#define MANGALISTWIDGET_H

#include <QtWidgets>
#include <QDir>

#include "ChapterInfoWidget.h"

class MangaListWidget : public QWidget {
  Q_OBJECT

public:
  MangaListWidget(QWidget* parent = nullptr);

  void markReadOrNot(bool read);
  void setTextAccordingToRead(QStandardItem* item, bool read);
  void updateChapterRead(QStandardItem* chapterItem, bool read);
  void checkIfMangaAreRead(void);

protected:
  virtual void keyReleaseEvent(QKeyEvent* event);

public slots:
  void markRead(void);
  void markUnread(void);
  void initModel(QString mangaSelected = "");
  void goToRead(QModelIndex modelIndex);
  void goToDownload(void);
  void addManga(void);
  void updateChaptersInfo(QModelIndex index);
  void updateMangaInfo(QModelIndex index);
  void updateReadChapter(QString mangaName, QString chapterName);
  void setDownloadButtonDisabled(bool b);

signals:
  void chapterSelected(QString mangaName, QString chapterName);
  void mangaSelected(QString mangaName);

private:
  QDir _scansDirectory;
  QPushButton* _markReadButton;
  QPushButton* _markUnreadButton;
  QPushButton* _downloadButton;
  QPushButton* _addMangaButton;
  QLabel* _mangaPreviewLabel;
  QLabel* _genreLabel;
  QLabel* _authorLabel;
  QLabel* _artistLabel;
  QLabel* _publisherLabel;
  QLabel* _magazineLabel;
  QLabel* _startDateLabel;
  QStandardItemModel* _model;
  QColumnView* _view;
  ChapterInfoWidget* _chapterInfoWidget;

};

#endif // MANGALISTWIDGET_H
