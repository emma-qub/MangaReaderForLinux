#include "MessageListModel.h"

MessageListModel::MessageListModel(QObject* parent) :
    QStandardItemModel(parent),
    _maxLines(10000) {
}

void MessageListModel::append(QStandardItem* item, bool newLine) {
    while (rowCount() > _maxLines)
        removeRow(0);

    if (!newLine)
        removeRow(rowCount()-1);

    appendRow(item);
}
