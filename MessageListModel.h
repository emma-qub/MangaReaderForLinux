#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class MessageListModel: public QStandardItemModel {
  Q_OBJECT

public:
  explicit MessageListModel(QObject *parent = nullptr, int maxLine = 10000);

  virtual void append(QStandardItem* item, bool newLine = true);

  inline void setMaxLines(int maxLines) { _maxLines = maxLines; }
  inline  int getMaxLines(void) const   { return _maxLines;     }

signals:

public slots:
  virtual void clearMessages(void);

protected:
  int _maxLines;
};

#endif // MESSAGELISTMODEL_H
