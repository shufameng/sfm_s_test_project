#include "pathfindingdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(time(NULL));

    PathFindingDialog w;
    w.show();
    return a.exec();
}
