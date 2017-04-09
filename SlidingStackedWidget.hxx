#ifndef SLIDINGSTACKEDWIDGET_HXX
#define SLIDINGSTACKEDWIDGET_HXX

#include <QStackedWidget>
#include <QEasingCurve>

class SlidingStackedWidget: public QStackedWidget {
  Q_OBJECT

public:
  enum Direction {
    eLeftToRight,
    eRightToLeft
  };

  explicit SlidingStackedWidget(QWidget* p_parent);

public slots:
  void slideInNext();
  void slideInPrev();

protected slots:
  void animationDone();

protected:
  void slideInWidget(int p_index, Direction p_direction);

private:
  static int const SPEED;
  static QEasingCurve::Type const ANIMATION_TYPE;
  int m_now;
  int m_next;
  QPoint m_pointNow;
  bool m_active;
};

#endif // SLIDINGSTACKEDWIDGET_HXX
