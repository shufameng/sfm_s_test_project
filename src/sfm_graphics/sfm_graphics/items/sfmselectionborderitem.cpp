#include "sfmselectionborderitem.h"

SFMSelectionBorderItem::SFMSelectionBorderItem(QGraphicsItem *parent) :
    BaseItem(parent)
{
    m_pen.setCosmetic(true);
    m_pen.setColor(QColor(255, 100, 100));
    m_pen.setJoinStyle(Qt::MiterJoin);
}

void SFMSelectionBorderItem::setRect(const QRectF &r)
{
    prepareGeometryChange();
    m_rect = r;
    update();
}

QRectF SFMSelectionBorderItem::boundingRect() const
{
    return m_rect.adjusted(-10, -10, 10, 10);
}

void SFMSelectionBorderItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setPen(m_pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_rect);

    painter->restore();
}
