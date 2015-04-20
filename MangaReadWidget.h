#ifndef MANGAREADWIDGET_H
#define MANGAREADWIDGET_H

#include <QtWidgets>
#include <QDir>

class MangaReadWidget : public QWidget {
  Q_OBJECT

public:
  MangaReadWidget(QWidget* parent = nullptr);

private:
  QLineEdit* _selectLineEdit;
  QComboBox* _mangasComboBox;
  QDir _scansDirectory;
  QDir _currentMangaDirectory;
  QDir _currentChapterDirectory;
  QComboBox* _chaptersComboBox;
  QComboBox* _pagesComboBox;
  QLabel* _nbPagesLabel;
  QLabel* _currentPageLabel;
  QStringList _chapterStringList;
  int _currentPageNumber;
  int _nbPagesCurrCh;
  QScrollArea* _scrollArea;
  QPushButton* _zoomButton;
  QLabel* _zoomLabel;
  QPoint _mousePosition;
  QComboBox* _zoomComboBox;

public slots:
  void updateChaptersComboBox(QString mangaName);
  void updatePagesComboBox(void);
  void changeChapter(QString chapterName);
  void changePage(int pageNumber);
  void updateCurrentPage(void);
  void switchManga(QString mangaName, QString chapterName);

protected:
  void keyReleaseEvent(QKeyEvent* event);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent* event);

signals:
  void chapterComboBoxUpdated(void);
  void chapterSelected(QString, QString);
};

#endif // MANGAREADWIDGET_H
