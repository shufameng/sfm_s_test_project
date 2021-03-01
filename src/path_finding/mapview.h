#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>

class MapViewData;
class MapViewOption;
class MapView : public QWidget
{
    Q_OBJECT
public:
    enum InteracteMode {
        IM_None = 0,
        IM_SetWalkable,
        IM_SetUnwalkable,
        IM_SetStart,
        IM_SetDest
    };

    explicit MapView(QWidget *parent = nullptr);

    static int getRandomInt(int lowBound, int highBound);

    void setMapSize(int w, int h);
    QSize getMapSize() const;
    void setCellSize(int w, int h);
    QSize getCellSize() const;
    void setUnwalkablePercent(qreal f);
    qreal getUnwalkablePercent() const;
    void fillMapArrayWithWalkable();
    void updateMinsize();
    void generateRandomMap();
    void generateEmptyMap();
    void cleanMap();
    void setStartCellPoint(int x, int y);
    void setDestCellPoint(int x, int y);

    InteracteMode getInteracteMode() const {
        return m_interacteMode;
    }

    void setInteractMode(InteracteMode mode) {
        if (m_interacteMode != mode) {
            emit interacteModeChanged(m_interacteMode, mode);
            m_interacteMode = mode;
        }
    }

    void findPath();
    void cleanPath();

signals:
    void interacteModeChanged(InteracteMode oldMode, InteracteMode currentMode);

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseInteracte(QMouseEvent *e);

    MapViewData *m_viewData;
    InteracteMode m_interacteMode;
};

#endif // MAPVIEW_H
