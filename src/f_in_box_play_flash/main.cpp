#include "finboxdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FInBoxDialog w;
    w.show();
    return a.exec();
}
