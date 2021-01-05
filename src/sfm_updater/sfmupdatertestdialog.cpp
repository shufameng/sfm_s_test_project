#include "sfmupdatertestdialog.h"
#include "ui_sfmupdatertestdialog.h"
#include "sfmupdater.h"
#include <QMessageBox>

SfmUpdaterTestDialog::SfmUpdaterTestDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SfmUpdaterTestDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_start_download, &QPushButton::clicked, [this]() {
        ui->progressBar_download_progress->setValue(0);

        SFMUpdater::downloadFile((HWND)winId(), _T("http://localhost/FlashDevelop.zip"),
                             _T("F:\\FlashDevelop.zip"));
    });
}

SfmUpdaterTestDialog::~SfmUpdaterTestDialog()
{
    delete ui;
}

bool SfmUpdaterTestDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = static_cast<MSG*>(message);

    if (msg->message == MSG_TYPE_DOWNLOAD_FILE_FINISHED) {
        SFMUpdaterDownloadResult *rst = (SFMUpdaterDownloadResult*)(msg->lParam);

    }
    else if (msg->message == MSG_TYPE_DOWNLOAD_FILE_PROGRESS) {

    }
    else if (msg->message == MSG_TYPE_DOWNLOAD_FILE_BYTES_RECEIVED) {

    }
    else if (msg->message == MSG_TYPE_POST_REQUEST_FINISHED) {

    }


    return QDialog::nativeEvent(eventType, message, result);
}

