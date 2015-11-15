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

  QRect informationRect = option.rect;
  QRect successRect     = option.rect;
  QRect warningRect     = option.rect;
  QRect errorRect       = option.rect;
  QRect downloadRect    = option.rect;

  // Paint information
  painter->setPen(QPen(QColor("#4169e1")));
  painter->drawText(informationRect, informationString);

  // Paint success
  painter->setPen(QPen(QColor("#228b22")));
  painter->drawText(successRect, successString);

  // Paint warning
  painter->setPen(QPen(QColor("#ff8c00")));
  painter->drawText(warningRect, warningString);

  // Paint error
  painter->setPen(QPen(QColor("#ff0000")));
  painter->drawText(errorRect, errorString);

  // Paint download
  painter->setPen(QPen(QColor("#333333")));
  painter->drawText(downloadRect, Qt::AlignRight, downloadString);

  painter->restore();
}
