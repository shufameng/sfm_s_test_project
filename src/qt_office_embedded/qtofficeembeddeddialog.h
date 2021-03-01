#ifndef QTOFFICEEMBEDDEDDIALOG_H
#define QTOFFICEEMBEDDEDDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class QtOfficeEmbeddedDialog; }
QT_END_NAMESPACE

class QtOfficeEmbeddedDialog : public QDialog
{
    Q_OBJECT

public:
    QtOfficeEmbeddedDialog(QWidget *parent = nullptr);
    ~QtOfficeEmbeddedDialog();

    static bool isDsoframerRegistered();
    static bool isCurrentUserAnAdmin();
    static bool registerDsoframer();

private slots:
    void on_pushButton_browse_clicked();

    void openFile(const QString &filePath);

private:
    Ui::QtOfficeEmbeddedDialog *ui;

    static QString sg_dsoframerId;
};
#endif // QTOFFICEEMBEDDEDDIALOG_H
