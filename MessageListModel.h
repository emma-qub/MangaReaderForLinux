#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class MessageListModel: public QStandardItemModel {
  Q_OBJECT

public:
  enum MessageStatus {
    Information,
    Success,
    Warning,
    Error,
    Download
  };

  explicit MessageListModel(QObject *parent = nullptr, int maxLine = 10000);

  inline void editMessageSuccess(QString message, bool newLine = true) {
    append(message, MessageStatus::Success, newLine);
  }

  inline void editMessageInformation(QString message, bool newLine = true) {
    append(message, MessageStatus::Information, newLine);
  }

  inline void editMessageWarning(QString message, bool newLine = true) {
    append(message, MessageStatus::Warning, newLine);
  }

  inline void editMessageError(QString message, bool newLine = true) {
    append(message, MessageStatus::Error, newLine);
  }

  inline void setMaxLines(int maxLines) { _maxLines = maxLines; }
  inline  int getMaxLines(void) const   { return _maxLines;     }

protected:
  virtual void append(QString message, MessageStatus messageStatus, bool newLine = true);

signals:
  void scrollToBottomRequested(void);

public slots:
  virtual void clearMessages(void);

protected:
  int _maxLines;
};

#endif // MESSAGELISTMODEL_H
