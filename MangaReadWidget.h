#ifndef MANGAREADWIDGET_H
#define MANGAREADWIDGET_H

#include <QtWidgets>
#include <QDir>

class MangaReadWidget : public QWidget {
  Q_OBJECT

public:
  MangaReadWidget(QWidget* parent = nullptr);

private:
  QLineEdit* m_selectLineEdit;
  QComboBox* m_mangasComboBox;
  QDir m_scansDirectory;
  QDir m_currentMangaDirectory;
  QDir m_currentChapterDirectory;
  QComboBox* m_chaptersComboBox;
  QComboBox* m_pagesComboBox;
  QLabel* m_nbPagesLabel;
  QLabel* m_currentPageLabel;
  QStringList m_chapterStringList;
  int m_currentPageNumber;
  int m_nbPagesCurrCh;
  QScrollArea* m_scrollArea;
  QPushButton* m_zoomButton;
  QLabel* m_zoomLabel;
  QPoint m_mousePosition;
  QComboBox* m_zoomComboBox;

public slots:
  void updateChaptersComboBox(QString mangaName);
  void updatePagesComboBox();
  void changeChapter(QString chapterName);
  void changePage(int pageNumber);
  void updateCurrentPage();
  void switchManga(QString mangaName, QString chapterName);

protected:
  void keyReleaseEvent(QKeyEvent* event);
  void mouseReleaseEvent(QMouseEvent* );
  void mouseMoveEvent(QMouseEvent* event);

signals:
  void chapterComboBoxUpdated();
  void chapterSelected(QString, QString);
};

#endif // MANGAREADWIDGET_H
