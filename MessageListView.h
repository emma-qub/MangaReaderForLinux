#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QListView>
#include <QStringListModel>

class MessageListView: public QListView {
  Q_OBJECT

public:
  explicit MessageListView(QWidget* parent = NULL);

};

#endif // MESSAGELISTVIEW_H
