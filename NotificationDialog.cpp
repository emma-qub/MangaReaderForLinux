#include "NotificationDialog.h"

#include <iostream>

NotificationDialog::NotificationDialog(const QString& iconPath, QWidget* parent):
  QWidget(parent) {

  _iconLabel = new QLabel;
  _iconLabel->setPixmap(QPixmap(iconPath).scaled(50, 50));
  _messageLabel = new QLabel(parent);
  _titleLabel = new QLabel(parent);
  QVBoxLayout* messageLayout = new QVBoxLayout;
  messageLayout->addWidget(_titleLabel);
  messageLayout->addWidget(_messageLabel);

  QHBoxLayout* bodyLayout = new QHBoxLayout;
  bodyLayout->addWidget(_iconLabel);
  bodyLayout->addLayout(messageLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  QLabel* dialogTitle = new QLabel("Manga Reader Notification");
  QString titleStyle(
    "color: black;"
    "font-weight: bold;"
    "font-size: 16px;"
  );

  dialogTitle->setStyleSheet(dialogTitle->styleSheet()+titleStyle);
  mainLayout->addWidget(dialogTitle);
  mainLayout->addLayout(bodyLayout);
  setLayout(mainLayout);

//  setAttribute(Qt::WA_ShowWithoutActivating);
  setWindowFlags(
      Qt::Window                    // Say it's a window, REALLY IMPORTANT
    | Qt::FramelessWindowHint       // No window border
    | Qt::WindowStaysOnTopHint      // Always on top
    | Qt::WindowDoesNotAcceptFocus  // No focus
  );
}

NotificationDialog::~NotificationDialog(void) {
}

void NotificationDialog::showPopup(const QString& title, const QString& message) {
  _titleLabel->setText(QString("<b>%1</b>").arg(title));
  _messageLabel->setText(message);

  // resize the widget, as text label may be larger than the previous size
  setGeometry(QStyle::alignedRect(Qt::RightToLeft, Qt::AlignBottom, size(), qApp->desktop()->availableGeometry()));
  show();

  QTimer::singleShot(2000, this, SIGNAL(hideRequested(void)));
}
