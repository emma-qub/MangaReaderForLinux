#include "MessageListView.hxx"

#include "MessageListModel.hxx"

MessageListView::MessageListView(QWidget* parent):
  QListView(parent) {

  setAlternatingRowColors(true);
  setSelectionMode(QAbstractItemView::NoSelection);
}

void MessageListView::setModel(QAbstractItemModel* p_model) {
  QListView::setModel(p_model);

  auto model = dynamic_cast<MessageListModel*>(p_model);
  if (model != nullptr) {
    connect(model, &MessageListModel::scrollToBottomRequested, this, &MessageListView::scrollToBottom);
  }
}
