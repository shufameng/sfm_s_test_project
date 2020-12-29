#include "sfmupdatertestdialog.h"

#include <QApplication>
#include "sfmupdater.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SfmUpdaterTestDialog w;
    w.show();


    SFMUpdater::test_getFileInfoFromDir();





    return a.exec();
}
