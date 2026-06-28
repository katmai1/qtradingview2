#include "watchlistdelegate.h"
#include <QPainter>

WatchListDelegate::WatchListDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}


void WatchListDelegate::paint(QPainter* painter,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
    painter->save();

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    QString name = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 2).toString();
    QString tag  = index.data(Qt::UserRole + 3).toString();
    if (tag.isEmpty()) {    tag = "#SinEtiqueta";  }

    QRect rect = option.rect.adjusted(8, 4, -8, -4);

    // name y tag
    QFont fontBold = painter->font();
    fontBold.setBold(true);
    painter->setFont(fontBold);
    painter->setPen(option.state & QStyle::State_Selected
                        ? option.palette.highlightedText().color()
                        : option.palette.text().color());
    painter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, name);
    painter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tag);

    // descripcion
    QFont fontSmall = painter->font();
    fontSmall.setBold(false);
    fontSmall.setPointSize(fontSmall.pointSize() - 1);
    painter->setFont(fontSmall);
    painter->setPen(Qt::gray);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, description);

    painter->restore();
}

QSize WatchListDelegate::sizeHint(const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const
{
    return QSize(option.rect.width(), 40); // altura del item
}