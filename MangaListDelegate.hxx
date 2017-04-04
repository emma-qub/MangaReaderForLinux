#ifndef MANGALISTDELEGATE_HXX
#define MANGALISTDELEGATE_HXX

#include <QStyledItemDelegate>
#include <QPainter>

class MangaListDelegate: public QStyledItemDelegate {

public:
  enum MangaDataRole {
    eChaptersToReadRole = Qt::UserRole,
    eChaptersCountRole,
    eAvailableChaptersRole,
    eColorRole
  };

  MangaListDelegate(QObject* p_parent = nullptr);

  void paint(QPainter* p_painter, const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const override;

protected:
  QSize sizeHint(const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const override;
};


#endif // MANGALISTDELEGATE_HXX
