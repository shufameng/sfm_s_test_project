#include "qtofficeembeddeddialog.h"
#include "ui_qtofficeembeddeddialog.h"
#include <QFileDialog>
#include <QAxObject>
#include <QMessageBox>
#include <QDebug>
#include <ShlObj.h>
#include <shellapi.h>
#include <QApplication>

QString QtOfficeEmbeddedDialog::sg_dsoframerId = "00460182-9E5E-11d5-B7C8-B8269041DD57";

QtOfficeEmbeddedDialog::QtOfficeEmbeddedDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QtOfficeEmbeddedDialog)
{
    ui->setupUi(this);
}

QtOfficeEmbeddedDialog::~QtOfficeEmbeddedDialog()
{
    delete ui;
}

bool QtOfficeEmbeddedDialog::isDsoframerRegistered()
{
    QAxObject obj;
    return obj.setControl(sg_dsoframerId);
}

bool QtOfficeEmbeddedDialog::isCurrentUserAnAdmin()
{
    return IsUserAnAdmin();
}

bool QtOfficeEmbeddedDialog::registerDsoframer()
{


    QString srcOcxPath = QDir(qApp->applicationDirPath()).filePath("dsoframer.ocx");
    QString batFilePath = QDir(qApp->applicationDirPath()).filePath("regdsoframer.bat");

    ShellExecuteW(NULL, L"runas", QDir::toNativeSeparators(batFilePath).toStdWString().c_str(), NULL, NULL, SW_HIDE);
    return true;

    QString cmd = QString("/k copy \"%1\" \"%2\"")
            .arg(QDir::toNativeSeparators(srcOcxPath))
            .arg("C:\\windows\\dsoframer.ocx");

    // Copy dsoframer.ocx to c:\\Windows
    ShellExecuteW(NULL, L"runas", L"cmd.exe", cmd.toStdWString().c_str(), NULL, SW_SHOW);

    // Register dsoframer use regsvr32 command
    cmd = QString("/k regsvr32 c:\\windows\\dsoframer.ocx");

    ShellExecuteW(NULL, L"runas", L"cmd.exe", cmd.toStdWString().c_str(), NULL, SW_SHOW);

    return true;
}



void QtOfficeEmbeddedDialog::on_pushButton_browse_clicked()
{
    registerDsoframer();return;

    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files(*.*);;Word(*.doc; *.docx;);;Excel(*.xls *.xlsx);;PPT(*.ppt *.pptx))");
    if (!filePath.isEmpty()) {
        ui->lineEdit_path->setText(filePath);
        openFile(filePath);
    }


}

const QString fileFilter = "Microsoft Office Files(*.doc *.docx *.docm *.rtf *.xls *.xlsx *.xlsm *.csv *.ppt *.pptx *.pptm *.mpp *.vsd *.vdx);;"
                           "Microsoft Word Files(*.doc *.docx *.docm *.rtf);;"
                           "Microsoft Excel Files(*.xls *.xlsx *.xlsm *.csv);;"
                           "Microsoft PowerPoint Files(*.ppt *.pptx *.pptm);;"
                           "Microsoft Project Files(*.mpp);;"
                           "Microsoft Visio Files(*.vsd *.vdx);;"
                           "All Files(*.*);;";

void QtOfficeEmbeddedDialog::openFile(const QString &filePath)
{
    QString curCtrl = ui->axWidget->control();
    QString dsoframerCtrl = "00460182-9E5E-11d5-B7C8-B8269041DD57";

    if (filePath.endsWith(".ppt", Qt::CaseInsensitive) ||
            filePath.endsWith(".pptx", Qt::CaseInsensitive) ||
            filePath.endsWith(".doc", Qt::CaseInsensitive) ||
            filePath.endsWith(".docx", Qt::CaseInsensitive) ||
            filePath.endsWith(".xls", Qt::CaseInsensitive) ||
            filePath.endsWith(".xlsx", Qt::CaseInsensitive)) {
        // Use dsoframer.ocx
        if (curCtrl != dsoframerCtrl) {
            if (!ui->axWidget->setControl(dsoframerCtrl)) {
                QMessageBox::information(this, "Error", "Can not open dsoframer control!");
                return;
            }
        }

        //
        //ui->axWidget->setProperty("Visible", true);
        ui->axWidget->dynamicCall("Open(string)", filePath);
        ui->axWidget->dynamicCall("SetMenuAccelerators(bool)", true);
        ui->axWidget->dynamicCall("SetMenubar(bool)", false);
        ui->axWidget->dynamicCall("SetTitlebar(bool)", false);
    } else {
        QMessageBox::information(this, "Error", "Unsupportted file format!");
    }
}
