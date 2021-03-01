#ifndef PATHFINDINGDIALOG_H
#define PATHFINDINGDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QAbstractButton>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class PathFindingDialog; }
QT_END_NAMESPACE

//
class MapCellOptionA
{
public:
    enum CellType {
        CT_Passable = 0,
        CT_Unpassable,
        CT_Path,
        CT_Start,
        CT_Dest
    };

    MapCellOptionA() {}
    QBrush backgroundBrush;
    QPen borderPen;
    QColor textColor;
    QFont textFont;
    QString text;
};

//
class MapViewDataA
{
public:
    MapViewDataA() {}

    int mapWidth, mapHeight, cellWidth, cellHeight;
    qreal unpassablePercent;
    int **cells;

    MapCellOptionA
    passableCellOption,
    unpassableCellOption,
    pathCellOption,
    startCellOption,
    destCellOption;
};

//
class MapViewA : public QWidget
{
    Q_OBJECT
public:
    enum InteractMode {
        IM_None = 0,
        IM_SetPassable,
        IM_SetUnpassable,
        IM_SetStart,
        IM_SetDest
    };

    explicit MapViewA(QWidget *parent = Q_NULLPTR) :
        QWidget(parent),
        m_cells(NULL),
        m_unpassablePercent(0.2),
        m_interactMode(IM_None),
        m_startX(-1), m_startY(-1), m_destX(-1), m_destY(-1)
    {
        m_passableCellOption.backgroundBrush.setStyle(Qt::SolidPattern);
        m_passableCellOption.backgroundBrush.setColor(QColor(1, 180, 247));
        m_passableCellOption.borderPen.setStyle(Qt::SolidLine);
        m_passableCellOption.borderPen.setColor(QColor(0, 0, 0));
        m_passableCellOption.text = "";
        m_passableCellOption.textColor = QColor(255, 255, 255);

        m_unpassableCellOption.backgroundBrush.setStyle(Qt::SolidPattern);
        m_unpassableCellOption.backgroundBrush.setColor(QColor(0, 0, 0));
        m_unpassableCellOption.borderPen.setStyle(Qt::SolidLine);
        m_unpassableCellOption.borderPen.setColor(QColor(0, 0, 0));
        m_unpassableCellOption.text = "";
        m_unpassableCellOption.textColor = QColor(255, 255, 255);

        m_pathCellOption.backgroundBrush.setStyle(Qt::SolidPattern);
        m_pathCellOption.backgroundBrush.setColor(QColor(0, 255, 0));
        m_pathCellOption.borderPen.setStyle(Qt::SolidLine);
        m_pathCellOption.borderPen.setColor(QColor(0, 0, 0));
        m_pathCellOption.text = "";
        m_pathCellOption.textColor = QColor(255, 255, 255);

        m_startCellOption.backgroundBrush.setStyle(Qt::SolidPattern);
        m_startCellOption.backgroundBrush.setColor(QColor(255, 0, 0));
        m_startCellOption.borderPen.setStyle(Qt::SolidLine);
        m_startCellOption.borderPen.setColor(QColor(0, 0, 0));
        m_startCellOption.text = "S";
        m_startCellOption.textColor = QColor(255, 255, 255);

        m_destCellOption.backgroundBrush.setStyle(Qt::SolidPattern);
        m_destCellOption.backgroundBrush.setColor(QColor(255, 0, 0));
        m_destCellOption.borderPen.setStyle(Qt::SolidLine);
        m_destCellOption.borderPen.setColor(QColor(0, 0, 0));
        m_destCellOption.text = "D";
        m_destCellOption.textColor = QColor(255, 255, 255);

        m_mapWidth = 0;
        m_mapHeight = 0;
        m_cellWidth = 0;
        m_cellHeight = 0;
        setMapSize(30, 20);
        setCellSize(36, 36);
    }

