#include "ChaptersOnPCView.h"

#include <QMimeData>
#include <QDebug>

ChaptersOnPCView::ChaptersOnPCView(QWidget* parent):
  QListView(parent) {

  setDragDropMode(QAbstractItemView::DropOnly);
  setAcceptDrops(true);
}

void ChaptersOnPCView::dropEvent(QDropEvent *event) {
  qDebug() << "#" << event->mimeData()->formats();
  if (event->mimeData()->hasFormat("application/x-downloadchapter")) {
    qDebug() << "OK";
    event->accept();
    emit this->newChaptersDrop();
  }
}

void ChaptersOnPCView::dragMoveEvent(QDragEnterEvent* event) {
  qDebug() << event->mimeData()->formats();
  if (event->mimeData()->hasFormat("application/x-downloadchapter")) {
    setAcceptDrops(true);
    event->accept();
  } else {
    setAcceptDrops(false);
  }
}
