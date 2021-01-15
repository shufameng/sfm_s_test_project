#ifndef SFMPIXMAPITEM_H
#define SFMPIXMAPITEM_H

#include "sfmrectitem.h"

class SFMPixmapItem : public SFMRectItem
{
public:
    explicit SFMPixmapItem(QGraphicsItem *parent = Q_NULLPTR);

    void setPixmap(const QPixmap &p);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QPixmap m_pixmap;
};

#endif // SFMPIXMAPITEM_H
