#include "sfmsquareitem.h"
#include <QGraphicsSceneHoverEvent>

SFMSquareItem::SFMSquareItem(QGraphicsItem *parent) :
    SFMBaseShapeItem(parent)
{
    initItem();
}

SFMSquareItem::SFMSquareItem(const QRectF &r, QGraphicsItem *parent) :\
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
    m_isLButtonOnPress = false;
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
    if (acceptHoverEvents()) {
        if (m_dragRectTopLeft.contains(e->pos()) || m_dragRectBottomRight.contains(e->pos())) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (m_dragRectTopRight.contains(e->pos()) || m_dragRectBottomLeft.contains(e->pos())) {
            setCursor(Qt::SizeBDiagCursor);
        } else {
            setCursor(Qt::SizeAllCursor);
        }
    }

    SFMBaseShapeItem::hoverMoveEvent(e);
}

void SFMSquareItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    SFMBaseShapeItem::hoverLeaveEvent(e);
}

void SFMSquareItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_isLButtonOnPress = true;

        if (m_dragRectTopLeft.contains(e->pos())) {
            m_mousePressLoc = MP_TopLeft;
        } else if (m_dragRectTopRight.contains(e->pos())) {
            m_mousePressLoc = MP_TopRight;
        } else if (m_dragRectBottomLeft.contains(e->pos())) {
            m_mousePressLoc = MP_BottomLeft;
        } else if (m_dragRectBottomRight.contains(e->pos())) {
            m_mousePressLoc = MP_BottomRight;
        } else {
            m_mousePressLoc = MP_Inside;
        }
    }

    SFMBaseShapeItem::mousePressEvent(e);
}

void SFMSquareItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (!m_isLButtonOnPress)
        return SFMBaseShapeItem::mouseMoveEvent(e);

    QRectF r = m_rect;
    int minWidth = 20;

    QPointF pt = e->scenePos() - e->lastScenePos();
    int absX = qAbs(pt.x());
    int absY = qAbs(pt.y());

    switch (m_mousePressLoc) {
    case MP_TopLeft:
    {
        qreal lenA = QLineF(e->scenePos(), m_rect.bottomRight()).length();
        qreal lenB = QLineF(e->lastScenePos(), m_rect.bottomRight()).length();
        if (lenA > lenB) {
            r.setLeft(r.left() - qMax(absX, absY));
            r.setTop(r.top() - qMax(absX, absY));
        } else if (lenA < lenB) {
            r.setLeft(r.left() + qMax(absX, absY));
            r.setTop(r.top() + qMax(absX, absY));
        }
        if (r.width() < minWidth || r.height() < minWidth) {
            r.setLeft(r.right() - minWidth);
            r.setTop(r.bottom() - minWidth);
        }
        setRect(r);
        return;
    }
    case MP_TopRight:
    {
        qreal lenA = QLineF(e->pos(), m_rect.bottomLeft()).length();
        qreal lenB = QLineF(e->lastPos(), m_rect.bottomLeft()).length();
        if (lenA > lenB) {
            r.setRight(r.right() + qMax(absX, absY));
            r.setTop(r.top() - qMax(absX, absY));
        } else if (lenA < lenB) {
            r.setRight(r.right() - qMax(absX, absY));
            r.setTop(r.top() + qMax(absX, absY));
        }
        if (r.width() < minWidth || r.height() < minWidth) {
            r.setRight(r.left() + minWidth);
            r.setTop(r.bottom() - minWidth);
        }
        setRect(r);
        return;
    }
    case MP_BottomLeft:
    {
        qreal lenA = QLineF(e->pos(), m_rect.topRight()).length();
        qreal lenB = QLineF(e->lastPos(), m_rect.topRight()).length();
        if (lenA > lenB) {
            r.setBottom(r.bottom() + qMax(absX, absY));
            r.setLeft(r.left() - qMax(absX, absY));
        } else if (lenA < lenB) {
            r.setBottom(r.bottom() - qMax(absX, absY));
            r.setLeft(r.left() + qMax(absX, absY));
        }
        if (r.width() < minWidth || r.height() < minWidth) {
            r.setBottom(r.top() + minWidth);
            r.setLeft(r.right() - minWidth);
        }
        setRect(r);
        return;
    }
    case MP_BottomRight:
    {
        qreal lenA = QLineF(e->pos(), m_rect.topLeft()).length();
        qreal lenB = QLineF(e->lastPos(), m_rect.topLeft()).length();
        if (lenA > lenB) {
            r.setBottom(r.bottom() + qMax(absX, absY));
            r.setRight(r.right() + qMax(absX, absY));
        } else if (lenA < lenB) {
            r.setBottom(r.bottom() - qMax(absX, absY));
            r.setRight(r.right() - qMax(absX, absY));
        }
        if (r.width() < minWidth || r.height() < minWidth) {
            r.setBottom(r.top() + minWidth);
            r.setRight(r.left() + minWidth);
        }
        setRect(r);
        return;
    }
    default:
        break;
    }

    SFMBaseShapeItem::mouseMoveEvent(e);
}

void SFMSquareItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_isLButtonOnPress = false;
    }
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
