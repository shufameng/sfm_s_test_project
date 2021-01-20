#ifndef SFMWAR3TOOLWINDOW_H
#define SFMWAR3TOOLWINDOW_H

#include <QMainWindow>
#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui { class SfmWar3ToolWindow; }
QT_END_NAMESPACE


//
class SFMAbility {
public:
    enum AttributeType {
        AT_Strength = 0,
        AT_Agile,
        AT_Intelligence,
        AT_AttackPoint,
        AT_AttackSpeed,
        AT_Defense,
        AT_MoveSpeed,
        AT_HealthPoint,
        AT_HealthRecovery,
        AT_ManaPoint,
        AT_ManaRecovery
    };

    static QString getAttributeName(AttributeType attrType) {
        switch (attrType) {
        case AT_Strength:            return QString::fromLocal8Bit("Strength");
        case AT_Agile:               return QString::fromLocal8Bit("Agile");
        case AT_Intelligence:        return QString::fromLocal8Bit("Intelligence");
        case AT_AttackPoint:         return QString::fromLocal8Bit("AttackPoint");
        case AT_AttackSpeed:         return QString::fromLocal8Bit("AttackSpeed");
        case AT_Defense:             return QString::fromLocal8Bit("Defense");
        case AT_MoveSpeed:           return QString::fromLocal8Bit("MoveSpeed");
        case AT_HealthPoint:         return QString::fromLocal8Bit("HealthPoint");
        case AT_HealthRecovery:      return QString::fromLocal8Bit("HealthRecovery");
        case AT_ManaPoint:           return QString::fromLocal8Bit("ManaPoint");
        case AT_ManaRecovery:        return QString::fromLocal8Bit("ManaRecovery");
        default:
            return QString();
        }
    }

    QString getDescription() const {
        return QString("%1+%2")
                .arg(getAttributeName(type))
                .arg(value.toString());
    }

    SFMAbility(AttributeType type, const QVariant &value)
    {
        this->type = type;
        this->value = value;
    }

    AttributeType type;
    QVariant value;
};

//
class SFMAbilityGroup {
public:
    SFMAbilityGroup() {}

    QString getDescription() const {
        QString ret;
        foreach (const SFMAbility &v, abilityList) {
            ret += v.getDescription();
        }
        return ret;
    }

    QList<SFMAbility> abilityList;
};



//
class SfmWar3ToolWindow : public QMainWindow
{
    Q_OBJECT

public:
    SfmWar3ToolWindow(QWidget *parent = nullptr);
    ~SfmWar3ToolWindow();

private slots:
    void on_pushButton_itemDesignTest_clicked();

private:
    Ui::SfmWar3ToolWindow *ui;
};
#endif // SFMWAR3TOOLWINDOW_H
