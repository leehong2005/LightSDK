// TestCommon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SdkCommonInclude.h"
#include <stdio.h>
#include <windows.h> 
#include <lm.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "Netapi32.lib")

class MyCryptFileSink : public ICryptFileNotify
{
public:

    char   psBuffer[128];
    FILE   *pPipe;

    MyCryptFileSink()
    {
        pPipe = _popen( "dir *.c /on /p", "rt" );
    }

    void OnCryptBegin()
    {
        printf("Begin to crypt...\n");
    }

    void OnCryptFinish()
    {
        printf("Finish to crypt...\n");
    }

    void OnCryptNext(IN DWORD dwCurIndex, IN LPCTSTR lpCurFilePath)
    {
        wprintf(L"Processing  %d, %s\n", (dwCurIndex + 1), lpCurFilePath);
    }

    void OnCryptError(IN LPCTSTR lpErrorMsg)
    {
        wprintf(L"Crypt error: %s\n", lpErrorMsg);
    }

    void OnProgressChanged(IN DWORD dwCompleted, IN DWORD dwTotal)
    {
        printf("%.1f%%\n", ((FLOAT)(dwCompleted) / (FLOAT)(dwTotal)) * 100);
    }

    void OnReplaceFiles(OUT CRYPT_INQUIRE_RESULT *pResult, IN LPCTSTR lpFileName)
    {
        wprintf(L"Replace? %s\n", lpFileName);
        int c = 0;
        cin >> c;
        if (c == 0)
        {
            *pResult = CRYPT_INQUIRE_YES;
        }
        else
        {
            *pResult = CRYPT_INQUIRE_NO;
        }
    }
};


//////////////////////////////////////////////////////////////////////////

void TestMediaContent()
{
    SdkFilePropHelper media(L"D:\\a.mp3");

    PROPERTYKEYNAME szKeys[] = 
    {
        SYSTEM_CORE_AUTHOR,                                 // String Author
        SYSTEM_CORE_COMMENT,                                // String Comment
        SYSTEM_CORE_COMPANY,                                // String Company
        SYSTEM_CORE_COPYRIGHT,                              // String Copyright
        SYSTEM_CORE_DATECREATED,                            // DateTime
        SYSTEM_CORE_DATEMODIFIED,                           // DateTime
        SYSTEM_CORE_DATEACCESSED,                           // DateTime
        SYSTEM_CORE_ITEMTYPE,                               // String
        SYSTEM_CORE_ITEMTYPETEXT,                           // String
        SYSTEM_CORE_RATING,                                 // UInt32
        SYSTEM_CORE_RATINGTEXT,                             // String
        SYSTEM_CORE_TITLE,                                  // String
        SYSTEM_CORE_TOTALFILESIZE,                          // UInt64
        SYSTEM_CORE_FILEALLOCATIONSIZE,                     // UInt64
        SYSTEM_CORE_SIZE,                                   // UInt64
        SYSTEM_CORE_DISPLAYNAME,                            // String
        SYSTEM_CORE_FILENAME,                               // String
        SYSTEM_CORE_ITEMPATHDISPLAY,                        // String
        SYSTEM_CORE_PARSINGPATH,                            // String
        SYSTEM_CORE_DESCRIPTION,                            // String
        SYSTEM_CORE_KEYWORDS,                               // String Tags
        SYSTEM_CORE_FOLDERPATH,                             // String Folder Path.
    };

    WCHAR szBuffer[MAX_PATH] = { 0 };
    for (int i = 0; i < ARRAYSIZE(szKeys); ++i)
    {
        media.FormatForDisplay(szKeys[i], szBuffer, MAX_PATH);
        ::OutputDebugString(szBuffer);
    }

    ::OutputDebugString(L"================================================ \n");

    media.SetFileName(L"D:\\blue_turkey.mp3");

    for (int i = 0; i < ARRAYSIZE(szKeys); ++i)
    {
        media.FormatForDisplay(szKeys[i], szBuffer, MAX_PATH);
        ::OutputDebugString(szBuffer);
    }
}

//////////////////////////////////////////////////////////////////////////

