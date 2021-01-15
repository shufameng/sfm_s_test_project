#ifndef BASEITEM_H
#define BASEITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class BaseItem : public QGraphicsItem
{
public:
    explicit BaseItem(QGraphicsItem *parent = Q_NULLPTR);
    virtual ~BaseItem();

    static void drawSelectionRect(const QRectF &r, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    static void drawDragResizeRect(const QRectF &r,  QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // BASEITEM_H
