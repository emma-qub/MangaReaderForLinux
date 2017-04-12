#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QListView>

class MessageListModel;

class MessageListView: public QListView {
  Q_OBJECT

public:
  explicit MessageListView(QWidget* parent = nullptr);

  void setModel(QAbstractItemModel* p_model) override;
};

#endif // MESSAGELISTVIEW_H
