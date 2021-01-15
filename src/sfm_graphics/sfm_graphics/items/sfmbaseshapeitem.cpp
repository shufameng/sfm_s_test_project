#include "sfmbaseshapeitem.h"

SFMBaseShapeItem::SFMBaseShapeItem(QGraphicsItem *parent) :
    BaseItem(parent)
{

}

void SFMBaseShapeItem::setPen(const QPen &p)
{
    m_pen = p;
    update();
}

void SFMBaseShapeItem::setBrush(const QBrush &b)
{
    m_brush = b;
    update();
}

