#ifndef NOTIFICATIONDIALOG_H
#define NOTIFICATIONDIALOG_H

#include <QWidget>

class QLabel;

class NotificationDialog: public QWidget {
  Q_OBJECT

public:
  NotificationDialog(const QString& iconPath, QWidget* parent = nullptr);

  void showPopup(const QString& title, const QString& message);

protected:
  void paintEvent(QPaintEvent* event) override;

signals:

private:
  QLabel* m_iconLabel;
  QLabel* m_titleLabel;
  QLabel* m_messageLabel;

  qreal m_opacity;
};

#endif // NOTIFICATIONDIALOG_H
