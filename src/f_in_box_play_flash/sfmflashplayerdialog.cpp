#include "sfmflashplayerdialog.h"
#include <QFile>
#include <QtDebug>
#include <QMessageBox>

SFMFlashPlayerDialog::SFMFlashPlayerDialog(QWidget *parent) :
    QDialog(parent),
     m_hFPC(nullptr),
    m_hwndFlashPlayer(nullptr)
{
    m_ocxPath = "Flash.ocx";
}

SFMFlashPlayerDialog::~SFMFlashPlayerDialog()
{

}

void SFMFlashPlayerDialog::createFlashPlayerControl()
{
    if (!m_hFPC)
        return;

    deleteFlashPlayerControl();

    m_hwndFlashPlayer = FPC_CreateWindow(
                m_hFPC,
                0,
                NULL,
                WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                0,
                0,
                width(),
                height(),
                (HWND)winId(),
                NULL,
                NULL,
                NULL
                );

    FPC_EnableFullScreen(m_hwndFlashPlayer, TRUE);
    FPC_EnableSound(m_hFPC, TRUE);
}

void SFMFlashPlayerDialog::deleteFlashPlayerControl()
{
    if (m_hwndFlashPlayer) {
        ::DestroyWindow(m_hwndFlashPlayer);
        m_hwndFlashPlayer = nullptr;
    }
}

bool SFMFlashPlayerDialog::loadOcx(const QString &ocxPath)
{
    QFile ocxFile(ocxPath);
    if (!ocxFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", ocxFile.errorString());
        return false;
    }

    m_ocxByteArray = ocxFile.readAll();
    ocxFile.close();

    m_hFPC = FPC_LoadOCXCodeFromMemory(m_ocxByteArray.data(), m_ocxByteArray.size());
    if (!m_hFPC) {
        QMessageBox::critical(this, "Error", "FPC_LoadOCXCodeFromMemory() returns null");
        return false;
    } else {
        return true;
    }
}

void SFMFlashPlayerDialog::play(const QString &flashPath)
{
    if (m_hwndFlashPlayer) {
        FPC_LoadMovie(m_hwndFlashPlayer, 0, flashPath.toStdWString().c_str());
    }
}

bool SFMFlashPlayerDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = (MSG*)message;

    if (WM_NOTIFY == msg->message) {

        LPNMHDR lpNMHDR = (LPNMHDR)msg->lParam;
        if (m_hwndFlashPlayer == lpNMHDR->hwndFrom) {
            // Flash_Call
            if (lpNMHDR->code == FPCN_FLASHCALL) {
                SFPCFlashCallInfoStruct *pInfo = (SFPCFlashCallInfoStruct*)lpNMHDR;
                QString xmlStr = QString::fromWCharArray(pInfo->request);

                // debug
                qDebug() << "ZLYFlashPlayerDialog::nativeEvent() -> " << xmlStr;

                if (xmlStr.contains("isReady")) {
                    FPCSetReturnValue(m_hwndFlashPlayer, _T("<string>true</string>"));
                } else if (xmlStr.contains("setSWFIsReady")) {
                    FPCSetReturnValue(m_hwndFlashPlayer, _T("<string>true</string>"));
                } else if (xmlStr.contains("setStatus")) {
                    QString qstrFunc =
                            QString("<invoke name=\"%1\" returntype=\"xml\"><arguments><string>%2</string></arguments></invoke>")
                            .arg("outputtttttt")
                            .arg(1);
                    FPCCallFunctionA(m_hwndFlashPlayer, qstrFunc.toLocal8Bit().data(), NULL, NULL);
                } else if (xmlStr.contains("PurchaseCourses")) {
                    close();
//                    emit purchaseCourses();
                } else if (xmlStr.contains("flashClose")) {
                    close();
                } else if (xmlStr.contains("Loading")) {
                    QString qstrFunc =
                            QString("<invoke name=\"%1\" returntype=\"xml\"><arguments><string>%2</string></arguments></invoke>")
                            .arg("cSharpCallFlash")
                            .arg(1);
                    FPCCallFunctionA(m_hwndFlashPlayer, qstrFunc.toLocal8Bit().data(), NULL, NULL);
                }
            // Load_External_Resource_EX
            } else if (lpNMHDR->code == FPCN_LOADEXTERNALRESOURCEEX) {
                //
            // Load_External_Resource
            } else if (lpNMHDR->code == FPCN_LOADEXTERNALRESOURCE) {
                //
            }
        }
    }

    return QDialog::nativeEvent(eventType, message, result);
}

void SFMFlashPlayerDialog::closeEvent(QCloseEvent *e)
{
    QDialog::closeEvent(e);
    deleteFlashPlayerControl();
}

void SFMFlashPlayerDialog::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
    if (m_hwndFlashPlayer) {
        ::MoveWindow(m_hwndFlashPlayer, 0, 0, width(), height(), FALSE);
    }
}
