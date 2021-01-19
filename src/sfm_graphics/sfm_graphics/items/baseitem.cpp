#include "baseitem.h"
#include <QMenu>
#include <QGraphicsScene>

BaseItem::BaseItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{

}

BaseItem::~BaseItem()
{

}

void BaseItem::drawSelectionRect(const QRectF &r, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen p(Qt::DashLine);
    p.setColor(QColor(0, 0, 200));

    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(255, 255, 255));

    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(r);

    painter->restore();
}

void BaseItem::drawDragResizeRect(const QRectF &r, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen p(Qt::SolidLine);
    p.setColor(QColor(0, 0, 200));
    p.setCosmetic(true);

    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(255, 255, 255));

    painter->setPen(p);
    painter->setBrush(b);
    painter->drawEllipse(r);

    painter->restore();
}

void BaseItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *e)
{
    QGraphicsItem::contextMenuEvent(e);

    QMenu m;
    m.addAction(QIcon(), "Stack to top", [this]() {
        if (scene()) {
            QList<QGraphicsItem*> list = scene()->items(this->boundingRect(), Qt::IntersectsItemBoundingRect);
            foreach (QGraphicsItem *item, list) {
                this->stackBefore(item);
            }
        }
    });
    m.addAction(QIcon(), "Delete", [this]() {
        if (scene()) {
            scene()->removeItem(this);
        }
    });
    m.exec(QCursor::pos());
}