    //
    void setMapSize(int w, int h) {
        // Init cells array, and init it with value CT_Passable
        if (m_cells) {
            for (int i = 0; i < m_mapWidth; ++ i) {
                delete [] m_cells[i];
            }
            delete [] m_cells;
        }
        m_cells = new int*[w];
        for (int x = 0; x < w; ++ x) {
            m_cells[x] = new int[h];
            for (int y = 0; y < h; ++ y) {
                m_cells[x][y] = MapCellOptionA::CT_Passable;
            }
        }

        m_mapWidth = w;
        m_mapHeight = h;

        setMinimumSize(m_mapWidth * m_cellWidth + 1, m_mapHeight * m_cellHeight + 1);
        update();
    }

    //
    void setCellSize(int w, int h) {
        m_cellWidth = w;
        m_cellHeight = h;
        setMinimumSize(m_mapWidth * m_cellWidth + 1, m_mapHeight * m_cellHeight + 1);
        update();
    }

    //
    void cleanMap() {
        for (int x = 0; x < m_mapWidth; ++ x) {
            for (int y = 0; y < m_mapHeight; ++ y) {
                m_cells[x][y] = MapCellOptionA::CT_Passable;
            }
        }
        m_startX = m_startY = m_destX = m_destY = -1;
    }

    //
    void generateRandomMap() {
        // Clean Map
        cleanMap();

        // Generate start and dest
        m_startX = getRandomInt(0, m_mapWidth - 1);
        m_startY = getRandomInt(0, m_mapHeight - 1);
        m_cells[m_startX][m_startY] = MapCellOptionA::CT_Start;
        while (true) {
            m_destX = getRandomInt(0, m_mapWidth - 1);
            m_destY = getRandomInt(0, m_mapHeight - 1);
            if (m_cells[m_destX][m_destY] == MapCellOptionA::CT_Passable) {
                m_cells[m_destX][m_destY] = MapCellOptionA::CT_Dest;
                break;
            }
        }

        // Generate passable and unpassable
        for (int x = 0; x < m_mapWidth; ++ x) {
            for (int y = 0; y < m_mapHeight; ++ y) {
                qreal f = 1 / m_unpassablePercent;
                if (m_cells[x][y] == MapCellOptionA::CT_Passable) {
                    if (f > 1.0) {
                        int radomInt = getRandomInt(1, (int)f);
                        if (radomInt == 1) {
                            m_cells[x][y] = MapCellOptionA::CT_Unpassable;
                        } else {
                            m_cells[x][y] = MapCellOptionA::CT_Passable;
                        }
                    } else {
                        m_cells[x][y] = MapCellOptionA::CT_Unpassable;
                    }
                }
            }
        }
    }

    //
    int getMapWidth() const {
        return m_mapWidth;
    }

    //
    int getMapHeight() const {
        return m_mapHeight;
    }

    //
    int getCellWidth() const {
        return m_cellWidth;
    }

    //
    int getCellHeight() const {
        return m_cellHeight;
    }

    //
    qreal getUnpassablePercent() const {
        return m_unpassablePercent;
    }

    //
    void setUnpassablePercent(qreal p) {
        m_unpassablePercent = p;
    }

    //
    InteractMode getInteractMode() const {
        return m_interactMode;
    }

    //
    void setInteractMode(InteractMode md) {
        if (m_interactMode != md) {
            m_interactMode = md;
            emit interactModeChanged(md);
        }
    }

    //
    void findPathAStar(QString &errorString, bool &hasPath) {
        //
        QPoint pts[] = {
            QPoint(1, 0),
            QPoint(0, 1),
            QPoint(-1, 0),
            QPoint(0, -1)
        };
        QList<QPoint> openList;
    }

    //
    static int getRandomInt(int lowBound, int highBound) {
        return rand() % (highBound - lowBound + 1) + lowBound;
    }

protected:
    int m_mapWidth;
    int m_mapHeight;
    int m_cellWidth;
    int m_cellHeight;
    int **m_cells;
    qreal m_unpassablePercent;
    MapCellOptionA m_passableCellOption;
    MapCellOptionA m_unpassableCellOption;
    MapCellOptionA m_pathCellOption;
    MapCellOptionA m_startCellOption;
    MapCellOptionA m_destCellOption;
    InteractMode m_interactMode;
    int m_startX, m_startY, m_destX, m_destY;

