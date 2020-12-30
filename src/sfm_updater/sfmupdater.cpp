#include "sfmupdater.h"
#include <tchar.h>
#include <QtDebug>

#define MSG_TYPE_DOWNLOAD_PROGRESS (WM_USER + 0x1)
#define MSG_TYPE_DOWNLOAD_FINISHED (WM_USER + 0x2)



SFMUpdater::SFMUpdater()
{

}

#ifdef WIN32
bool SFMUpdater::getFileInfoFromDir(const std::wstring &dirPath, std::list<SFMUpdaterFileInfo> &listOut)
{
    WIN32_FIND_DATA find_data;
    HANDLE hFile;
    std::wstring path = dirPath;
    path += _T("\\*");

    hFile = FindFirstFile(path.c_str(), &find_data);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    while (true) {
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (lstrcmp(find_data.cFileName, _T(".")) != 0 && lstrcmp(find_data.cFileName, _T("..")) != 0) {
                if (!getFileInfoFromDir(dirPath + _T("\\") + find_data.cFileName, listOut)) {
                    return false;
                }
            }
        } else {
            SFMUpdaterFileInfo info;
            info.name = find_data.cFileName;
            info.absoluteDir = dirPath;
            listOut.push_back(info);
        }

        if (!FindNextFile(hFile, &find_data)) {
            break;
        }
    }

    return true;
}

DWORD WINAPI download_thread_proc(LPVOID lpThreadParameter)
{
    DownloadFuncParameter *param = static_cast<DownloadFuncParameter*>(lpThreadParameter);
    HINTERNET hInternet = InternetOpen(_T("User agent"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        if (param->onErrorCb) {
            param->onErrorCb(param->onErrorCbParam);
            return -1;
        }
    }

    for (std::list<SFMUpdaterDownloadInfo>::const_iterator iter = param->downloadList.cbegin(); iter != param->downloadList.cend(); ++ iter) {
        HINTERNET hUrlFile = InternetOpenUrl(hInternet, (*iter).downloadFromUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hUrlFile) {
            if (param->onErrorCb) {
                param->onErrorCb(param->onErrorCbParam);
                return -1;
            }
        }

        FILE *f = nullptr;
        errno_t code = _wfopen_s(&f, (*iter).saveToLocalPath.c_str(), _T("wb"));
        if (0 != code) {
            if (param->onErrorCb) {
                param->onErrorCb(param->onErrorCbParam);
                return -1;
            }
        }

        std::unique_ptr<char[]> buf_ptr(new char[1024*1024]);
        DWORD read_len = 0;
        size_t write_len = 0;
        while (true) {
            if (!InternetReadFile(hUrlFile, buf_ptr.get(), 1024*1024, &read_len)) {
                if (param->onErrorCb) {
                    param->onErrorCb(param->onErrorCbParam);
                    return -1;
                }
            }
            if (read_len > 0) {
                write_len = fwrite(buf_ptr.get(), read_len, 1, f);
                if (write_len <= 0) {
                    break;
                }
            } else {
                break;
            }
        }

        fflush(f);
        fclose(f);
    }

    return 0;
}

void SFMUpdater::download(const std::list<SFMUpdaterDownloadInfo> &downloadList, sfmupdater_download_cb onErrorCb, void *onErrorCbParam, sfmupdater_download_cb onFinishedCb, void *onFinishedCbParam)
{
    DWORD dwThreadId = 0;
    DownloadFuncParameter *param = new DownloadFuncParameter;
    param->onErrorCb = onErrorCb;
    param->onErrorCbParam = onErrorCbParam;
    param->onFinishedCb = onFinishedCb;
    param->onFinishedCbParam = onFinishedCbParam;
    param->downloadList = downloadList;
    HANDLE hThreadHandle = CreateThread(NULL, 0, download_thread_proc, (void*)param, 0, &dwThreadId);
    if (!hThreadHandle) {
        if (onErrorCb) {
            onErrorCb(onErrorCbParam);
        }
    }
}

DWORD WINAPI download_thread_proc_1(LPVOID lpThreadParameter)
{
    SFMUpdaterDownloadParam *param = static_cast<SFMUpdaterDownloadParam*>(lpThreadParameter);
    SFMUpdaterDownloadResult *rst = new SFMUpdaterDownloadResult;

    HINTERNET hInternet = InternetOpen(_T("User agent"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        if (param->hWnd) {
            rst->error = 2;
            rst->errorDesc = _T("InternetOpen() returns null");
            PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FINISHED, NULL, (LPARAM)rst);
            return 2;
        }
    }

    HINTERNET hUrlFile = InternetOpenUrl(hInternet, param->downloadFromUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrlFile) {
        if (param->hWnd) {
            rst->error = 3;
            rst->errorDesc = _T("InternetOpenUrl() returns null");
            PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FINISHED, NULL, (LPARAM)rst);
            return 3;
        }
    }

    FILE *f = _tfopen(param->saveToLocalPath, _T("wb"));
    if (!f) {
        rst->error = 5;
        rst->errorDesc = _T("_tfopen() returns null");
        return 5;
    }

    int bufferLen = 1024*1024;
    std::unique_ptr<char[]> bufferPtr(new char[bufferLen]);
    DWORD readLen = 0;
    size_t writeLen = 0;
    while (true) {
        if (!InternetReadFile(hUrlFile, bufferPtr.get(), bufferLen, &readLen)) {
            rst->error = 4;
            rst->errorDesc = _T("InternetReadFile() returns false");
            PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FINISHED, NULL, (LPARAM)rst);
            fclose(f);
            return 4;
        }
        if (readLen > 0) {
            writeLen = fwrite(bufferPtr.get(), readLen, 1, f);
            if (writeLen <= 0) {
                break;
            }
        } else {
            break;
        }
    }

    rst->error = 0;
    rst->errorDesc = _T("No Error");
    PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FINISHED, NULL, (LPARAM)rst);

    fclose(f);
    return 0;
}

void SFMUpdater::download(LPCTSTR url, LPCTSTR savePath, HWND hWnd)
{
    DWORD dwThreadId = 0;
    SFMUpdaterDownloadParam *param = new SFMUpdaterDownloadParam;
    param->hWnd = hWnd;
    param->downloadFromUrl = url;
    param->saveToLocalPath = savePath;
    HANDLE hThreadHandle = CreateThread(NULL, 0, download_thread_proc_1, (void*)param, 0, &dwThreadId);
    if (!hThreadHandle) {
        if (hWnd) {
            SFMUpdaterDownloadResult *rst = new SFMUpdaterDownloadResult;
            rst->error = 1;
            rst->errorDesc = _T("CreateThread() returns null");
            PostMessage(hWnd, MSG_TYPE_DOWNLOAD_FINISHED, NULL, (LPARAM)rst);
        }
    }
}

#endif
