#ifndef SFMGRAPHICSDIALOG_H
#define SFMGRAPHICSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SFMGraphicsDialog; }
QT_END_NAMESPACE

class SFMGraphicsDialog : public QDialog
{
    Q_OBJECT

public:
    SFMGraphicsDialog(QWidget *parent = nullptr);
    ~SFMGraphicsDialog();

private:
    Ui::SFMGraphicsDialog *ui;
};
#endif // SFMGRAPHICSDIALOG_H
