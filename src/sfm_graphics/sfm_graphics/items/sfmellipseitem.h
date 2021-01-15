#ifndef SFMELLIPSEITEM_H
#define SFMELLIPSEITEM_H

#include "sfmrectitem.h"

class SFMEllipseItem : public SFMRectItem
{
public:
    explicit SFMEllipseItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit SFMEllipseItem(const QRectF &r, QGraphicsItem *parent = Q_NULLPTR);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // SFMELLIPSEITEM_H
