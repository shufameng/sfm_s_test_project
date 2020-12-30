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
    enum DownloadResultCode {
        OK = 0,
        NotOK
    };

    //
    class DownloadParam
    {
    public:
        DownloadParam() : hWnd(nullptr) {}
        HWND hWnd;
        std::string downloadFromUrl;
        std::string saveToLocalPath;
    };

    //
    class DownloadInfo
    {
    public:
        DownloadInfo() {}
        std::string downloadFromUrl;
        std::string saveToLocalPath;
    };

    //
    class DownloadResult
    {
    public:
        DownloadResult() : code(0) {}
        int code;
        std::string msg;
    };

    SFMFileDownloader();
    ~SFMFileDownloader();

    void add(const DownloadInfo &download);
    void clean();
    void startDownload();

protected:
    static DWORD WINAPI downloadThreadProc(LPVOID lpThreadParameter);

private:
    std::list<DownloadInfo> m_downloadList;
    DownloadParam *m_downloadParam;
    DownloadResult *m_downloadResult;
};

#endif // SFMFILEDOWNLOADER_H
