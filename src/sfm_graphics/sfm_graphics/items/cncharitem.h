#ifndef CNCHARITEM_H
#define CNCHARITEM_H

#include "baseitem.h"

class CNCharItem : public BaseItem
{
public:
    explicit CNCharItem(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setCNChar(const QString &ch);

    QString CNChar() const {
        return m_CNChar;
    }

    void setSpell(const QString &sp);

    QString spell() const {
        return m_spell;
    }

    void setCNCharFont(const QFont &ft);

    QFont CNCharFont() const {
        return m_CNCharFont;
    }

    void setSpellFont(const QFont &ft);

    QFont spellFont() const {
        return m_spellFont;
    }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *e) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e) override;

private:
    QString m_CNChar;
    QString m_spell;
    QFont m_CNCharFont;
    QFont m_spellFont;
};

#endif // CNCHARITEM_H
