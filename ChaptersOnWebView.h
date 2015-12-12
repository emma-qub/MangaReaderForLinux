#ifndef CHAPTERSONWEBVIEW_H
#define CHAPTERSONWEBVIEW_H

#include <QListView>

class ChaptersOnWebView: public QListView {

public:
  ChaptersOnWebView(QWidget* parent = nullptr);

  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);

private:
  QPoint _dragStartPosition;
};

#endif // CHAPTERSONWEBVIEW_H
