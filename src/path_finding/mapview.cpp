#include "mapview.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>

//
class MapViewCellOption
{
public:
    MapViewCellOption()
    {}

    QBrush      backgroundBrush;
    QPen        borderPen;
    QString     text;
    QFont       textFont;
    QColor      textColor;
};

//
class MapViewCell
{
public:
    enum CellType {
        CT_Walkable = 0,
        CT_Unwalkable,
        CT_Start,
        CT_Dest,
        CT_Path
    };

    MapViewCell() :
        type(CT_Walkable),
        x(-1),
        y(-1),
        g(0),
        h(0),
        parent(Q_NULLPTR)
    {}

    CellType type;
    int x;
    int y;
    int g;
    int h;
    MapViewCell *parent;
};

//
class MapViewData
{
public:
    MapViewData() :
        hasStartCell(false),
        hasDestCell(false),
        cellArray(Q_NULLPTR),
        unwalkablePercent(0.2)
    {}

    QSize mapSize;
    QSize cellSize;
    QPoint startCellPt;
    QPoint destCellPt;
    bool hasStartCell;
    bool hasDestCell;

    MapViewCellOption
    walkableCellOpt,
    unwalkableCellOpt,
    startCellOpt,
    destCellOpt,
    pathCellOpt;

    MapViewCell *cellArray;
    qreal unwalkablePercent;
};

MapView::MapView(QWidget *parent) :
    QWidget(parent),
    m_viewData(new MapViewData),
    m_interacteMode(IM_None)
{
    m_viewData->walkableCellOpt.backgroundBrush.setStyle(Qt::SolidPattern);
    m_viewData->walkableCellOpt.backgroundBrush.setColor(QColor(1, 180, 247));
    m_viewData->walkableCellOpt.borderPen.setStyle(Qt::SolidLine);
    m_viewData->walkableCellOpt.borderPen.setColor(QColor(0, 0, 0));
    m_viewData->walkableCellOpt.text = "";
    m_viewData->walkableCellOpt.textColor = QColor(255, 255, 255);

    m_viewData->unwalkableCellOpt.backgroundBrush.setStyle(Qt::SolidPattern);
    m_viewData->unwalkableCellOpt.backgroundBrush.setColor(QColor(0, 0, 0));
    m_viewData->unwalkableCellOpt.borderPen.setStyle(Qt::SolidLine);
    m_viewData->unwalkableCellOpt.borderPen.setColor(QColor(0, 0, 0));
    m_viewData->unwalkableCellOpt.text = "";
    m_viewData->unwalkableCellOpt.textColor = QColor(255, 255, 255);

    m_viewData->startCellOpt.backgroundBrush.setStyle(Qt::SolidPattern);
    m_viewData->startCellOpt.backgroundBrush.setColor(QColor(255, 0, 0));
    m_viewData->startCellOpt.borderPen.setStyle(Qt::SolidLine);
    m_viewData->startCellOpt.borderPen.setColor(QColor(0, 0, 0));
    m_viewData->startCellOpt.text = "S";
    m_viewData->startCellOpt.textColor = QColor(255, 255, 255);

    m_viewData->destCellOpt.backgroundBrush.setStyle(Qt::SolidPattern);
    m_viewData->destCellOpt.backgroundBrush.setColor(QColor(255, 0, 0));
    m_viewData->destCellOpt.borderPen.setStyle(Qt::SolidLine);
    m_viewData->destCellOpt.borderPen.setColor(QColor(0, 0, 0));
    m_viewData->destCellOpt.text = "D";
    m_viewData->destCellOpt.textColor = QColor(255, 255, 255);

    m_viewData->pathCellOpt.backgroundBrush.setStyle(Qt::SolidPattern);
    m_viewData->pathCellOpt.backgroundBrush.setColor(QColor(255, 0, 0));
    m_viewData->pathCellOpt.borderPen.setStyle(Qt::SolidLine);
    m_viewData->pathCellOpt.borderPen.setColor(QColor(0, 0, 0));
    m_viewData->pathCellOpt.text = "";
    m_viewData->pathCellOpt.textColor = QColor(255, 255, 255);

    setMapSize(30, 20);
    setCellSize(36, 36);
}

