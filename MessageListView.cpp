#include "MessageListView.h"
#include <QStringListModel>
#include <QPainter>

MessageListView::MessageListView(QWidget* parent):
  QListView(parent) {

  setAlternatingRowColors(true);
  setSelectionMode(QAbstractItemView::NoSelection);
}

void MessageListView::setModel(MessageListModel* model) {
  QListView::setModel(model);

  connect(model, SIGNAL(scrollToBottomRequested()), this, SLOT(scrollToBottom()));
}
