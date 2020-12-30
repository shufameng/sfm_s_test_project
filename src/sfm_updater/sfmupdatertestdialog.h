#ifndef SFMUPDATERTESTDIALOG_H
#define SFMUPDATERTESTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SfmUpdaterTestDialog; }
QT_END_NAMESPACE

class SfmUpdaterTestDialog : public QDialog
{
    Q_OBJECT

public:
    SfmUpdaterTestDialog(QWidget *parent = nullptr);
    ~SfmUpdaterTestDialog();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    Ui::SfmUpdaterTestDialog *ui;
};
#endif // SFMUPDATERTESTDIALOG_H
