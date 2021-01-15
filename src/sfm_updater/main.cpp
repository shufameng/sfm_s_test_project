#include "sfmupdatertestdialog.h"

#include <QApplication>
#include "sfmupdater.h"
#include <QDebug>
#include "tinyxml.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SfmUpdaterTestDialog w;
    w.show();


    //SFMUpdater::test_getFileInfoFromDir();


    std::unique_ptr<char[]> buffer_ptr(new char[100]);
    memset(buffer_ptr.get(), '\0', 100);
    const char *str = "Good day sir";
    memcpy(buffer_ptr.get(), str, strlen(str));

    qDebug() << buffer_ptr.get();

    std::unique_ptr<QString> qstring_ptr(new QString);

    TiXmlDocument doc;


    return a.exec();
}