int MapView::getRandomInt(int lowBound, int highBound)
{
    return rand() % (highBound - lowBound + 1) + lowBound;
}

void MapView::setMapSize(int w, int h)
{
    if (m_viewData->cellArray) {
        delete [] m_viewData->cellArray;
    }
    int arraySize = w * h;
    m_viewData->cellArray = new MapViewCell[arraySize];
    m_viewData->mapSize = QSize(w, h);

    fillMapArrayWithWalkable();
    updateMinsize();
    update();
}

QSize MapView::getMapSize() const
{
    return m_viewData->mapSize;
}

void MapView::setCellSize(int w, int h)
{
    m_viewData->cellSize = QSize(w, h);
    updateMinsize();
    update();
}

QSize MapView::getCellSize() const
{
    return m_viewData->cellSize;
}

void MapView::setUnwalkablePercent(qreal f)
{
    m_viewData->unwalkablePercent = f;
}

qreal MapView::getUnwalkablePercent() const
{
    return m_viewData->unwalkablePercent;
}

void MapView::fillMapArrayWithWalkable()
{
    int w = m_viewData->mapSize.width();

    for (int x = 0; x < m_viewData->mapSize.width(); ++ x) {
        for (int y = 0; y < m_viewData->mapSize.height(); ++ y) {
            MapViewCell *cell = &m_viewData->cellArray[w * y + x];
            cell->x = x;
            cell->y = y;
            cell->type = MapViewCell::CT_Walkable;
        }
    }
}

void MapView::updateMinsize()
{
    if (m_viewData->mapSize.isValid() && m_viewData->cellSize.isValid()) {
        setMinimumSize(m_viewData->mapSize.width() * m_viewData->cellSize.width() + 1,
                       m_viewData->mapSize.height() * m_viewData->cellSize.height() + 1);
    }
}

void MapView::generateRandomMap()
{
    // Clean map
    cleanMap();

    // Generate start and dest
    QPoint pt = QPoint(getRandomInt(0, m_viewData->mapSize.width() - 1),
                                     getRandomInt(0, m_viewData->mapSize.height() - 1));
    setStartCellPoint(pt.x(), pt.y());

    while (true) {
        pt = QPoint(getRandomInt(0, m_viewData->mapSize.width() - 1),
                                        getRandomInt(0, m_viewData->mapSize.height() - 1));
        if (m_viewData->cellArray[
                m_viewData->mapSize.width() *
                pt.y() +
                pt.x()].type == MapViewCell::CT_Walkable) {
            //
            setDestCellPoint(pt.x(), pt.y());
            break;
        }
    }

    // Generate walkable and unwalkable
    qreal f = 1.0 / m_viewData->unwalkablePercent;
    for (int x = 0; x < m_viewData->mapSize.width(); ++ x) {
        //
        for (int y = 0; y < m_viewData->mapSize.height(); ++ y) {
            //
            if (MapViewCell::CT_Walkable == m_viewData->cellArray[m_viewData->mapSize.width() * y + x].type) {
                if (f > 1.0) {
                    if (1 == getRandomInt(1, (int)f)) {
                        m_viewData->cellArray[m_viewData->mapSize.width() * y + x].type = MapViewCell::CT_Unwalkable;
                    }
                } else {
                    m_viewData->cellArray[m_viewData->mapSize.width() * y + x].type = MapViewCell::CT_Unwalkable;
                }
            }
        }
    }
}

void MapView::generateEmptyMap()
{
    cleanMap();
}

void MapView::cleanMap()
{
    fillMapArrayWithWalkable();
    m_viewData->hasStartCell = false;
    m_viewData->hasDestCell = false;
}

