#include "ChaptersOnWebView.h"

#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

ChaptersOnWebView::ChaptersOnWebView(QWidget* parent):
  QListView(parent),
  _dragStartPosition() {

  setDragDropMode(QAbstractItemView::DragOnly);
}

void ChaptersOnWebView::mousePressEvent(QMouseEvent* event) {
  QListView::mousePressEvent(event);

  if (event->button() == Qt::LeftButton)
    _dragStartPosition = event->pos();
}

void ChaptersOnWebView::mouseMoveEvent(QMouseEvent* event) {
  if (!(event->buttons() == Qt::LeftButton))
      return;

  if ((event->pos() - _dragStartPosition).manhattanLength()
     < QApplication::startDragDistance())
    return;

  QDrag* drag = new QDrag(this);
  QMimeData* mimeData = new QMimeData;

  mimeData->setData("application/x-downloadchapter", QByteArray());
  drag->setMimeData(mimeData);

  drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
}
