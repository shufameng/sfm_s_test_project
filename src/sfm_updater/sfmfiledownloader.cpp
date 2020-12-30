#include "sfmfiledownloader.h"

SFMFileDownloader::SFMFileDownloader()
{
    m_downloadParam = new DownloadParam;
    m_downloadResult = new DownloadResult;
}

SFMFileDownloader::~SFMFileDownloader()
{

}

void SFMFileDownloader::add(const SFMFileDownloader::DownloadInfo &download)
{
    m_downloadList.push_back(download);
}

void SFMFileDownloader::clean()
{
    m_downloadList.clear();
}

void SFMFileDownloader::startDownload()
{
    DWORD dwThreadId = 0;

    HANDLE hThreadHandle = CreateThread(NULL, 0, downloadThreadProc, param, 0, &dwThreadId);
}

DWORD SFMFileDownloader::downloadThreadProc(LPVOID lpThreadParameter)
{
    return (DWORD)SFMFileDownloader::OK;
}
