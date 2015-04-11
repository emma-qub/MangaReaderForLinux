#include "MessageListView.h"
#include <QStringListModel>
#include <QPainter>

MessageListView::MessageListView(QWidget* parent) :
    QListView(parent) {

    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::NoSelection);
}
