#include "sfmfiledownloader.h"

SFMFileDownloader::SFMFileDownloader()
{
    m_param = new DownloadParam;
    m_result = new DownloadResult;
    m_progress = new DownloadProgress;
}

SFMFileDownloader::~SFMFileDownloader()
{
    delete m_param;
    delete m_result;
}

void SFMFileDownloader::add(const SFMFileDownloader::DownloadTaskInfo &download)
{
    m_param->downloadTaskList.push_back(download);
}

void SFMFileDownloader::clean()
{
    m_param->downloadTaskList.clear();
}

void SFMFileDownloader::startDownload()
{
    DWORD dwThreadId = 0;
    HANDLE hThreadHandle = CreateThread(nullptr, 0, downloadThreadProc, m_param, 0, &dwThreadId);
    if (!hThreadHandle) {
        m_result->msg = "CreateThread() returns null";
        postDownloadResultMessage();
    }
}

DWORD SFMFileDownloader::downloadThreadProc(LPVOID lpThreadParameter)
{
    SFMFileDownloader *d = (SFMFileDownloader*)lpThreadParameter;
    int bufferLen = 1024*1024;
    std::unique_ptr<char[]> bufferPtr(new char[bufferLen]);
    DWORD readLen = 0;
    size_t writeLen = 0;
    FILE *f = nullptr;

    for (std::list<DownloadTaskInfo>::const_iterator iter = d->m_param->downloadTaskList.cbegin();
         iter != d->m_param->downloadTaskList.cend(); ++ iter) {

        HINTERNET hInternet = InternetOpen(_T("User agent"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) {
            d->m_result->msg = "InternetOpen() returns null";
            d->postDownloadResultMessage();
            return -1;
        }

        HINTERNET hUrlFile = InternetOpenUrl(hInternet, (*iter).downloadFromUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hInternet) {
            d->m_result->msg = "InternetOpenUrl() returns null";
            d->postDownloadResultMessage();
            return -1;
        }

        f = _tfopen((*iter).saveToLocalPath, _T("wb"));
        if (!f) {
            d->m_result->msg = "_tfopen() returns null";
            d->postDownloadResultMessage();
            return -1;
        }

        while (true) {
            if (!InternetReadFile(hUrlFile, bufferPtr.get(), bufferLen, &readLen)) {
                d->m_result->msg = "InternetReadFile() returns false";
                d->postDownloadResultMessage();
                return -1;
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
    }

    fflush(f);
    fclose(f);

    d->m_result->code = 0;
    d->m_result->msg = "OK";
    d->postDownloadResultMessage();

    return 0;
}

void SFMFileDownloader::postDownloadResultMessage()
{
    if (m_param->hWnd) {
        PostMessage(m_param->hWnd, DownloadFinishedType, 0, (LPARAM)this);
    }
}

void SFMFileDownloader::postDownloadProgressMessage()
{
    if (m_param->hWnd) {
        PostMessage(m_param->hWnd, DownloadProgressType, 0, (LPARAM)this);
    }
}
