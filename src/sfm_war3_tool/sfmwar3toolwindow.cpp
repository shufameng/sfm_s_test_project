#include "sfmwar3toolwindow.h"
#include "ui_sfmwar3toolwindow.h"
#include <QDebug>

SfmWar3ToolWindow::SfmWar3ToolWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SfmWar3ToolWindow)
{
    ui->setupUi(this);


}

SfmWar3ToolWindow::~SfmWar3ToolWindow()
{
    delete ui;
}


void SfmWar3ToolWindow::on_pushButton_itemDesignTest_clicked()
{
#if 0
    SFMAbilityGroup abil_str;
    abil_str.abilityList.append(SFMAbility(SFMAbility::AT_Strength, 3));
    SFMAbilityGroup abil_agi;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_Agile, 3));
    SFMAbilityGroup abil_int;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_Intelligence, 3));
    SFMAbilityGroup abil_allstate;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_Strength, 1));
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_Agile, 1));
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_Intelligence, 1));
    SFMAbilityGroup abil_ap;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_AttackPoint, 5));
    SFMAbilityGroup abil_as;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_AttackSpeed, 0.1));
    SFMAbilityGroup abil_def;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_Defense, 2));
    SFMAbilityGroup abil_ms;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_MoveSpeed, 30));
    SFMAbilityGroup abil_hp;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_HealthPoint, 75));
    SFMAbilityGroup abil_hprec;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_HealthRecovery, 1));
    SFMAbilityGroup abil_mp;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_ManaPoint, 50));
    SFMAbilityGroup abil_mprec;
    abil_agi.abilityList.append(SFMAbility(SFMAbility::AT_ManaRecovery, 0.25));

    QList<SFMAbilityGroup> list;
    list.append(abil_str);
    list.append(abil_agi);
    list.append(abil_int);
    list.append(abil_allstate);
    list.append(abil_ap);
    list.append(abil_as);
    list.append(abil_def);
    list.append(abil_ms);
    list.append(abil_hp);
    list.append(abil_hprec);
    list.append(abil_mp);
    list.append(abil_mprec);

    QString log;
    SFMAbilityGroup grp;

    for (int i = 0; i < list.size(); ++ i) {
        log += list.at(i).getDescription() + "\n";

        for (int j = i + 1; j < list.size(); ++ j) {
            grp.abilityList.clear();

        }
    }
#endif

    QStringList list;
    list << QString::fromLocal8Bit("Strength+3")
         << QString::fromLocal8Bit("Agile+3")
         << QString::fromLocal8Bit("Intelligence+3")
         << QString::fromLocal8Bit("AllState+1")
         << QString::fromLocal8Bit("AttackPoint+5")
         << QString::fromLocal8Bit("AttackSpeed+10%;")
         << QString::fromLocal8Bit("Defense+2;")
         << QString::fromLocal8Bit("MoveSpeed+30")
         << QString::fromLocal8Bit("HealthPoint+75")
         << QString::fromLocal8Bit("HealthRecovery+1")
         << QString::fromLocal8Bit("ManaPoint+50")
         << QString::fromLocal8Bit("ManaRecovery+25%");

    QStringList strList;
    QString str;

    for (int i = 0; i < list.size(); ++ i) {
        str.clear();
        str += list.at(i) + ";\n";
        strList.append(str);

        for (int j = i + 1; j < list.size(); ++ j) {
            str.clear();
            str += list.at(i) + ";" + list.at(j) + ";\n";
            strList.append(str);
        }
    }

    QString text;
    foreach (const QString &str, strList) {
        text += str;
    }

    ui->textEdit_itemDesign->setText(text);

    qDebug() << strList.size();
}




















