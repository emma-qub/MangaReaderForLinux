#include "NotificationDialog.h"


NotificationDialog::NotificationDialog(const QString& iconPath, QWidget* parent):
  QWidget(parent) {

  _iconLabel = new QLabel;
  _iconLabel->setPixmap(QPixmap(iconPath).scaled(50, 50));
  _messageLabel = new QLabel(parent);
  _titleLabel = new QLabel(parent);
  QVBoxLayout* messageLayout = new QVBoxLayout;
  messageLayout->addWidget(_titleLabel);
  messageLayout->addWidget(_messageLabel);

  QHBoxLayout* mainLayout = new QHBoxLayout;
  mainLayout->addWidget(_iconLabel);
  mainLayout->addLayout(messageLayout);
  setLayout(mainLayout);

  setAttribute(Qt::WA_ShowWithoutActivating);
  setWindowFlags(
        Qt::Window                    // Say it's a window, REALLY IMPORTANT
        | Qt::FramelessWindowHint       // No window border
        | Qt::WindowStaysOnTopHint      // Always on top
        | Qt::WindowDoesNotAcceptFocus  // No focus
  );

  setObjectName("myWidget");
  setStyleSheet(
        "QWidget#myWidget{"
        "  background-color: white;"
        "  border-left: 1px solid #aaa;"
        "  border-bottom: 1px solid #aaa;"
        "  border-right: 1px solid #aaa;"
        "}"
        "QWidget#myWidget::title {"
        "  height: 24px;"
        "  border-top: 1px solid #aaa;"
        "  border-left: 1px solid #aaa;"
        "  border-right: 1px solid #aaa;"
        "  border-top-left-radius: 5px;"
        "  border-top-right-radius: 5px;"
        "  background:"
        "    qlineargradient("
        "      x1: 0, y1: 0, x2: 0, y2: 1,"
        "      stop: 0.0 #fccd85, stop: 0.3 #fccd85,"
        "      stop: 0.3 #fe9a04, stop: 1.0 #fc6800"
        "    );"
        "}"
  );

  setWindowTitle("   Manga Reader Notification");
  setFixedSize(250, 100);
}

NotificationDialog::~NotificationDialog(void) {
}

void NotificationDialog::showPopup(const QString& title, const QString& message) {
  _titleLabel->setText(title);
  _messageLabel->setText(message);

  // resize the widget, as text label may be larger than the previous size
  setGeometry(
        (parentWidget()->geometry().right()-parentWidget()->geometry().left())/2 - width()/2,
        parentWidget()->geometry().bottom() - height(),
        width(),
        height()
  );

  show();

  QTimer::singleShot(5000, this, SLOT(hide(void)));
}

void NotificationDialog::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event)

  QPainter p(this);
  QStyle* style = this->style();
  QRect active_area = this->rect();
  int titlebar_height = 0;

  // Titlebar.
  QStyleOptionTitleBar t_opt;
  t_opt.initFrom(this);

  titlebar_height = style->pixelMetric(QStyle::PM_TitleBarHeight, &t_opt, this);

  t_opt.rect = QRect(0, 0, this->width(), titlebar_height);
  t_opt.titleBarState = this->windowState();
  t_opt.text = t_opt.fontMetrics.elidedText(this->windowTitle(), Qt::ElideRight, t_opt.rect.width());
  t_opt.palette.setBrush(QPalette::Text, QBrush(Qt::white));
  style->drawComplexControl(QStyle::CC_TitleBar, &t_opt, &p, this);
  style->drawItemText(&p, t_opt.rect, Qt::AlignLeft|Qt::AlignVCenter, t_opt.palette, true, t_opt.text, QPalette::Text);

  // Background widget.
  active_area.setTopLeft(QPoint(0, titlebar_height));
  this->setContentsMargins(0, titlebar_height, 0, 0);

  QStyleOption w_opt;
  w_opt.initFrom(this);
  w_opt.rect = active_area;
  style->drawPrimitive(QStyle::PE_Widget, &w_opt, &p, this);
}
