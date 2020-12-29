#ifndef FINBOXDIALOG_H
#define FINBOXDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class FInBoxDialog; }
QT_END_NAMESPACE

class FInBoxDialog : public QDialog
{
    Q_OBJECT

public:
    FInBoxDialog(QWidget *parent = nullptr);
    ~FInBoxDialog();

private:
    Ui::FInBoxDialog *ui;
};
#endif // FINBOXDIALOG_H
