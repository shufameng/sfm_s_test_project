#include "inheritsfmfixeddialog.h"
#include "ui_inheritsfmfixeddialog.h"

InheritSFMFixedDialog::InheritSFMFixedDialog(QWidget *parent) :
    SFMFixedDialog(parent),
    ui(new Ui::InheritSFMFixedDialog)
{
    ui->setupUi(this);

    setWindowShadowSize(16);
    setWindowShadowImagePath("F:/document/wnd_shadow.png");
    //setWindowShadowImagePath("F:/vs_test_projects/SetupInterface/Release/WindowShadow.png");
    setTitleBarHeight(62);

}

InheritSFMFixedDialog::~InheritSFMFixedDialog()
{
    delete ui;
}

void InheritSFMFixedDialog::on_toolButton_closeWindow_clicked()
{
    close();
}
