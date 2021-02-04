#ifndef SFMFIXEDDIALOG_H
#define SFMFIXEDDIALOG_H

#include <QDialog>

namespace Ui {
class SFMFixedDialog;
}

class SFMFixedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SFMFixedDialog(QWidget *parent = nullptr);
    ~SFMFixedDialog();

    bool  canDragTitleBarMove() const {
        return m_canDragTitleBarMove;
    }

    void setCanDragTitleBarMove(bool can) {
        m_canDragTitleBarMove = can;
    }

    QRect getTitleBarRect() const {
        return m_titleBarRect;
    }

    void setTitleBarRect(const QRect &r) {
        m_titleBarRect = r;
    }

    int getTitleBarHeight() const {
        return m_titleBarHeight;
    }

    void setTitleBarHeight(int h) {
        m_titleBarHeight = h;
    }

    void setWindowShadowImagePath(const QString &path);
    void setWindowShadowSize(int sz);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    bool event(QEvent *e) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    void getShadowPart(const QPixmap &pm);
private:
    Ui::SFMFixedDialog *ui;

    bool m_isLButtonOnPress;
    QPoint m_lButtonPressPos;

    bool m_canDragTitleBarMove;
    QRect m_titleBarRect;
    int m_titleBarHeight;

    QString m_windowShadowImagePath;
    QPixmap m_windowShadowPixmap;
    int m_windowShadowSize;
    int m_windowShadowSizeNotActive;
    QPixmap
    m_topLeftShadowPm,
    m_topShadowPm,
    m_topRightShadowPm,
    m_rightShadowPm,
    m_bottomRightShadowPm,
    m_bottomShadowPm,
    m_bottomLeftShadowPm,
    m_leftShadowPm;

    bool m_ncactive;
};

#endif // SFMFIXEDDIALOG_H
