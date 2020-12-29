#include "sfmupdatertestdialog.h"
#include "ui_sfmupdatertestdialog.h"

SfmUpdaterTestDialog::SfmUpdaterTestDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SfmUpdaterTestDialog)
{
    ui->setupUi(this);
}

SfmUpdaterTestDialog::~SfmUpdaterTestDialog()
{
    delete ui;
}

