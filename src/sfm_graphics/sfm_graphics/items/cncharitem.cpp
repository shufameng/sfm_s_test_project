#include "cncharitem.h"
#include <QFontMetricsF>
#include <QDebug>

CNCharItem::CNCharItem(QGraphicsItem *parent) :
    BaseItem(parent)
{
    m_CNCharFont.setPixelSize(164);
    m_CNCharFont.setFamily("KaiTi");
    m_spellFont.setPixelSize(48);
    setAcceptHoverEvents(true);

    //setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF CNCharItem::boundingRect() const
{
    return QRectF(0, 0, 260, 260);
}

void CNCharItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QBrush b(Qt::SolidPattern);
    if (option->state & QStyle::State_Selected) {
        b.setColor(QColor(255, 0, 0));
    }
    else if (option->state & QStyle::State_MouseOver) {
        b.setColor(QColor(200, 200, 200));
    }
    else {
        b.setColor(QColor(255, 255, 255));
    }

    painter->setBrush(b);
    painter->drawRect(boundingRect());

    if (option->state & QStyle::State_Sunken) {
        QPen p(Qt::DashLine);
        p.setColor(QColor(0, 255, 0));
        p.setWidthF(2);
        painter->setPen(p);
        painter->drawRect(boundingRect());
    }

    painter->restore();

#if 0
    painter->save();

   QString strState;
   strState.sprintf("%X", (int)option->state);
   qDebug() << "option->state = " << strState;

    painter->setRenderHints(QPainter::Antialiasing);

    QPen p(Qt::SolidLine);
    p.setColor(QColor(100, 100, 100));
    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(250, 250, 250));

    QRectF rct = boundingRect();
    rct.adjust(34, 4, -34, -4);

    QRectF CNChar_rct = rct;
    CNChar_rct.setTop(60);

    QRectF spell_rct = rct;
    spell_rct.setBottom(60);

    // Draw background
    painter->setPen(p);
    painter->setBrush(b);
    painter->drawRect(boundingRect());

    // Draw lines
    p.setColor(QColor(0, 100, 100));
    p.setStyle(Qt::SolidLine);
    p.setWidthF(2.0);
    painter->setPen(p);
    painter->drawLine(CNChar_rct.topLeft(), CNChar_rct.topRight());
    painter->drawLine(CNChar_rct.topLeft(), CNChar_rct.bottomLeft());
    painter->drawLine(CNChar_rct.bottomLeft(), CNChar_rct.bottomRight());
    painter->drawLine(CNChar_rct.topRight(), CNChar_rct.bottomRight());
    p.setWidthF(1.5);
    p.setStyle(Qt::DashLine);
    p.setCapStyle(Qt::RoundCap);
    p.setJoinStyle(Qt::RoundJoin);
    painter->setPen(p);
    painter->drawLine(CNChar_rct.left(), CNChar_rct.top() + CNChar_rct.height() / 2, CNChar_rct.right(), CNChar_rct.top() + CNChar_rct.height() / 2);
    painter->drawLine(CNChar_rct.left() + CNChar_rct.width() / 2, CNChar_rct.top(), CNChar_rct.left() + CNChar_rct.width() / 2, CNChar_rct.bottom());
    painter->drawLine(CNChar_rct.topLeft(), CNChar_rct.bottomRight());
    painter->drawLine(CNChar_rct.topRight(), CNChar_rct.bottomLeft());

    // Draw text
    p.setColor(QColor(0, 0, 0));
    painter->setPen(p);
    painter->setFont(m_CNCharFont);
    painter->drawText(CNChar_rct, m_CNChar, QTextOption(Qt::AlignCenter));

    painter->setFont(m_spellFont);
    painter->drawText(spell_rct, m_spell, QTextOption(Qt::AlignCenter));

    if (option->state & QStyle::State_Selected) {
        p.setColor(QColor(255, 0, 0));
        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
    if (option->state & QStyle::State_MouseOver) {
        p.setColor(QColor(0, 255, 0));
        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(1, 1, -1, -1));
    }

    painter->restore();
#endif
}

void CNCharItem::setCNChar(const QString &ch)
{
    m_CNChar = ch;
    update();
}

void CNCharItem::setSpell(const QString &sp)
{
    m_spell = sp;
    update();
}

void CNCharItem::setCNCharFont(const QFont &ft)
{
    m_CNCharFont = ft;
    update();
}

void CNCharItem::setSpellFont(const QFont &ft)
{
    m_spellFont = ft;
    update();
}

void CNCharItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    //qDebug() << "CNCharItem::hoverEnterEvent";
    BaseItem::hoverEnterEvent(e);
}

void CNCharItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
    //qDebug() << "CNCharItem::hoverMoveEvent";
    BaseItem::hoverMoveEvent(e);
}

void CNCharItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    //qDebug() << "CNCharItem::hoverLeaveEvent";
    BaseItem::hoverLeaveEvent(e);
}
