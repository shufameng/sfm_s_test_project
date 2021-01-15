#include "sfmsquareitem.h"

SFMSquareItem::SFMSquareItem(QGraphicsItem *parent) :
    SFMBaseShapeItem(parent)
{
    initItem();
}

SFMSquareItem::SFMSquareItem(const QRect &r, QGraphicsItem *parent) :\
    SFMBaseShapeItem(parent)
{
    setRect(r);
    initItem();
}

void SFMSquareItem::setRect(const QRectF &r)
{
    prepareGeometryChange();
    m_rect = r;
    calculateRects();
    update();
}

void SFMSquareItem::setPen(const QPen &p)
{
    SFMBaseShapeItem::setPen(p);
    calculateRects();
}

QRectF SFMSquareItem::boundingRect() const
{
    qreal halfpw = m_pen.style() == Qt::NoPen ? 0.0 : m_pen.widthF() / 2;
    halfpw += m_dragRectWidth / 2;
    halfpw += 1;
    return m_rect.adjusted(- halfpw, - halfpw, halfpw, halfpw);
}

void SFMSquareItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    m_pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->drawRect(m_rect);

    if (isSelected()) {
        drawSelectionRect(m_selectionRect, painter, option, widget);
        drawDragResizeRect(m_dragRectTopLeft, painter, option, widget);
        drawDragResizeRect(m_dragRectTopRight, painter, option, widget);
        drawDragResizeRect(m_dragRectBottomRight, painter, option, widget);
        drawDragResizeRect(m_dragRectBottomLeft, painter, option, widget);
    }

    painter->restore();
}

void SFMSquareItem::calculateRects()
{
    qreal halfpw = m_pen.style() == Qt::NoPen ? 0.0 : m_pen.widthF() / 2;
    QRectF r = m_rect.adjusted(- halfpw, - halfpw, halfpw, halfpw);
    m_selectionRect = r;
    m_dragRectTopLeft.moveCenter(r.topLeft());
    m_dragRectTopRight.moveCenter(r.topRight());
    m_dragRectBottomLeft.moveCenter(r.bottomLeft());
    m_dragRectBottomRight.moveCenter(r.bottomRight());
}

void SFMSquareItem::initItem()
{
    m_dragRectWidth = 10;
    m_mousePressLoc = MP_Outside;
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);

    m_dragRectTopLeft =
            m_dragRectTopRight =
            m_dragRectBottomLeft =
            m_dragRectBottomRight =
            QRectF(0, 0, m_dragRectWidth, m_dragRectWidth);
    calculateRects();
}

void SFMSquareItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    SFMBaseShapeItem::hoverEnterEvent(e);
}

void SFMSquareItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
    SFMBaseShapeItem::hoverMoveEvent(e);
}

void SFMSquareItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    SFMBaseShapeItem::hoverLeaveEvent(e);
}

void SFMSquareItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    SFMBaseShapeItem::mousePressEvent(e);
}

void SFMSquareItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    SFMBaseShapeItem::mouseMoveEvent(e);
}

void SFMSquareItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    SFMBaseShapeItem::mouseReleaseEvent(e);
}

void SFMSquareItem::focusInEvent(QFocusEvent *e)
{
    SFMBaseShapeItem::focusInEvent(e);
}

void SFMSquareItem::focusOutEvent(QFocusEvent *e)
{
    SFMBaseShapeItem::focusOutEvent(e);
}

QVariant SFMSquareItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return SFMBaseShapeItem::itemChange(change, value);
}
