#include "SlidingStackedWidget.hxx"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

int const SlidingStackedWidget::SPEED = 1000;
QEasingCurve::Type const SlidingStackedWidget::ANIMATION_TYPE = QEasingCurve::InOutQuint;

SlidingStackedWidget::SlidingStackedWidget(QWidget* p_parent):
  QStackedWidget(p_parent),
  m_now(0),
  m_next(0),
  m_pointNow(QPoint(0, 0)),
  m_active(false) {
}

void SlidingStackedWidget::slideInNext() {
  int currIndex = currentIndex();
  if (currIndex < count()-1)
    slideInWidget(currIndex+1, eRightToLeft);
}

void SlidingStackedWidget::slideInPrev() {
  int currIndex = currentIndex();
  if (currIndex > 0)
    slideInWidget(currIndex-1, eLeftToRight);
}

void SlidingStackedWidget::slideInWidget(int p_index, Direction p_direction) {
  if (m_active) {
    return;
  } else {
    m_active = true;
  }

  int now = currentIndex();
  int next = p_index;

  int offsetX = frameRect().width();
  int offsetY = frameRect().height();
  widget(next)->setGeometry(0, 0, offsetX, offsetY);

  offsetY = 0;

  auto pointNext = widget(next)->pos();
  auto pointNow = widget(now)->pos();
  m_pointNow = pointNow;

  if (p_direction == eRightToLeft)
    widget(next)->move(pointNext.x()+offsetX, pointNext.y());
  else if (p_direction == eLeftToRight)
    widget(next)->move(pointNext.x()-offsetX, pointNext.y());
  widget(next)->show();
  widget(next)->raise();

  auto animNow = new QPropertyAnimation(widget(now), "pos");
  animNow->setDuration(SPEED);
  animNow->setEasingCurve(ANIMATION_TYPE);
  animNow->setStartValue(pointNow);
  if (p_direction == eRightToLeft)
    animNow->setEndValue(QPoint(pointNow.x()-offsetX, pointNow.y()));
  else if (p_direction == eLeftToRight)
    animNow->setEndValue(QPoint(pointNow.x()+offsetX, pointNow.y()));

  auto animNext = new QPropertyAnimation(widget(next), "pos");
  animNext->setDuration(SPEED);
  animNext->setEasingCurve(ANIMATION_TYPE);
  if (p_direction == eRightToLeft)
    animNext->setStartValue(QPoint(pointNext.x()+offsetX, pointNext.y()));
  else if (p_direction == eLeftToRight)
    animNext->setStartValue(QPoint(pointNext.x()-offsetX, pointNext.y()));
  animNext->setEndValue(pointNext);

  auto animationGroup = new QParallelAnimationGroup;
  animationGroup->addAnimation(animNow);
  animationGroup->addAnimation(animNext);
  connect(animationGroup, &QParallelAnimationGroup::finished, this, &SlidingStackedWidget::animationDone);

  m_next = next;
  m_now = now;
  m_active = true;
  animationGroup->start();
}

void SlidingStackedWidget::animationDone() {
  setCurrentIndex(m_next);
  widget(m_now)->hide();
  widget(m_now)->move(m_pointNow);
  m_active = false;
}