void TestMediaInfoProvider()
{
}

//////////////////////////////////////////////////////////////////////////

void TestCrypt()
{
    BYTE *pbData = NULL;
    DWORD len = 0;
    DWORD len2 = 0;

    {
        SdkCrypt *obj = NULL;
        CRYPT_RESULT result = CRYPT_ERROR_SUCCEED;
        SdkCryptKey key;
        if (key.LoadFromFile(L"E:\\a.key"))
        {
            obj = new SdkCrypt(&key);
            obj->InitializeScene();
        }
        else
        {
            SdkCryptKey *pKey = NULL;
            obj = new SdkCrypt();
            obj->CreateCryptKey(&pKey);
            if (NULL != pKey && CRYPT_ERROR_SUCCEED == result)
            {
                pKey->SaveToFile(L"E:\\a.key");
                SAFE_DELETE(pKey);
            }
        }

        //obj.InitializeScene();

        CHAR *pText = "l_key";
        len = sizeof(CHAR) *( strlen(pText) + 1);
        len2 = len;
        obj->GetCheckedCbSize(&len2);


        pbData = new BYTE[len2];
        ZeroMemory(pbData, len2);
        memcpy_s(pbData, len2, pText, len);

        obj->EncryptStream(pbData, len2, TRUE);

        printf("encrypt stream pbData = %s\n", (CHAR*)pbData);
        CHAR szBuf[512] = { 0 };
        SdkBase64Util::Encode((CHAR*)pbData, len, szBuf, 512);
        szBuf[0] = 0;
        SdkBase64Util::Encode((CHAR*)pText, len, szBuf, 512);

    }

    {
        SdkCryptKey key;
        if (key.LoadFromFile(L"E:\\a.key"))
        {
            SdkCrypt obj(&key);
            obj.InitializeScene();

            CHAR szBuf[512] = { 0 };
            SdkBase64Util::Encode((CHAR*)pbData, len2, szBuf, 512);

            obj.DecryptStream(pbData, len2, TRUE);

            printf("decrypt stream pbData = %s\n", (CHAR*)pbData);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void TestGetUserInfo()
{
    USERINFODATA *pUserInfo = NULL;
    SdkUserInfoUtil::GetCurLogonUserInfo(&pUserInfo);
    if (NULL != pUserInfo)
    {
        SdkUserInfoUtil::DeleteUserInfoData(pUserInfo);
    }

    DWORD dwLevel = 1;
    LPWKSTA_USER_INFO_1 pBuf = NULL;
    NET_API_STATUS nStatus;
    //
    // Call the NetWkstaUserGetInfo function;
    //  specify level 1.
    //
    nStatus = NetWkstaUserGetInfo(NULL,
        dwLevel,
        (LPBYTE *)&pBuf);
    //
    // If the call succeeds, print the information
    //  about the logged-on user.
    //
    if (nStatus == NERR_Success)
    {
        if (pBuf != NULL)
        {
            wprintf(L"\n\tUser:          %s\n", pBuf->wkui1_username);
            wprintf(L"\tDomain:        %s\n", pBuf->wkui1_logon_domain);
            wprintf(L"\tOther Domains: %s\n", pBuf->wkui1_oth_domains);
            wprintf(L"\tLogon Server:  %s\n", pBuf->wkui1_logon_server);
        }
    }
    // Otherwise, print the system error.
    //
    else
        fprintf(stderr, "A system error has occurred: %d\n", nStatus);
    //
    // Free the allocated memory.
    //
    if (pBuf != NULL)
        NetApiBufferFree(pBuf);
}

//////////////////////////////////////////////////////////////////////////

void TestCryptFileClass()
{
    printf("Enter password:\n");
    CHAR pPWD[100] = { 0 };
    cin >> pPWD;

    wstring str = SdkCommonHelper::AnsiToUnicode(pPWD);

    MyCryptFileSink *pMySink = new MyCryptFileSink();
    SdkCryptFile *pCrypt = new SdkCryptFile();
    pCrypt->SetCryptFileSink(pMySink);
    pCrypt->SetCryptKeyPath(_T("E:\\aa.key"));

    pCrypt->SetCryptPassword(str.c_str());
    vector<CryptString> vctFiles;

    BOOL isEncrypt = FALSE;
    if (isEncrypt)
    {
        vctFiles.push_back(L"E:\\Tools\\dotnetfx35.iso");
        //vctFiles.push_back(L"E:\\a.ini");
        pCrypt->SetCryptFiles(vctFiles, FALSE);
        pCrypt->BeginCrypt(CRYPT_OP_ENCRYPT);
    }
    else
    {
        vctFiles.push_back(L"E:\\Tools\\dotnetfx35.iso.tofp");
        //vctFiles.push_back(L"E:\\a.ini.tofp");
        pCrypt->SetCryptFiles(vctFiles, FALSE);
        pCrypt->BeginCrypt(CRYPT_OP_DECRYPT);
    }

    delete pMySink;
    delete pCrypt;
}

//////////////////////////////////////////////////////////////////////////

unsigned int WINAPI TestProgressDialogThreadProc(LPVOID lpParam);

void TestProgressDialog()
{
    unsigned int nThreadId = 0;
    HANDLE hThread = chBEGINTHREADEX(
        NULL,
        0,
        TestProgressDialogThreadProc,
        NULL,
        0,
        &nThreadId);

    if (NULL != hThread)
    {
        printf("Succeed to begin thread.\n");
        WaitForSingleObject(hThread, INFINITE);
        printf("The thread finishes.\n");
    }
}

//////////////////////////////////////////////////////////////////////////

unsigned int WINAPI TestProgressDialogThreadProc(LPVOID lpParam)
{
    CoInitialize(NULL);

    {
        SdkProgressDialog sdkProgressDlg;
        sdkProgressDlg.Start(TRUE);
        sdkProgressDlg.SetLines(1, _T("Test progress dialog 1"));
        sdkProgressDlg.SetLines(2, _T("Test progress dialog 2"));
        sdkProgressDlg.SetLines(3, _T("Test progress dialog 3"));
        sdkProgressDlg.SetTitle(_T("The dialog title"));
        sdkProgressDlg.SetCancelMessage(L"Test cancel the operation");

        int nPercent = 0;

        while (nPercent <= 100)
        {
            if (sdkProgressDlg.HasCancelled())
            {
                break;
            }

            nPercent += 5;
            sdkProgressDlg.SetProgress(nPercent, 100);
            Sleep(500);
        }
    }

    CoUninitialize();
    _endthreadex(0);
    return 0;
}

//////////////////////////////////////////////////////////////////////////

typedef struct _DATA
{
    int num;
} DATA;

void ChangePosition()
{
    vector<DATA*> vect;
    vector<DATA*>::iterator iter;
    vector<DATA*>::iterator iterTemp;

    for (int i = 0; i < 10; ++i)
    {
        DATA *pData = new DATA();
        pData->num = (i + 1);
        vect.push_back(pData);
    }

    printf("First Vector data:\n ");
    for each (DATA *pData in vect)
    {
        printf(" %d ", pData->num);
    }
    printf("\n");

    int index = 5;
    int size = (int)vect.size();

    //if (index > 0 && index < size)
    //{
    //    DATA* item = vect.at(index);

    //    for (int i = index; i > 0; i--)
    //    {
    //        vect[i] = vect[i - 1];
    //    }

    //    vect[0] = item;
    //}

    DATA *pData = *(vect.begin() + index);
    for (iter = vect.begin()+index; iter > vect.begin(); --iter)
    {
        *iter = *(iter - 1);
    }
    *(vect.begin()) = pData;

    printf("After Vector data:\n ");
    for each (DATA *pData in vect)
    {
        printf(" %d ", pData->num);
    }
    printf("\n");
}


//////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    CoInitialize(NULL);

    //TestCryptFileClass();
    //TestProgressDialog();

    //TestGetUserInfo();
    //TestCrypt();
    //TestMediaContent();
    //TestMediaInfoProvider();

    ChangePosition();

    CoUninitialize();

	return 0;
}
