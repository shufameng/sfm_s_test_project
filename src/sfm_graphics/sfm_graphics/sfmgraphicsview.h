#ifndef SFMGRAPHICSVIEW_H
#define SFMGRAPHICSVIEW_H

#include <QGraphicsView>

class SFMGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    enum Tool {
        Tool_ResizeableRect = 0,
        Tool_Square,
        Tool_ResizeableEllipse,
        Tool_Round,
        Tool_Pixmap,
        Tool_Select
    };

    explicit SFMGraphicsView(QWidget *parent = Q_NULLPTR);
    ~SFMGraphicsView();

    void setPixmapForItem(const QPixmap &p) {
        m_pixmapForItem = p;
    }

    void setCurrentTool(Tool t);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
signals:
    void currentToolChanged(int oldTool, int currentTool);
private:
    bool m_isLButtonOnPress;

    Tool m_currentTool;

    QPixmap m_pixmapForItem;
    QString m_pixmapForItemFilePath;
};

#endif // SFMGRAPHICSVIEW_H
