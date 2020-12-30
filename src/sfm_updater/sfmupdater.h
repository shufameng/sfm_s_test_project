#ifndef SFMUPDATER_H
#define SFMUPDATER_H

#include <string>
#include <list>
#include <tchar.h>
#include <QDebug>
#ifdef WIN32
#include <Windows.h>
#pragma comment(lib, "User32.lib")
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#endif


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

//
typedef void(*sfmupdater_download_cb)(void *param);
class DownloadFuncParameter {
public:
    DownloadFuncParameter() :
        onErrorCb(nullptr),
        onErrorCbParam(nullptr),
        onFinishedCb(nullptr),
        onFinishedCbParam(nullptr)
    {}
    sfmupdater_download_cb onErrorCb;
    void *onErrorCbParam;
    sfmupdater_download_cb onFinishedCb;
    void *onFinishedCbParam;
    std::list<SFMUpdaterDownloadInfo> downloadList;
};

//
class DownloadNotifyMessage
{
public:
    DownloadNotifyMessage() : error(0), errorDesc(nullptr)
    {

    }

    int error;
    const char *errorDesc;
};

//
class SFMUpdaterDownloadParam
{
public:
    SFMUpdaterDownloadParam() : hWnd(nullptr)
    {}

    HWND hWnd;
    LPCTSTR downloadFromUrl;
    LPCTSTR saveToLocalPath;
};

//
class SFMUpdaterDownloadResult
{
public:
    SFMUpdaterDownloadResult() : error(0), errorDesc(nullptr)
    {}

    int error;
    const TCHAR *errorDesc;
};

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

    static void download(LPCTSTR url, LPCTSTR savePath, HWND hWnd);

};

#endif // SFMUPDATER_H
