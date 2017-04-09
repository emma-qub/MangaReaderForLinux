#ifndef MANGAREADWIDGET_H
#define MANGAREADWIDGET_H

#include <QWidget>
#include <QDir>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

class MangaReadWidget: public QWidget {
  Q_OBJECT

public:
  MangaReadWidget(QWidget* p_parent = nullptr);

public slots:
  void updateChaptersComboBox(QString const& p_mangaName);
  void updatePagesComboBox();
  void changeChapter(QString const& p_chapterName);
  void changePage(int p_pageNumber);
  void updateCurrentPage();
  void switchManga(QString const& p_mangaName, QString const& p_chapterName);

protected:
  void keyReleaseEvent(QKeyEvent* p_event);
  void mouseReleaseEvent(QMouseEvent* p_event);
  void mouseMoveEvent(QMouseEvent* p_event);

signals:
  void goToListRequested();
  void currentChapterChanged(QString const& p_chapterName);

private:
  QDir m_scansDirectory;
  QDir m_currentMangaDirectory;
  QDir m_currentChapterDirectory;
  QStringList m_chapterStringList;
  int m_currentPageNumber;
  int m_nbPagesCurrCh;
  QPoint m_mousePosition;
  QScrollArea* m_scrollArea;
  QLabel* m_zoomLabel;
  QLabel* m_nbPagesLabel;
  QLabel* m_currentPageLabel;
  QPushButton* m_zoomButton;
  QPushButton* m_goToListButton;
  QComboBox* m_chaptersComboBox;
};

#endif // MANGAREADWIDGET_H
