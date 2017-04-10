#ifndef MESSAGEITEMDELEGATE_H
#define MESSAGEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class MessageItemDelegate: public QStyledItemDelegate {
  Q_OBJECT

public:
  enum dataRole {
    InformationRole = Qt::UserRole+100,
    SuccessRole = Qt::UserRole+101,
    WarningRole = Qt::UserRole+102,
    ErrorRole = Qt::UserRole+103,
    DownloadRole = Qt::UserRole+104
  };

  explicit MessageItemDelegate(QObject* parent = nullptr);

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

};

#endif // MESSAGEITEMDELEGATE_H