void MapView::setStartCellPoint(int x, int y)
{
    if (m_viewData->hasStartCell) {
        m_viewData->cellArray[m_viewData->mapSize.width() * m_viewData->startCellPt.y() + m_viewData->startCellPt.x()].type = MapViewCell::CT_Walkable;
    }
    m_viewData->startCellPt = QPoint(x, y);
    m_viewData->hasStartCell = true;
    m_viewData->cellArray[m_viewData->mapSize.width() * y + x].type = MapViewCell::CT_Start;
}

void MapView::setDestCellPoint(int x, int y)
{
    if (m_viewData->hasDestCell) {
        m_viewData->cellArray[m_viewData->mapSize.width() * m_viewData->destCellPt.y() + m_viewData->destCellPt.x()].type = MapViewCell::CT_Walkable;
    }
    m_viewData->destCellPt = QPoint(x, y);
    m_viewData->hasDestCell = true;
    m_viewData->cellArray[m_viewData->mapSize.width() * y + x].type = MapViewCell::CT_Dest;
}

void MapView::findPath()
{
    // 没有指定起始点
    if (!m_viewData->hasStartCell) {
        QMessageBox::information(this, "Error", "Need set a start point!");
        return;
    }
    // 没有指定目标点
    if (!m_viewData->hasDestCell) {
        QMessageBox::information(this, "Error", "Need set a dest point!");
        return;
    }

    QPoint pts[] = {
        QPoint(1, 0),
        QPoint(0, 1),
        QPoint(-1, 0),
        QPoint(0, -1)
    };
    QPoint startPt = m_viewData->startCellPt;
    QPoint destPt = m_viewData->destCellPt;
    int mapw = m_viewData->mapSize.width();
    int maph = m_viewData->mapSize.height();
    QList<MapViewCell*> openList;
    QList<MapViewCell*> closeList;
    MapViewCell *currentCell = Q_NULLPTR;
    MapViewCell *tempCell = Q_NULLPTR;
    MapViewCell *startCell = Q_NULLPTR;
    bool findDestPt = false;

    // 将起始点加入openlist
    startCell = &m_viewData->cellArray[mapw * startPt.y() + startPt.x()];
    startCell->g = 0;
    startCell->h = qAbs(destPt.x() - startPt.x()) + qAbs(destPt.y() - startPt.y());
    openList.push_back(startCell);

    while (true) {
        // 遍历openlist,查找F值最小的节点, f = g + h;
        currentCell = Q_NULLPTR;
        for (QList<MapViewCell*>::iterator iter = openList.begin(); iter != openList.end(); ++ iter) {
            if (!currentCell) {
                currentCell = (*iter);
            } else {
                if (currentCell->g + currentCell->h > (*iter)->g + (*iter)->h) {
                    currentCell = (*iter);
                }
            }
        }

        // 没有找到路径
        if (!currentCell) {
            qDebug() << "current cell is null.";
            break;
        }

        // 把这个点移到closelist,并从Openlist移除
        openList.removeOne(currentCell);
        closeList.push_back(currentCell);

        // 对周围的方格做...
        for (int i = 0; i < 4; ++ i) {
            QPoint pt = QPoint(currentCell->x + pts[i].x(), currentCell->y + pts[i].y());
            if (pt.x() < 0 || pt.x() >= mapw || pt.y() < 0 || pt.y() >= maph)
                continue;
            tempCell = &m_viewData->cellArray[mapw * pt.y() + pt.x()];
            if (tempCell->type == MapViewCell::CT_Dest) {
                tempCell->parent = currentCell;
                findDestPt = true;
                break;
            } else if (tempCell->type != MapViewCell::CT_Walkable || closeList.contains(tempCell)) {

            } else {
                if (!openList.contains(tempCell)) {
                    tempCell->parent = currentCell;
                    tempCell->g = currentCell->g + 1;
                    tempCell->h = qAbs(tempCell->x - destPt.x()) + qAbs(tempCell->y - destPt.y());
                    openList.push_back(tempCell);
                } else {
                    int g = currentCell->g + 1;
                    if (g < tempCell->g) {
                        tempCell->g = g;
                        tempCell->parent = currentCell;
                    }
                }
            }
        }

        if (findDestPt) {
            qDebug() << "Dest point has found.";
            break;
        }
    }

    if (!findDestPt) {
        QMessageBox::information(this, "Error", "No valid path.");
        return;
    }

    QList<MapViewCell*> pathList;
    MapViewCell *cell = &m_viewData->cellArray[mapw * destPt.y() + destPt.x()];
    while (true) {
        cell = cell->parent;
        if (cell->type == MapViewCell::CT_Start) {
            break;
        }
        pathList.push_back(cell);
        cell->type = MapViewCell::CT_Path;
    }

    qDebug() << pathList.size();
    update();
}

