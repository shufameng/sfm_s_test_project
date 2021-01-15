#include "sfmrectitem.h"
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMoveEvent>
#include <QMenu>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>

SFMRectItem::SFMRectItem(QGraphicsItem *parent) :
    SFMBaseShapeItem(parent)
{
    initItem();
}

SFMRectItem::SFMRectItem(const QRectF &rct, QGraphicsItem *parent) :
    SFMBaseShapeItem(parent)
{
    setRect(rct);
    initItem();
}

void SFMRectItem::setRect(const QRectF &r)
{
    prepareGeometryChange();
    m_rect = r;
    calculateDragRectsGeometry();
    update();
}

void SFMRectItem::setPen(const QPen &p)
{
    SFMBaseShapeItem::setPen(p);
    calculateDragRectsGeometry();
}

QRectF SFMRectItem::boundingRect() const
{
    qreal halfpw = m_pen.style() == Qt::NoPen ? 0.0 : m_pen.widthF() / 2;
    halfpw += m_dragRectWidth / 2;
    halfpw += 1;
    return m_rect.adjusted(- halfpw, - halfpw, halfpw, halfpw);
}

void SFMRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    m_pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->drawRect(m_rect);

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

void SFMRectItem::calculateDragRectsGeometry()
{
    qreal halfpw = m_pen.style() == Qt::NoPen ? 0.0 : m_pen.widthF() / 2;
    QRectF r = m_rect.adjusted(- halfpw, - halfpw, halfpw, halfpw);
    m_selectionRect = r;
    m_dragRectLeft.moveCenter(QPointF(r.left(), r.top() + r.height() / 2));
    m_dragRectTop.moveCenter(QPointF(r.left() + r.width() / 2, r.top()));
    m_dragRectRight.moveCenter(QPointF(r.right(), r.top() + r.height() / 2));
    m_dragRectBottom.moveCenter(QPointF(r.left() + r.width() / 2, r.bottom()));
    m_dragRectTopLeft.moveCenter(r.topLeft());
    m_dragRectTopRight.moveCenter(r.topRight());
    m_dragRectBottomLeft.moveCenter(r.bottomLeft());
    m_dragRectBottomRight.moveCenter(r.bottomRight());
}

void SFMRectItem::initItem()
{
    m_dragRectWidth = 10;
    m_mousePressLoc = MP_OutSide;
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);

    m_dragRectLeft =
            m_dragRectTop =
            m_dragRectRight =
            m_dragRectBottom =
            m_dragRectTopLeft =
            m_dragRectTopRight =
            m_dragRectBottomLeft =
            m_dragRectBottomRight =
            QRectF(0, 0, m_dragRectWidth, m_dragRectWidth);
    calculateDragRectsGeometry();
}

void SFMRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    SFMBaseShapeItem::hoverEnterEvent(e);
}

void SFMRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
    if (acceptHoverEvents()) {
        if (m_dragRectTop.contains(e->pos()) || m_dragRectBottom.contains(e->pos())) {
            setCursor(Qt::SizeVerCursor);
        } else if (m_dragRectLeft.contains(e->pos()) || m_dragRectRight.contains(e->pos())) {
            setCursor(Qt::SizeHorCursor);
        } else if (m_dragRectTopLeft.contains(e->pos()) || m_dragRectBottomRight.contains(e->pos())) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (m_dragRectTopRight.contains(e->pos()) || m_dragRectBottomLeft.contains(e->pos())) {
            setCursor(Qt::SizeBDiagCursor);
        } else {
            setCursor(Qt::SizeAllCursor);
        }
    }

    SFMBaseShapeItem::hoverMoveEvent(e);
}

void SFMRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    SFMBaseShapeItem::hoverLeaveEvent(e);
}

void SFMRectItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (m_dragRectLeft.contains(e->pos())) {
        m_mousePressLoc = MP_Left;
    } else if (m_dragRectTop.contains(e->pos())) {
        m_mousePressLoc = MP_Top;
    } else if (m_dragRectRight.contains(e->pos())) {
        m_mousePressLoc = MP_Right;
    } else if (m_dragRectBottom.contains(e->pos())) {
        m_mousePressLoc = MP_Bottom;
    } else if (m_dragRectTopLeft.contains(e->pos())) {
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
    SFMBaseShapeItem::mousePressEvent(e);
}

void SFMRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QRectF r = m_rect;
    int minWidth = 20;

    switch (m_mousePressLoc) {
    case MP_Left:
        if (e->pos().x() < r.right() - minWidth) {
            r.setLeft(e->pos().x());
        } else {
            r.setLeft(r.right() - minWidth);
        }
        setRect(r);
        return;
    case MP_Top:
        if (e->pos().y() < r.bottom() - minWidth) {
            r.setTop(e->pos().y());
        } else {
            r.setTop(r.bottom() - minWidth);
        }
        setRect(r);
        return;
    case MP_Right:
        if (e->pos().x() > r.left() + minWidth) {
            r.setRight(e->pos().x());
        } else {
            r.setRight(r.left() + minWidth);
        }
        setRect(r);
        return;
    case MP_Bottom:
        if (e->pos().y() > r.top() + minWidth) {
            r.setBottom(e->pos().y());
        } else {
            r.setBottom(r.top() + minWidth);
        }
        setRect(r);
        return;
    case MP_TopLeft:
        r.setTop(e->pos().y() < r.bottom() - minWidth ? e->pos().y() : r.bottom() - minWidth);
        r.setLeft(e->pos().x() < r.right() - minWidth ? e->pos().x() : r.right() - minWidth);
        setRect(r);
        return;
    case MP_TopRight:
        r.setTop(e->pos().y() < r.bottom() - minWidth ? e->pos().y() : r.bottom() - minWidth);
        r.setRight(e->pos().x() > r.left() + minWidth ? e->pos().x() : r.left() + minWidth);
        setRect(r);
        return;
    case MP_BottomLeft:
        r.setBottom(e->pos().y() > r.top() + minWidth ? e->pos().y() : r.top() + minWidth);
        r.setLeft(e->pos().x() < r.right() - minWidth ? e->pos().x() : r.right() - minWidth);
        setRect(r);
        return;
    case MP_BottomRight:
        r.setBottom(e->pos().y() > r.top() + minWidth ? e->pos().y() : r.top() + minWidth);
        r.setRight(e->pos().x() > r.left() + minWidth ? e->pos().x() : r.left() + minWidth);
        setRect(r);
        return;
    default:
        break;
    }
    SFMBaseShapeItem::mouseMoveEvent(e);
}

void SFMRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    SFMBaseShapeItem::mouseReleaseEvent(e);
}

void SFMRectItem::focusInEvent(QFocusEvent *e)
{
    SFMBaseShapeItem::focusInEvent(e);
}

void SFMRectItem::focusOutEvent(QFocusEvent *e)
{
    SFMBaseShapeItem::focusOutEvent(e);
}

QVariant SFMRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemSelectedChange:
    {
        if (value.toBool()) {

        }
    }
        break;
    default:
        break;
    }

    return SFMBaseShapeItem::itemChange(change, value);
}
