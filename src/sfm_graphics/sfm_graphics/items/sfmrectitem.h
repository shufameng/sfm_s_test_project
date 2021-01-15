#ifndef SFMRECTITEM_H
#define SFMRECTITEM_H

#include "sfmbaseshapeitem.h"

class SFMRectItem : public SFMBaseShapeItem
{
public:
    enum MousePressLocation {
        MP_Inside = 0,
        MP_OutSide,
        MP_Left,
        MP_Top,
        MP_Right,
        MP_Bottom,
        MP_TopLeft,
        MP_TopRight,
        MP_BottomLeft,
        MP_BottomRight
    };

    explicit SFMRectItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit SFMRectItem(const QRectF &rct, QGraphicsItem *parent = Q_NULLPTR);

    void setRect(const QRectF &r);

    QRectF rect() const {
        return m_rect;
    }

    void setPen(const QPen &p);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QRectF m_rect;
    QRectF m_selectionRect;

    QRectF
    m_dragRectLeft,
    m_dragRectTop,
    m_dragRectRight,
    m_dragRectBottom,
    m_dragRectTopLeft,
    m_dragRectTopRight,
    m_dragRectBottomRight,
    m_dragRectBottomLeft;
    qreal m_dragRectWidth;

    MousePressLocation m_mousePressLoc;

    void calculateDragRectsGeometry();
    void initItem();

    void hoverEnterEvent(QGraphicsSceneHoverEvent *e) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *e) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
    void focusOutEvent(QFocusEvent *e) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;


};

#endif // SFMRECTITEM_H
