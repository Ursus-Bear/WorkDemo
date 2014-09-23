#include <stdio.h>  
#include <windows.h>  
#include <wininet.h>  
#include <process.h>  
#include <stdlib.h>
#pragma comment(lib, "WININET.lib")  

using namespace std;

int WinFtp(char *pszSourceFile, char *pszDestinationFile)
{
    if (pszDestinationFile == NULL || pszSourceFile == NULL)
    {
        printf("Input NULL error!\n");
        return -1;
    }
    BOOL bSuccess;
    HINTERNET hIntSession;
    HINTERNET hFtpSession;

    char szServer[] = "ftp.huatek.com";
    char szUser[] = "adc";
    char szPwd[] = "HTftpadc";

    char szCurDir[MAX_PATH];
    DWORD dwCurDir = MAX_PATH;

    printf("try to open intSession...\n");
    hIntSession = InternetOpen("FTP", PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
    if (hIntSession == NULL)
    {
        printf("can not open intSession!\n");
        return -1;
    }
    printf("intSession is open!\n");
    DWORD dw = 0;
    DWORD ErrorNo = 0;
    char szError[1024] = "";
    DWORD nSize = sizeof(szError);

    printf("try to open ftpSession...\n");
    hFtpSession = InternetConnect(hIntSession, szServer, INTERNET_DEFAULT_FTP_PORT, szUser, szPwd, INTERNET_SERVICE_FTP, 0, 0);
    if (hFtpSession == NULL)
    {
        dw = GetLastError();
        InternetGetLastResponseInfo(&ErrorNo, szError, &nSize);
        InternetCloseHandle(hIntSession);
        printf("can not open ftpSession!dw=[%d], ErrorNo=[%d],szError=[%s]\n", dw, ErrorNo, szError);
        InternetCloseHandle(hFtpSession);
        InternetCloseHandle(hIntSession);
        return -1;
    }
    printf("ftpSession is open!\n");

    ZeroMemory(szCurDir, sizeof(szCurDir));
    FtpGetCurrentDirectory(hFtpSession, szCurDir, &dwCurDir);
    printf("current dir is %s .\n", szCurDir);

    printf("try to set current directory...\n");
    bSuccess = FtpSetCurrentDirectory(hFtpSession, "/ADC/");
    if (!bSuccess)
    {
        dw = GetLastError();
        InternetGetLastResponseInfo(&ErrorNo, szError, &nSize);
        InternetCloseHandle(hIntSession);
        printf("can not set directory!!dw=[%d], ErrorNo=[%d],szError=[%s]\n", dw, ErrorNo, szError);
        InternetCloseHandle(hFtpSession);
        InternetCloseHandle(hIntSession);
        return -1;
    }
    printf("set directory ok!\n");

    printf("try to get file...\n");
    bSuccess = FtpPutFile(hFtpSession, pszSourceFile, pszDestinationFile, FTP_TRANSFER_TYPE_BINARY, 0);
    if (!bSuccess)
    {
        dw = GetLastError();
        InternetGetLastResponseInfo(&ErrorNo, szError, &nSize);
        InternetCloseHandle(hIntSession);
        printf("can not Ftp the File!!dw=[%d], ErrorNo=[%d],szError=[%s]\n", dw, ErrorNo, szError);
        InternetCloseHandle(hFtpSession);
        InternetCloseHandle(hIntSession);
        return -1;
    }

    InternetCloseHandle(hFtpSession);
    InternetCloseHandle(hIntSession);
}

int main()
{
    WinFtp("D:\\msdia80.dll", "msdia80.dll2");
    system("pause");
    return 0;
}