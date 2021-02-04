#ifndef SFMSELECTIONBORDERITEM_H
#define SFMSELECTIONBORDERITEM_H

#include "baseitem.h"

class SFMSelectionBorderItem : public BaseItem
{
public:
    enum DragPos {
        DP_Left = 0,
        DP_Top,
        DP_Right,
        DP_Bottom,
        DP_TopLeft,
        DP_TopRight,
        DP_BottomLeft,
        DP_BottomRight
    };

    explicit SFMSelectionBorderItem(QGraphicsItem *parent = Q_NULLPTR);

    void setRect(const QRectF &r);

    QRectF rect() const {
        return m_rect;
    }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPen m_pen;
    QRectF m_rect;

};

#endif // SFMSELECTIONBORDERITEM_H
