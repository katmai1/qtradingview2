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

    QString linea1 = index.data(Qt::DisplayRole).toString();       // ticker
    QString linea2 = index.data(Qt::UserRole + 2).toString();      // nombre u otro dato

    QRect rect = option.rect.adjusted(8, 4, -8, -4);

    // Línea 1 — texto principal (negrita)
    QFont fontBold = painter->font();
    fontBold.setBold(true);
    painter->setFont(fontBold);
    painter->setPen(option.state & QStyle::State_Selected
                        ? option.palette.highlightedText().color()
                        : option.palette.text().color());
    painter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, linea1);

    // Línea 2 — texto secundario (más pequeño)
    QFont fontSmall = painter->font();
    fontSmall.setBold(false);
    fontSmall.setPointSize(fontSmall.pointSize() - 2);
    painter->setFont(fontSmall);
    painter->setPen(Qt::gray);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, linea2);

    painter->restore();
}

QSize WatchListDelegate::sizeHint(const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const
{
    return QSize(option.rect.width(), 40); // altura del item
}