void MapView::cleanPath()
{
    int w = m_viewData->mapSize.width();

    for (int x = 0; x < m_viewData->mapSize.width(); ++ x) {
        for (int y = 0; y < m_viewData->mapSize.height(); ++ y) {
            MapViewCell *cell = &m_viewData->cellArray[w * y + x];
            if (cell->type == MapViewCell::CT_Path) {
                cell->type = MapViewCell::CT_Walkable;
            }
        }
    }
}

void MapView::paintEvent(QPaintEvent *)
{
    if (!m_viewData->cellArray)
        return;

    QPainter painter(this);
    int mapw = m_viewData->mapSize.width();
    int maph = m_viewData->mapSize.height();
    int cellw = m_viewData->cellSize.width();
    int cellh = m_viewData->cellSize.height();
    MapViewCellOption *opt = Q_NULLPTR;

    for (int x = 0; x < mapw; ++ x) {
        //
        for (int y = 0; y < maph; ++ y) {
            //
            switch (m_viewData->cellArray[mapw * y + x].type) {
            case MapViewCell::CT_Walkable:
                opt = &m_viewData->walkableCellOpt;
                break;
            case MapViewCell::CT_Unwalkable:
                opt = &m_viewData->unwalkableCellOpt;
                break;
            case MapViewCell::CT_Start:
                opt = &m_viewData->startCellOpt;
                break;
            case MapViewCell::CT_Dest:
                opt = &m_viewData->destCellOpt;
                break;
            case MapViewCell::CT_Path:
                opt = &m_viewData->pathCellOpt;
                break;
            }
            // Draw box
            painter.setPen(opt->borderPen);
            painter.setBrush(opt->backgroundBrush);
            painter.drawRect(cellw * x, cellh * y, cellw, cellh);
            // Draw text if text not empty
            if (!opt->text.isEmpty()) {
                QPen p(Qt::SolidLine);
                p.setColor(opt->textColor);
                opt->textFont.setPixelSize(qMin(cellw, cellh) * 2 / 3);
                painter.setPen(p);
                painter.setFont(opt->textFont);
                painter.drawText(cellw * x, cellh * y, cellw, cellh, Qt::AlignCenter, opt->text);
            }
        }
    }
}

void MapView::mousePressEvent(QMouseEvent *e)
{
    mouseInteracte(e);
}

void MapView::mouseMoveEvent(QMouseEvent *e)
{
    mouseInteracte(e);
}

void MapView::mouseReleaseEvent(QMouseEvent *)
{

}

void MapView::mouseInteracte(QMouseEvent *e)
{
    int x = e->pos().x() / m_viewData->cellSize.width();
    int y = e->pos().y() / m_viewData->cellSize.height();
    int mapw = m_viewData->mapSize.width();

    switch (m_interacteMode) {
    case IM_None:
        break;
    case IM_SetWalkable:
        m_viewData->cellArray[mapw * y + x].type = MapViewCell::CT_Walkable;
        break;
    case IM_SetUnwalkable:
        m_viewData->cellArray[mapw * y + x].type = MapViewCell::CT_Unwalkable;
        break;
    case IM_SetStart:
        setStartCellPoint(x, y);
        break;
    case IM_SetDest:
        setDestCellPoint(x, y);
        break;
    default:
        break;
    }

    update();
}
