#include "sfmcustomwidgetsdialog.h"
#include "ui_sfmcustomwidgetsdialog.h"
#include "inheritsfmfixeddialog.h"
#include <QDebug>
#include <QMessageBox>

SFMCustomWidgetsDialog::SFMCustomWidgetsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SFMCustomWidgetsDialog)
{
    ui->setupUi(this);
}

SFMCustomWidgetsDialog::~SFMCustomWidgetsDialog()
{
    delete ui;
}


void SFMCustomWidgetsDialog::on_pushButton_sfmfixeddialog_clicked()
{
    InheritSFMFixedDialog d(this);
    //d.resize(600, 400);
    d.exec();

    //QMessageBox::information(this, "msg", "this is a msg");
}

bool SFMCustomWidgetsDialog::event(QEvent *e)
{
    //qDebug() << "SFMCustomWidgetsDialog::event(), eventType = " << e->type();

    return QDialog::event(e);
}
