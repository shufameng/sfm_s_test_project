#ifndef INHERITSFMFIXEDDIALOG_H
#define INHERITSFMFIXEDDIALOG_H

#include "sfmfixeddialog.h"

namespace Ui {
class InheritSFMFixedDialog;
}

class InheritSFMFixedDialog : public SFMFixedDialog
{
    Q_OBJECT

public:
    explicit InheritSFMFixedDialog(QWidget *parent = nullptr);
    ~InheritSFMFixedDialog();

private slots:


    void on_toolButton_closeWindow_clicked();

private:
    Ui::InheritSFMFixedDialog *ui;
};

#endif // INHERITSFMFIXEDDIALOG_H
