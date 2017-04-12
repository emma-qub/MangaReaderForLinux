#include "MessageListModel.hxx"

MessageListModel::MessageListModel(QObject* parent, int maxLine):
  QStandardItemModel(parent),
  m_maxLines(maxLine) {
}

void MessageListModel::append(QString message, MessageStatus messageStatus, bool newLine) {
  QStandardItem* item = new QStandardItem;
  item->setText(message);
  item->setFont(QFont("Monospace"));

  switch(messageStatus) {
  case MessageStatus::Information:
  {
    item->setData(QColor("#4169e1"), Qt::ForegroundRole);
    break;
  }
  case MessageStatus::Success:
  {
    item->setData(QColor("#228b22"), Qt::ForegroundRole);
    break;
  }
  case MessageStatus::Warning:
  {
    item->setData(QColor("#ff8c00"), Qt::ForegroundRole);
    break;
  }
  case MessageStatus::Error:
  {
    item->setData(QColor("#ff0000"), Qt::ForegroundRole);
    break;
  }
  case MessageStatus::Download:
  default:
  {
    item->setData(QColor("#333333"), Qt::ForegroundRole);
    break;
  }
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
