#include "sfmellipseitem.h"

SFMEllipseItem::SFMEllipseItem(QGraphicsItem *parent) :
    SFMRectItem(parent)
{

}

SFMEllipseItem::SFMEllipseItem(const QRectF &r, QGraphicsItem *parent) :
    SFMRectItem(r, parent)
{

}

void SFMEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->drawEllipse(m_rect);

    if (isSelected()) {
        drawSelectionRect(m_selectionRect, painter, option, widget);
        drawDragResizeRect(m_dragRectLeft, painter, option, widget);
        drawDragResizeRect(m_dragRectTop, painter, option, widget);
        drawDragResizeRect(m_dragRectRight, painter, option, widget);
        drawDragResizeRect(m_dragRectBottom, painter, option, widget);
        drawDragResizeRect(m_dragRectTopLeft, painter, option, widget);
        drawDragResizeRect(m_dragRectTopRight, painter, option, widget);
        drawDragResizeRect(m_dragRectBottomRight, painter, option, widget);
        drawDragResizeRect(m_dragRectBottomLeft, painter, option, widget);
    }

    painter->restore();
}
