#include "sfmgraphicsscene.h"
#include "sfmrectitem.h"
#include "sfmellipseitem.h"
#include "sfmpixmapitem.h"
#include "sfmsquareitem.h"
#include "sfmrounditem.h"

SFMGraphicsScene::SFMGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

SFMGraphicsScene::~SFMGraphicsScene()
{

}

SFMRectItem *SFMGraphicsScene::addResizeableRect(const QRectF &r)
{
    SFMRectItem *item = new SFMRectItem(r);
    addItem(item);
    return item;
}

SFMEllipseItem *SFMGraphicsScene::addResizeableEllipse(const QRectF &r)
{
    SFMEllipseItem *item = new SFMEllipseItem(r);
    addItem(item);
    return item;
}

SFMPixmapItem *SFMGraphicsScene::addResizeablePixmap(const QPixmap &p)
{
    SFMPixmapItem *item = new SFMPixmapItem;
    item->setPixmap(p);
    addItem(item);
    return item;
}

SFMSquareItem *SFMGraphicsScene::addSquareItem(const QRectF &r)
{
    SFMSquareItem *item = new SFMSquareItem(r);
    addItem(item);
    return item;
}

SFMRoundItem *SFMGraphicsScene::addRoundItem(const QRectF &r)
{
    SFMRoundItem *item = new SFMRoundItem(r);
    addItem(item);
    return item;
}
