#include "sfmupdater.h"
#include <tchar.h>
#include <QtDebug>



SFMUpdater::SFMUpdater()
{

}

std::string SFMUpdater::wideCharToMultiByte(unsigned int codePage, const wchar_t *wideCharBuffer)
{
   int len = WideCharToMultiByte(codePage, 0, wideCharBuffer, -1, NULL, 0, NULL, NULL);
   char *multiByteBuffer = new char[len];
   WideCharToMultiByte(codePage, 0, wideCharBuffer, -1, multiByteBuffer, len, NULL, NULL);
   std::string ret(multiByteBuffer);
   delete []multiByteBuffer;
   return ret;
}

std::wstring SFMUpdater::multiByteToWideChar(unsigned int codePage, const char *multiByteBuffer)
{
    int len = MultiByteToWideChar(codePage, 0, multiByteBuffer, -1, NULL, 0);
    wchar_t *wideCharBuffer = new wchar_t[len];
    MultiByteToWideChar(codePage, 0, multiByteBuffer, -1, wideCharBuffer, len);
    std::wstring ret(wideCharBuffer);
    delete []wideCharBuffer;
    return ret;
}

void SFMUpdater::updateSoft()
{
    // 获取最新文件状态信息
    // 获取本地文件状态信息
    // 比对最新和本地文件状态信息,获取到需要更新的文件
    // 下载需要更新的文件
    // 安装需要更新的文件
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

void SFMUpdater::downloadFile(HWND hWnd, const std::wstring &downloadUrl, const std::wstring &localPath)
{
    DWORD dwThreadId = 0;
    SFMUpdaterDownloadParam *param = new SFMUpdaterDownloadParam;
    param->hWnd = hWnd;
    param->downloadUrl = downloadUrl;
    param->localPath = localPath;
    HANDLE hThreadHandle = CreateThread(NULL, 0, downloadFileThreadProc, param, 0, &dwThreadId);
    if (!hThreadHandle) {
        SFMUpdaterDownloadResult *res = new SFMUpdaterDownloadResult;
        res->resCode = 1;
        res->downloadUrl;
        res->resMsg = "CreateThread() returns null";
        PostMessage(hWnd, MSG_TYPE_DOWNLOAD_FILE_FINISHED, NULL, (LPARAM)res);
        delete param;
        return;
    }
}

DWORD SFMUpdater::downloadFileThreadProc(LPVOID lpThreadParameter)
{
    SFMUpdaterDownloadParam *param = (SFMUpdaterDownloadParam*)(lpThreadParameter);
    int bufferLen = 1024 * 1024;
    std::unique_ptr<char[]> bufferPtr(new char[bufferLen]);
    DWORD readLen = 0;
    size_t writeLen = 0;
    FILE *fp = nullptr;

    HINTERNET hInternet = InternetOpen(_T("SFMUpdater"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        SFMUpdaterDownloadResult *res = new SFMUpdaterDownloadResult;
        res->resCode = 1;
        res->resMsg = "InternetOpen() returns null";
        PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FILE_FINISHED, NULL, (LPARAM)res);
        delete param;
        return 1;
    }

    HINTERNET hUrlFile = InternetOpenUrl(hInternet, param->downloadUrl.c_str(), nullptr, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrlFile) {
        SFMUpdaterDownloadResult *res = new SFMUpdaterDownloadResult;
        res->resCode = 2;
        res->resMsg = "InternetOpenUrl() returns null";
        PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FILE_FINISHED, NULL, (LPARAM)res);
        delete param;
        return 2;
    }

    if (0 != _wfopen_s(&fp, param->localPath.c_str(), L"wb")) {
        SFMUpdaterDownloadResult *res = new SFMUpdaterDownloadResult;
        res->resCode = 3;
        res->resMsg = "_wfopen_s() returns none zero";
        PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FILE_FINISHED, NULL, (LPARAM)res);
        delete param;
        return 3;
    }

    while (true) {
        if (!InternetReadFile(hUrlFile, bufferPtr.get(), bufferLen, &readLen)) {
            SFMUpdaterDownloadResult *res = new SFMUpdaterDownloadResult;
            res->resCode = 4;
            res->resMsg = "InternetReadFile() returns false";
            PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FILE_FINISHED, NULL, (LPARAM)res);
            fclose(fp);
            delete param;
            return 4;
        }
        if (readLen > 0) {
            writeLen = fwrite(bufferPtr.get(), readLen, 1, fp);
            if (writeLen <= 0) {
                break;
            }
        }
        else {
            break;
        }
    }

    fflush(fp);
    fclose(fp);

    SFMUpdaterDownloadResult *res = new SFMUpdaterDownloadResult;
    res->resCode = 0;
    res->resMsg = "OK";
    PostMessage(param->hWnd, MSG_TYPE_DOWNLOAD_FILE_FINISHED, NULL, (LPARAM)res);
    delete param;

    return 0;
}


