#include "sfmrounditem.h"

SFMRoundItem::SFMRoundItem(QGraphicsItem *parent) :
    SFMSquareItem(parent)
{

}

SFMRoundItem::SFMRoundItem(const QRectF &r, QGraphicsItem *parent) :
    SFMSquareItem(r, parent)
{

}

void SFMRoundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    m_pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->drawEllipse(m_rect);

    if (isSelected()) {
        drawSelectionRect(m_selectionRect, painter, option, widget);
        drawDragResizeRect(m_dragRectTopLeft, painter, option, widget);
        drawDragResizeRect(m_dragRectTopRight, painter, option, widget);
        drawDragResizeRect(m_dragRectBottomRight, painter, option, widget);
        drawDragResizeRect(m_dragRectBottomLeft, painter, option, widget);
    }

    painter->restore();
}
