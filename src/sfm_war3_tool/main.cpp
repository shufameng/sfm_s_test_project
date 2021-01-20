#include "sfmwar3toolwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SfmWar3ToolWindow w;
    w.show();
    return a.exec();
}
