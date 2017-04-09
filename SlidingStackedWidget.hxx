#ifndef SLIDINGSTACKEDWIDGET_HXX
#define SLIDINGSTACKEDWIDGET_HXX

#include <QStackedWidget>
#include <QEasingCurve>

class SlidingStackedWidget: public QStackedWidget {
  Q_OBJECT

public:
  enum Direction {
    eLeftToRight,
    eRightToLeft,
    eAutomatic
  };

  enum PageIndex {
    eListIndex,
    eReadIndex
  };

  SlidingStackedWidget(QWidget* p_parent);

public slots:
  void slideInNext();
  void slideInPrev();

signals:
  void animationFinished();

protected slots:
  void animationDone();

protected:
  void slideInWidget(int p_index, Direction p_direction = eAutomatic);

private:
  QWidget* m_mainWindow;
  int m_speed;
  QEasingCurve::Type m_animationType;
  bool m_vertical;
  int m_now;
  int m_next;
  QPoint m_pointNow;
  bool m_active;
  QList<QWidget*> m_blockedPageList;
};

#endif // SLIDINGSTACKEDWIDGET_HXX
