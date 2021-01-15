#ifndef SFMBASESHAPEITEM_H
#define SFMBASESHAPEITEM_H

#include "baseitem.h"

class SFMBaseShapeItem : public BaseItem
{
public:
    explicit SFMBaseShapeItem(QGraphicsItem *parent = Q_NULLPTR);

    void setPen(const QPen &p);

    QPen pen() const {
        return m_pen;
    }

    void setBrush(const QBrush &b);

    QBrush brush() const {
        return  m_brush;
    }

    //QRectF boundingRect() const override;

    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QPen m_pen;
    QBrush m_brush;
};

#endif // SFMBASESHAPEITEM_H