    //
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        MapCellOptionA *opt = NULL;
        for (int x = 0; x < m_mapWidth; ++ x) {
            //
            for (int y = 0; y < m_mapHeight; ++ y) {
                // Check cell type
                if (m_cells[x][y] == MapCellOptionA::CT_Passable) {
                    opt = &m_passableCellOption;
                } else if (m_cells[x][y] == MapCellOptionA::CT_Unpassable) {
                    opt = &m_unpassableCellOption;
                } else if (m_cells[x][y] == MapCellOptionA::CT_Path) {
                    opt = &m_pathCellOption;
                } else if (m_cells[x][y] == MapCellOptionA::CT_Start) {
                    opt = &m_startCellOption;
                } else if (m_cells[x][y] == MapCellOptionA::CT_Dest) {
                    opt = &m_destCellOption;
                }

                // Draw box
                painter.setPen(opt->borderPen);
                painter.setBrush(opt->backgroundBrush);
                painter.drawRect(m_cellWidth * x, m_cellHeight * y, m_cellWidth, m_cellHeight);
                // Draw text if text not empty
                if (!opt->text.isEmpty()) {
                    QPen p(Qt::SolidLine);
                    p.setColor(opt->textColor);
                    opt->textFont.setBold(true);
                    opt->textFont.setPixelSize(qMin(m_cellWidth, m_cellHeight) * 2 / 3);
                    painter.setPen(p);
                    painter.setFont(opt->textFont);
                    painter.drawText(m_cellWidth * x, m_cellHeight * y, m_cellWidth, m_cellHeight, Qt::AlignCenter, opt->text);
                }
            }
        }
    }

    //
    void mousePressEvent(QMouseEvent *e) override {
        onMouseMove(e);
    }

    //
    void mouseMoveEvent(QMouseEvent *e) override {
        onMouseMove(e);
    }

    //
    void onMouseMove(QMouseEvent *e) {
        int x = e->pos().x() / m_cellWidth;
        int y = e->pos().y() / m_cellHeight;

        switch (m_interactMode) {
        case IM_None:
            break;
        case IM_SetPassable:
            m_cells[x][y] = MapCellOptionA::CT_Passable;
            break;
        case IM_SetUnpassable:
            m_cells[x][y] = MapCellOptionA::CT_Unpassable;
            break;
        case IM_SetStart:
            if (m_startX >= 0 && m_startY >= 0) {
                m_cells[m_startX][m_startY] = MapCellOptionA::CT_Passable;
            }
            m_startX = x;
            m_startY = y;
            m_cells[x][y] = MapCellOptionA::CT_Start;
            break;
        case IM_SetDest:
            if (m_destX >= 0 && m_destY >= 0) {
                m_cells[m_destX][m_destY] = MapCellOptionA::CT_Passable;
            }
            m_destX = x;
            m_destY = y;
            m_cells[x][y] = MapCellOptionA::CT_Dest;
            break;
        }

        //update(m_cellWidth * x, m_cellHeight * y, m_cellWidth, m_cellHeight);
        update();
    }

signals:
    void interactModeChanged(InteractMode mode);
};


//
class MapView;
class PathFindingDialog : public QDialog
{
    Q_OBJECT

public:
    PathFindingDialog(QWidget *parent = nullptr);
    ~PathFindingDialog();

private slots:
    void on_pushButton_genRandomMap_clicked();

    void on_pushButton_genEmptyMap_clicked();

    void onButtonGroupButtonClicked(QAbstractButton *button);

    void on_pushButton_findPath_clicked();

private:
    Ui::PathFindingDialog *ui;
    MapView *m_view;
};
#endif // PATHFINDINGDIALOG_H
