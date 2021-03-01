#include "qtofficeembeddeddialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtOfficeEmbeddedDialog w;
    w.show();
    return a.exec();
}
