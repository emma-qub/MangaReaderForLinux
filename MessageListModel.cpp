#include "MessageListModel.h"

MessageListModel::MessageListModel(QObject* parent, int maxLine):
  QStandardItemModel(parent),
  _maxLines(maxLine) {
}

void MessageListModel::append(QStandardItem* item, bool newLine) {
  while (rowCount() > _maxLines)
    removeRow(0);

  if (!newLine)
    removeRow(rowCount()-1);

  appendRow(item);
}

void MessageListModel::clearMessages(void) {
  clear();
}
