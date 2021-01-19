#include "sfmgraphicsdialog.h"
#include <Windows.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SFMGraphicsDialog w;
    w.show();
    return a.exec();
}
