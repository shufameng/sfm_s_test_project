#ifndef SFMFILEDOWNLOADER_H
#define SFMFILEDOWNLOADER_H

#ifdef WIN32
#include <Windows.h>
#pragma comment(lib, "User32.lib")
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#endif

#include <tchar.h>
#include <string>
#include <list>

class SFMFileDownloader
{
public:

    //
    enum DownloadMessageType {
        DownloadFinishedType = WM_USER + 100,
        DownloadProgressType
    };

    //
    enum DownloadResultCode {
        OK = 0,
        NotOK
    };

    //
    class DownloadTaskInfo
    {
    public:
        DownloadTaskInfo()
        {
            memset(downloadFromUrl, '\0', sizeof(downloadFromUrl));
        }
        TCHAR downloadFromUrl[MAX_PATH];
        TCHAR saveToLocalPath[MAX_PATH];
    };

    //
    class DownloadParam
    {
    public:
        DownloadParam() : hWnd(nullptr), downloader(nullptr) {}
        std::list<DownloadTaskInfo> downloadTaskList;
        HWND hWnd;
        SFMFileDownloader *downloader;
    };

    //
    class DownloadResult
    {
    public:
        DownloadResult() : code(0) {}
        int code;
        std::string msg;
    };

    //
    class DownloadProgress
    {
    public:
        DownloadProgress() : finished(0), total(0) {}
        unsigned long long finished;
        unsigned long long total;
    };

    SFMFileDownloader();
    ~SFMFileDownloader();

    void add(const DownloadTaskInfo &download);
    void clean();
    void startDownload();

protected:
    static DWORD WINAPI downloadThreadProc(LPVOID lpThreadParameter);
    void postDownloadResultMessage();
    void postDownloadProgressMessage();

private:
    DownloadParam *m_param;
    DownloadResult *m_result;
    DownloadProgress *m_progress;
};

#endif // SFMFILEDOWNLOADER_H
