#ifndef SFMUPDATER_H
#define SFMUPDATER_H

#include <string>
#include <list>
#include <tchar.h>
#include <QDebug>


//
class SFMUpdaterAppInfo
{
public:
    SFMUpdaterAppInfo() {}

    std::string name;
    std::string versionString;
    std::string releaseTime;
    std::string releaseLog;
};

//
class SFMUpdaterFileInfo
{
public:
    SFMUpdaterFileInfo() : size(0) {}

    std::wstring name;
    unsigned long long size;
    std::string checkSum;
    std::string checkSumMethod;
    std::wstring absoluteDir;
    std::wstring relativeDir;
};

//
class SFMUpdaterDownloadInfo
{
public:
    SFMUpdaterDownloadInfo() {}

    std::wstring downloadFromUrl;
    std::wstring saveToLocalPath;
};
typedef void(*sfmupdater_download_cb)(void *param);

//
class SFMUpdater
{
public:
    SFMUpdater();

#ifdef WIN32
    static bool getFileInfoFromDir(const std::wstring &dirPath, std::list<SFMUpdaterFileInfo> &listOut);
#endif

    static void test_getFileInfoFromDir() {
        std::list<SFMUpdaterFileInfo> list;
        getFileInfoFromDir(_T("F:\\download\\zhibo"), list);

        for (std::list<SFMUpdaterFileInfo>::const_iterator iter = list.cbegin(); iter != list.cend(); ++ iter) {
            qDebug() << QString::fromStdWString((*iter).absoluteDir) << QString::fromStdWString((*iter).name);
        }
    }


    static void download(const std::list<SFMUpdaterDownloadInfo> &downloadList,
                         sfmupdater_download_cb onErrorCb,
                         void *onErrorCbParam,
                         sfmupdater_download_cb onFinishedCb,
                         void *onFinishedCbParam);

};

#endif // SFMUPDATER_H
