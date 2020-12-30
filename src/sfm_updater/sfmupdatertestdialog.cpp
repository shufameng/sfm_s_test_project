#include "sfmupdatertestdialog.h"
#include "ui_sfmupdatertestdialog.h"
#include "sfmupdater.h"

SfmUpdaterTestDialog::SfmUpdaterTestDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SfmUpdaterTestDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_start_download, &QPushButton::clicked, [this]() {
        ui->progressBar_download_progress->setValue(0);

        SFMUpdater::download(_T("http://localhost/FlashDevelop.zip"),
                             _T("F:\\FlashDevelop.zip"),
                             (HWND)winId());
    });
}

SfmUpdaterTestDialog::~SfmUpdaterTestDialog()
{
    delete ui;
}

bool SfmUpdaterTestDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = static_cast<MSG*>(message);

    if (msg->message == WM_USER + 2) {

        SFMUpdaterDownloadResult *rst = (SFMUpdaterDownloadResult*)(msg->lParam);

        qDebug() << QString::fromWCharArray( rst->errorDesc );
    }

    return QDialog::nativeEvent(eventType, message, result);
}

