#include "baseitem.h"

BaseItem::BaseItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{

}

BaseItem::~BaseItem()
{

}

void BaseItem::drawSelectionRect(const QRectF &r, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen p(Qt::DashLine);
    p.setColor(QColor(0, 0, 200));

    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(255, 255, 255));

    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(r);

    painter->restore();
}

void BaseItem::drawDragResizeRect(const QRectF &r, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen p(Qt::SolidLine);
    p.setColor(QColor(0, 0, 200));
    p.setCosmetic(true);

    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(255, 255, 255));

    painter->setPen(p);
    painter->setBrush(b);
    painter->drawEllipse(r);

    painter->restore();
}

