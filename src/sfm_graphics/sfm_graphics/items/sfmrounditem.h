#ifndef SFMROUNDITEM_H
#define SFMROUNDITEM_H

#include "sfmsquareitem.h"

class SFMRoundItem : public SFMSquareItem
{
public:
    explicit SFMRoundItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit SFMRoundItem(const QRectF &r, QGraphicsItem *parent = Q_NULLPTR);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // SFMROUNDITEM_H
