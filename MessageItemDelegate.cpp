#include "MessageItemDelegate.h"


MessageItemDelegate::MessageItemDelegate(QObject* parent):
  QStyledItemDelegate(parent) {
}

void MessageItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
  QStyledItemDelegate::paint(painter, option, index);

  painter->save();

  QFont font("Monospace");
  painter->setFont(font);

  QString informationString = qvariant_cast<QString>(index.data(InformationRole));
  QString successString     = qvariant_cast<QString>(index.data(SuccessRole    ));
  QString warningString     = qvariant_cast<QString>(index.data(WarningRole    ));
  QString errorString       = qvariant_cast<QString>(index.data(ErrorRole      ));
  QString downloadString    = qvariant_cast<QString>(index.data(DownloadRole   ));

  // Paint information
  painter->setPen(QPen(QColor("#4169e1")));
  painter->drawText(option.rect, informationString);

  // Paint success
  painter->setPen(QPen(QColor("#228b22")));
  painter->drawText(option.rect, successString);

  // Paint warning
  painter->setPen(QPen(QColor("#ff8c00")));
  painter->drawText(option.rect, warningString);

  // Paint error
  painter->setPen(QPen(QColor("#ff0000")));
  painter->drawText(option.rect, errorString);

  // Paint download
  painter->setPen(QPen(QColor("#333333")));
  painter->drawText(option.rect, Qt::AlignRight, downloadString);

  painter->restore();
}
