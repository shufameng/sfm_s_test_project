#ifndef SFMGRAPHICSSCENE_H
#define SFMGRAPHICSSCENE_H

#include <QGraphicsScene>

class SFMRectItem;
class SFMEllipseItem;
class SFMPixmapItem;
class SFMSquareItem;
class SFMRoundItem;

class SFMGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Tool {
        Tool_Eraser = 0,
        Tool_Pen
    };

    explicit SFMGraphicsScene(QObject *parent = Q_NULLPTR);
    virtual ~SFMGraphicsScene();

    SFMRectItem *addResizeableRect(const QRectF &r);
    SFMEllipseItem *addResizeableEllipse(const QRectF &r);
    SFMPixmapItem *addResizeablePixmap(const QPixmap &p);
    SFMSquareItem *addSquareItem(const QRectF &r);
    SFMRoundItem *addRoundItem(const QRectF &r);

};

#endif // SFMGRAPHICSSCENE_H
