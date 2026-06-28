#ifndef WATCHLISTDELEGATE_H
#define WATCHLISTDELEGATE_H

#include <QStyledItemDelegate>

class WatchListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit WatchListDelegate(QObject *parent = nullptr);

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};

#endif // WATCHLISTDELEGATE_H
