#include "sfmgraphicsview.h"
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QDebug>
#include "sfmgraphicsscene.h"
#include "sfmrectitem.h"
#include "sfmellipseitem.h"
#include "sfmpixmapitem.h"
#include <QFileDialog>
#include <QMessageBox>

SFMGraphicsView::SFMGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    m_isLButtonOnPress(false),
    m_currentTool(Tool_ResizeableRect)
{
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setSceneRect(QRectF(0, 0, 1920, 1080));
    setRenderHints(QPainter::Antialiasing);
}

SFMGraphicsView::~SFMGraphicsView()
{

}

void SFMGraphicsView::setCurrentTool(SFMGraphicsView::Tool t)
{
    m_currentTool = t;
    if (m_currentTool == Tool_Select) {
        QList<QGraphicsItem*> list = items();
        foreach (QGraphicsItem *item, list) {
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setAcceptHoverEvents(true);
        }
        viewport()->setCursor(Qt::PointingHandCursor);
    } else {
        QList<QGraphicsItem*> list = items();
        foreach (QGraphicsItem *item, list) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
            item->setAcceptHoverEvents(false);
        }
        viewport()->setCursor(Qt::CrossCursor);
    }
}

void SFMGraphicsView::mousePressEvent(QMouseEvent *e)
{
    QPointF sp = mapToScene(e->pos());
    QPen p(Qt::SolidLine);
    p.setColor(QColor(100, 100, 255));
    p.setWidthF(6);
    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(100, 100, 50));

    QRectF r(sp - QPointF(100, 100), QSizeF(200, 200));

    SFMGraphicsScene *s = qobject_cast<SFMGraphicsScene*>(scene());
    if (s) {
        switch (m_currentTool) {
        case Tool_ResizeableRect:
        {
            SFMRectItem *item = s->addResizeableRect(r);
            item->setPen(p);
            item->setBrush(b);
        }
            break;
        case Tool_Square:
            break;
        case Tool_ResizeableEllipse:
        {
            SFMEllipseItem *item = s->addResizeableEllipse(r);
            item->setPen(p);
            item->setBrush(b);
        }
            break;
        case Tool_Round:
            break;
        case Tool_Pixmap:
        {
            QSizeF sz = m_pixmapForItem.size();
            SFMPixmapItem *item = s->addResizeablePixmap(m_pixmapForItem);
            item->setRect(QRectF(sp - QPointF(sz.width() / 2, sz.height() / 2), sz));

        }
            break;
        default:
            break;
        }
    }

    QGraphicsView::mousePressEvent(e);
}

void SFMGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QGraphicsView::mouseMoveEvent(e);
}

void SFMGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    QGraphicsView::mouseReleaseEvent(e);
}
