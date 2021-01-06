#include "sfmupdatertestdialog.h"
#include "ui_sfmupdatertestdialog.h"
#include "sfmupdater.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QtDebug>

SfmUpdaterTestDialog::SfmUpdaterTestDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SfmUpdaterTestDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_start_download, &QPushButton::clicked, [this]() {
        ui->progressBar_download_progress->setValue(0);

        SFMUpdater::downloadFile((HWND)winId(),
                                 ui->comboBox_download_url->currentText().toStdWString().c_str(),
                                 ui->comboBox_download_save_path->currentText().toStdWString().c_str());
    });

    connect(ui->pushButton_open_download_save_dir, &QPushButton::clicked, [this]() {
        QFileInfo info(ui->comboBox_download_save_path->currentText());
        QDir dir = info.dir();
        QDesktopServices::openUrl(dir.path());
    });

    connect(ui->pushButton_do_post, &QPushButton::clicked, [this]() {
        QString formData = ui->textEdit_post_formdata->toPlainText();
        QString url = ui->comboBox_post_url->currentText();
        ui->textEdit_post_replydata->clear();

        QUrl u(url);
        QString host = u.host();
        int port = u.port(80);
        QString location = u.path();

        SFMUpdater::httpPost((HWND)winId(),
                             host.toStdWString(),
                             port,
                             location.toStdWString(),
                             formData.toStdWString());

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
        QMessageBox::information(this, "Download File Finished", QString::fromStdString(rst->resMsg));
    }
    else if (msg->message == MSG_TYPE_DOWNLOAD_FILE_PROGRESS) {

    }
    else if (msg->message == MSG_TYPE_DOWNLOAD_FILE_BYTES_RECEIVED) {

    }
    else if (msg->message == MSG_TYPE_HTTP_POST_FINISHED) {
        SFMUpdaterHttpPostResult *rst = (SFMUpdaterHttpPostResult*)msg->lParam;
        ui->textEdit_post_replydata->setText(QString::fromStdString(rst->replyData));
        qDebug() << rst->replyData.size();
    }


    return QDialog::nativeEvent(eventType, message, result);
}

