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

// Message type
#define MSG_TYPE_DOWNLOAD_FILE_FINISHED             (WM_USER + 0x01)
#define MSG_TYPE_DOWNLOAD_FILE_PROGRESS             (WM_USER + 0x02)
#define MSG_TYPE_DOWNLOAD_FILE_BYTES_RECEIVED       (WM_USER + 0x03)
#define MSG_TYPE_HTTP_POST_FINISHED                 (WM_USER + 0x04)

// Error Code
#define ERROR_CODE_OK 0


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
    std::wstring downloadUrl;
    std::wstring localPath;
};

//
class SFMUpdaterDownloadResult
{
public:
    SFMUpdaterDownloadResult() : resCode(0)
    {}
    int resCode;
    std::string resMsg;
    std::string downloadUrl;
};

//
class SFMUpdaterHttpPostParam {
public:
    SFMUpdaterHttpPostParam() : hWnd(NULL), port(80)
    {}
    HWND hWnd;
    std::wstring host;
    int port;
    std::wstring location;
    std::wstring formData;
};

//
class SFMUpdaterHttpPostResult {
public:
    SFMUpdaterHttpPostResult() : resCode(0)
    {}
    int resCode;
    std::string resMsg;
    std::wstring url;
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

    static std::string wideCharToMultiByte(unsigned int codePage, const wchar_t *wideCharBuffer);
    static std::wstring multiByteToWideChar(unsigned int codePage, const char *multiByteBuffer);

    static void downloadFile(HWND hWNd, const std::wstring &downloadUrl, const std::wstring &localPath);
    static DWORD WINAPI downloadFileThreadProc(LPVOID lpThreadParameter);

    static void httpPost(HWND hWnd, const std::wstring &host, int port, const std::wstring &location, const std::wstring &formData);
    static DWORD WINAPI httpPostThreadProc(LPVOID lpThreadParameter);


    void updateSoft();

private:
    std::string m_logDirPath;
    std::string m_downloadDirPath;
    std::string m_appExecuteablePath;
};

#endif // SFMUPDATER_H
