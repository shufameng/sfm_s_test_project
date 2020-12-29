#ifndef SFMFLASHPLAYERDIALOG_H
#define SFMFLASHPLAYERDIALOG_H

#include <QDialog>
#include <Windows.h>
#include <tchar.h>
#include "f_in_box.h"

//
// For help about f_in_box, see https://www.f-in-box.com/dll/
//
class SFMFlashPlayerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SFMFlashPlayerDialog(QWidget *parent = Q_NULLPTR);
    ~SFMFlashPlayerDialog();

    void createFlashPlayerControl();
    void deleteFlashPlayerControl();
    bool loadOcx(const QString &ocxPath);
    void play(const QString &flashPath);

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    void closeEvent(QCloseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    HFPC m_hFPC;
    HWND m_hwndFlashPlayer;
    QByteArray m_ocxByteArray;
    QString m_ocxPath;
};

#endif // SFMFLASHPLAYERDIALOG_H
