#ifndef NOTIFICATIONDIALOG_H
#define NOTIFICATIONDIALOG_H

#include <QtWidgets>

class NotificationDialog: public QWidget {
  Q_OBJECT

public:
  NotificationDialog(const QString& iconPath, QWidget* parent = nullptr);
  virtual ~NotificationDialog(void);

  void showPopup(const QString& title, const QString& message);

protected:
  void paintEvent(QPaintEvent* event) override;

signals:

private:
  QLabel* _iconLabel;
  QLabel* _titleLabel;
  QLabel* _messageLabel;

  qreal _opacity;
};

#endif // NOTIFICATIONDIALOG_H
