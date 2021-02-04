#ifndef SFMCUSTOMWIDGETSDIALOG_H
#define SFMCUSTOMWIDGETSDIALOG_H

#include <QDialog>
#include <QApplication>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class SFMCustomWidgetsDialog; }
QT_END_NAMESPACE

class MyApp : public QApplication {
    Q_OBJECT
public:
    MyApp(int &argc, char **argv) : QApplication(argc, argv) {

    }

protected:
    bool event(QEvent *e) override {
        qDebug() << "MyApp::event(), eventtype=" << e->type();
        return QApplication::event(e);
    }
};

class SFMCustomWidgetsDialog : public QDialog
{
    Q_OBJECT

public:
    SFMCustomWidgetsDialog(QWidget *parent = nullptr);
    ~SFMCustomWidgetsDialog();

private slots:
    void on_pushButton_sfmfixeddialog_clicked();

protected:
    bool event(QEvent *e) override;

private:
    Ui::SFMCustomWidgetsDialog *ui;
};
#endif // SFMCUSTOMWIDGETSDIALOG_H
