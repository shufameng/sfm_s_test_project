#include "sfmpixmapitem.h"

SFMPixmapItem::SFMPixmapItem(QGraphicsItem *parent) :
    SFMRectItem(parent)
{
    m_pen.setStyle(Qt::NoPen);
}

void SFMPixmapItem::setPixmap(const QPixmap &p)
{
    prepareGeometryChange();
    setRect(QRectF(0, 0, p.width(), p.height()));
    m_pixmap = p;
    update();
}

void SFMPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->drawPixmap(m_rect, m_pixmap, m_pixmap.rect());

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
