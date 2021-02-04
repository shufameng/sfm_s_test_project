#include "sfmfixeddialog.h"
#include "ui_sfmfixeddialog.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

SFMFixedDialog::SFMFixedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SFMFixedDialog),
    m_isLButtonOnPress(false),
    m_canDragTitleBarMove(true),
    m_titleBarHeight(40),
    m_windowShadowSize(9),
    m_windowShadowSizeNotActive(7),
    m_ncactive(true)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

SFMFixedDialog::~SFMFixedDialog()
{
    delete ui;
}

void SFMFixedDialog::setWindowShadowImagePath(const QString &path)
{
    m_windowShadowImagePath = path;
    m_windowShadowPixmap.load(m_windowShadowImagePath);
    getShadowPart(m_windowShadowPixmap);
}

void SFMFixedDialog::setWindowShadowSize(int sz)
{
    m_windowShadowSize = sz;
    m_windowShadowSizeNotActive = sz - 2;
    getShadowPart(m_windowShadowPixmap);
    update();
}

void SFMFixedDialog::mousePressEvent(QMouseEvent *e)
{
    QDialog::mousePressEvent(e);
    if (e->button() == Qt::LeftButton && e->pos().y() <= m_titleBarHeight) {
        m_isLButtonOnPress = true;
        m_lButtonPressPos = e->pos();
    }
}

void SFMFixedDialog::mouseMoveEvent(QMouseEvent *e)
{
    QDialog::mouseMoveEvent(e);
    if (m_isLButtonOnPress && m_canDragTitleBarMove) {
        QPoint pt = pos() + e->pos() - m_lButtonPressPos;
        QRect screen_rect = qApp->primaryScreen()->availableGeometry();
        if (pt.y() > screen_rect.bottom() - 40) {
            pt.setY(screen_rect.bottom() - 40);
        }
        move(pt);
    }
}

void SFMFixedDialog::mouseReleaseEvent(QMouseEvent *e)
{
    QDialog::mouseReleaseEvent(e);
    if (e->button() & Qt::LeftButton) {
        m_isLButtonOnPress = false;
    }
}

void SFMFixedDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int w = m_windowShadowSize;

    if (m_ncactive || true) {
        painter.drawPixmap(0, 0, w, w, m_topLeftShadowPm);
        painter.drawPixmap(w, 0, width() - 2 * w, w, m_topShadowPm);
        painter.drawPixmap(0, w, w, height() - 2 * w, m_leftShadowPm);
        painter.drawPixmap(0, height() - w, w, w, m_bottomLeftShadowPm);
        painter.drawPixmap(w, height() - w, width() - 2 * w, w, m_bottomShadowPm);
        painter.drawPixmap(width() - w, 0, w, w, m_topRightShadowPm);
        painter.drawPixmap(width() - w, w, w, height() - 2 * w, m_rightShadowPm);
        painter.drawPixmap(width() - w, height() - w, w, w, m_bottomRightShadowPm);
    }

    QBrush b(Qt::SolidPattern);
    QPen p(Qt::SolidLine);

    if (m_ncactive) {
        p.setColor(QColor(200, 200, 200, 230));
        b.setColor(QColor("#f7f7f7"));
    } else {
        p.setColor(QColor(200, 200, 200, 255));
        b.setColor(QColor("#f4f4f4"));
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(b);
    painter.drawRect(w, w, width() - 2 * w, height() - 2 * w);
}

bool SFMFixedDialog::event(QEvent *e)
{
    return QDialog::event(e);
}

bool SFMFixedDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = (MSG*)message;
    if (msg->message == WM_NCACTIVATE) {
        m_ncactive = (bool)msg->wParam;
        update();
        return true;
    }
    return QDialog::nativeEvent(eventType, message, result);
}

void SFMFixedDialog::getShadowPart(const QPixmap &pm)
{
    int w = m_windowShadowSize;
    QSize sz = pm.size();

    m_topLeftShadowPm = pm.copy(0, 0, w, w);
    m_topRightShadowPm = pm.copy(sz.width() - w, 0, w, w);
    m_bottomLeftShadowPm = pm.copy(0, sz.height() - w, w, w);
    m_bottomRightShadowPm = pm.copy(sz.width() - w, sz.height() - w, w, w);

    m_topShadowPm = pm.copy(w, 0, sz.width() - w * 2, w);
    m_rightShadowPm = pm.copy(sz.width() - w, w, w, sz.height() - w * 2);
    m_bottomShadowPm = pm.copy(w, sz.height() - w, sz.width() - w * 2, w);
    m_leftShadowPm = pm.copy(0, w, w, sz.height() - w * 2);
}

