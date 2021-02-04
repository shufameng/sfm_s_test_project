#ifndef SFMSQUAREITEM_H
#define SFMSQUAREITEM_H

#include "sfmbaseshapeitem.h"

class SFMSquareItem : public SFMBaseShapeItem
{
public:
    enum MousePressLocation {
        MP_TopLeft = 0,
        MP_TopRight,
        MP_BottomLeft,
        MP_BottomRight,
        MP_Inside,
        MP_Outside
    };

    explicit SFMSquareItem(QGraphicsItem *parent = Q_NULLPTR);

    explicit SFMSquareItem(const QRectF &r, QGraphicsItem *parent = Q_NULLPTR);

    void setRect(const QRectF &r);

    QRectF rect() const {
        return m_rect;
    }

    void setPen(const QPen &p);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    MousePressLocation m_mousePressLoc;
    QRectF m_rect;
    QRectF m_selectionRect;

    qreal m_dragRectWidth;

    QRectF
    m_dragRectTopLeft,
    m_dragRectTopRight,
    m_dragRectBottomLeft,
    m_dragRectBottomRight;

    bool m_isLButtonOnPress;
    QPointF m_lButtonPressPos;

    void calculateRects();
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

#endif // SFMSQUAREITEM_H
