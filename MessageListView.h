#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QListView>

#include "MessageListModel.h"

class MessageListView: public QListView {
  Q_OBJECT

public:
  explicit MessageListView(QWidget* parent = nullptr);

  virtual void setModel(MessageListModel* model);
};

#endif // MESSAGELISTVIEW_H
