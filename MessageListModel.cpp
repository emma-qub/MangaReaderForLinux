#include "MessageListModel.h"

#include "MessageItemDelegate.h"

MessageListModel::MessageListModel(QObject* parent, int maxLine):
  QStandardItemModel(parent),
  m_maxLines(maxLine) {
}

void MessageListModel::append(QString message, MessageStatus messageStatus, bool newLine) {
  QStandardItem* item = new QStandardItem;

  switch(messageStatus) {
  case MessageStatus::Information:
    item->setData(message, MessageItemDelegate::InformationRole);
    break;
  case MessageStatus::Success:
    item->setData(message, MessageItemDelegate::SuccessRole);
    break;
  case MessageStatus::Warning:
    item->setData(message, MessageItemDelegate::WarningRole);
    break;
  case MessageStatus::Error:
    item->setData(message, MessageItemDelegate::ErrorRole);
    break;
  case MessageStatus::Download:
    item->setData(message, MessageItemDelegate::DownloadRole);
    break;
  default:
    item->setData(message, MessageItemDelegate::DownloadRole);
    break;
  }

  while (rowCount() > m_maxLines)
    removeRow(0);

  if (!newLine)
    removeRow(rowCount()-1);

  appendRow(item);

  emit scrollToBottomRequested();
}

void MessageListModel::clearMessages() {
  clear();
}
