#include "sfmupdater.h"
#include <tchar.h>
#include <QtDebug>

#ifdef WIN32
#include <Windows.h>
#pragma comment(lib, "User32.lib")
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#endif

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

void SFMUpdater::download(const std::list<SFMUpdaterDownloadInfo> &downloadList, sfmupdater_download_cb onErrorCb, void *onErrorCbParam, sfmupdater_download_cb onFinishedCb, void *onFinishedCbParam)
{
    HINTERNET hInternet = InternetOpen(_T("User agent"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        if (onErrorCb) {
            onErrorCb(onErrorCbParam);
            return;
        }
    }

    for (std::list<SFMUpdaterDownloadInfo>::const_iterator iter = downloadList.cbegin(); iter != downloadList.cend(); ++ iter) {
        HINTERNET hUrlFile = InternetOpenUrl(hInternet, (*iter).downloadFromUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    }






}
#endif
