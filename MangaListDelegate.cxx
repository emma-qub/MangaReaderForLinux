#include "MangaListDelegate.hxx"

#include <QApplication>
#include <QFont>

MangaListDelegate::MangaListDelegate(QObject* p_parent):
  QStyledItemDelegate(p_parent) {
}

void MangaListDelegate::paint(QPainter* p_painter, const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const {
  /// Initiate variables
  auto rect = p_option.rect;
  QRect circleRect(rect.left(), rect.top(), rect.height(), rect.height());
  int circlePadding = 5;
  int leftShift = circleRect.width();
  circleRect.adjust(circlePadding, circlePadding, -circlePadding, -circlePadding);
  circleRect.moveLeft(leftShift);
  QRect textRect(rect.left()+2*leftShift, rect.top(), rect.width()-4*rect.height(), rect.height());
  auto chaptersToDownload = p_index.data(eAvailableChaptersRole).toInt();
  auto isOutOfDate = chaptersToDownload > 0;
  auto chaptersToRead = p_index.data(eChaptersToReadRole).toInt();
  auto currentColor = p_index.data(eColorRole).value<QColor>();
  auto currentCircleColorSelected = currentColor;
  currentCircleColorSelected.setHsv(currentColor.hue(), currentColor.saturation()/2, currentColor.value());
  QPen circlePen(currentColor);
  circlePen.setWidth(2);
  QPen circleTransparentPen = circlePen;
  circleTransparentPen.setColor(currentCircleColorSelected);
  QBrush circleBrush(currentColor);
  QBrush circleTransparentBrush(currentCircleColorSelected);
  QRect chaptersNumberRect(rect.right() - 2*leftShift, rect.top(), 1.2*leftShift, leftShift);

  p_painter->save();

  p_painter->setRenderHints(QPainter::Antialiasing);

  /// Draw selection
  if (p_option.state & QStyle::State_Selected)
  {
    p_painter->setBrush(circleBrush);
    p_painter->setPen(Qt::NoPen);
    p_painter->drawRoundedRect(rect, 5, 5);
  }

  /// Draw the circle in front of manga name
  if (isOutOfDate) {
    p_painter->setBrush(circleBrush);
    p_painter->setPen(Qt::NoPen);
  } else {
    p_painter->setBrush(Qt::NoBrush);
    p_painter->setPen(circlePen);
  }
  p_painter->drawEllipse(circleRect);

  /// Draw the circle according to selection
  if (p_option.state & QStyle::State_Selected )
  {
    if (isOutOfDate) {
      p_painter->setBrush(circleTransparentBrush);
      p_painter->setPen(Qt::NoPen);
    } else {
      p_painter->setBrush(Qt::NoBrush);
      p_painter->setPen(circleTransparentPen);
    }
    p_painter->drawEllipse(circleRect);
  }

  /// Draw manga name
  QPen textPen(p_index.data(Qt::ForegroundRole).value<QColor>());
  p_painter->setPen(textPen);
  auto textFont = QFont(QApplication::font());
  auto fontWeight = 1;
  if (chaptersToRead > 0) {
    fontWeight = 99;
  }
  textFont.setWeight(fontWeight);
  textFont.setPointSize(14);
  p_painter->setFont(textFont);
  p_painter->drawText(textRect, p_index.data().toString(), QTextOption(Qt::AlignVCenter));

  /// Draw number of chapters to read
  if (chaptersToRead > 0)
  {
    p_painter->drawText(chaptersNumberRect, QString::number(chaptersToRead), QTextOption(Qt::AlignVCenter | Qt::AlignRight));
  }

  p_painter->restore();
}

QSize MangaListDelegate::sizeHint(QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const {
  auto oldSizeHint = QStyledItemDelegate::sizeHint(p_option, p_index);
  return QSize(oldSizeHint.width(), 1.5*oldSizeHint.height());
}