void SFMUpdater::httpPost(HWND hWnd, const std::wstring &host, int port, const std::wstring &location, const std::wstring &formData)
{
    DWORD dwThreadId = 0;
    SFMUpdaterHttpPostParam *param = new SFMUpdaterHttpPostParam;
    param->hWnd = hWnd;
    param->host = host;
    param->port = port;
    param->location = location;
    param->formData = formData;
    HANDLE hThreadHandle = CreateThread(NULL, 0, httpPostThreadProc, (LPVOID)param, 0, &dwThreadId);
    if (!hThreadHandle) {
        SFMUpdaterHttpPostResult *res = new SFMUpdaterHttpPostResult;
        res->resCode = ERROR_CODE_OK;
        res->resMsg = "CreateThread() returns null";
        PostMessage(hWnd, MSG_TYPE_HTTP_POST_FINISHED, NULL, (LPARAM)res);
        delete param;
        return;
    }
}

DWORD SFMUpdater::httpPostThreadProc(LPVOID lpThreadParameter)
{
    SFMUpdaterHttpPostParam *param = (SFMUpdaterHttpPostParam*)lpThreadParameter;

    HINTERNET hInternet = InternetOpen(_T("SFMUpdater"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        SFMUpdaterHttpPostResult *res = new SFMUpdaterHttpPostResult;
        res->resCode = 1;
        res->resMsg = "InternetOpen() returns null";
        PostMessage(param->hWnd, MSG_TYPE_HTTP_POST_FINISHED, NULL, (LPARAM)res);
        delete param;
        return 1;
    }

    HINTERNET hConnect = InternetConnect(hInternet, param->host.c_str(), param->port, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
    if (!hConnect) {
        SFMUpdaterHttpPostResult *result = new SFMUpdaterHttpPostResult;
        result->resCode = 2;
        result->resMsg = "InternetConnect() returns null";
        PostMessage(param->hWnd, MSG_TYPE_HTTP_POST_FINISHED, NULL, (LPARAM)result);
        delete param;
        return 2;
    }
    HINTERNET hRequest = HttpOpenRequest(hConnect, _T("POST"), param->location.c_str(), HTTP_VERSION, NULL, NULL, INTERNET_FLAG_DONT_CACHE, NULL);
    if (!hRequest) {
        SFMUpdaterHttpPostResult *result = new SFMUpdaterHttpPostResult;
        result->resCode = 3;
        result->resMsg = "HttpOpenRequest() returns null";
        PostMessage(param->hWnd, MSG_TYPE_HTTP_POST_FINISHED, NULL, (LPARAM)result);
        delete param;
        return 3;
    }
    std::string multiByteData = wideCharToMultiByte(CP_UTF8, param->formData.c_str());
    if (!HttpSendRequest(hRequest, NULL, 0, (void*)multiByteData.c_str(), strlen(multiByteData.c_str()))) {
        SFMUpdaterHttpPostResult *result = new SFMUpdaterHttpPostResult;
        result->resCode = 4;
        result->resMsg = "HttpSendRequest() returns false";
        PostMessage(param->hWnd, MSG_TYPE_HTTP_POST_FINISHED, NULL, (LPARAM)result);
        delete param;
        return 4;
    }
    DWORD readLen = 0;
    int bufferLen = 1024 * 1024 * 2;
    std::shared_ptr<char[]> bufferPtr(new char[bufferLen]);
    memset(bufferPtr.get(), '\0', bufferLen);
    while (true) {
        if (!InternetReadFile(hRequest, bufferPtr.get(), bufferLen, &readLen)) {
            return 5;
        }
        if (readLen <= 0) {
            break;
        }
    }

    delete param;
    SFMUpdaterHttpPostResult *result = new SFMUpdaterHttpPostResult;
    result->resCode = ERROR_CODE_OK;
    result->resMsg = "OK";
    PostMessage(param->hWnd, MSG_TYPE_HTTP_POST_FINISHED, NULL, (LPARAM)result);
    delete param;

    return 0;
}

#endif
