#include "finboxdialog.h"
#include "ui_finboxdialog.h"
#include "sfmflashplayerdialog.h"

FInBoxDialog::FInBoxDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FInBoxDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_open_flash_use_f_in_box, &QPushButton::clicked, [this]() {
        SFMFlashPlayerDialog d;
        d.loadOcx(ui->comboBox_flashocx_path->currentText());
        d.createFlashPlayerControl();
        d.play(ui->comboBox_flashfile_path->currentText());
        d.exec();
    });

}

FInBoxDialog::~FInBoxDialog()
{
    delete ui;
}

