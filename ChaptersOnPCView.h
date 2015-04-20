#ifndef CHAPTERSONPCVIEW_H
#define CHAPTERSONPCVIEW_H

#include <QListView>
#include <QDropEvent>
#include <QDragEnterEvent>

class ChaptersOnPCView: public QListView {
  Q_OBJECT

public:
  ChaptersOnPCView(QWidget* parent = nullptr);

  void dropEvent(QDropEvent* event);
  void dragMoveEvent(QDragEnterEvent* event);

signals:
  void newChaptersDrop(void);
};

#endif // CHAPTERSONPCVIEW_H
