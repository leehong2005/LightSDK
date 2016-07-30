#ifdef __cplusplus
#ifndef _SDKCRYPTTOOLWINDOW_H_
#define _SDKCRYPTTOOLWINDOW_H_

#include "Resource.h"
#include "SdkCommonInclude.h"
#include "SampleShowMessage.h"
#include "SamplePWDDialog.h"

class SdkCryptToolWindow : public ICryptFileNotify
{
public:

    SdkCryptToolWindow();
    ~SdkCryptToolWindow();

    BOOL CreateToolWindow(HINSTANCE hInst, HWND hWndParent);

    void OnCryptBegin();
    void OnCryptFinish();
    void OnCryptNext(IN DWORD dwCurIndex, IN LPCTSTR lpCurFilePath);
    void OnCryptError(IN LPCTSTR lpErrorMsg);
    void OnProgressChanged(IN DWORD dwCompleted, IN DWORD dwTotal);
    void OnReplaceFiles(OUT CRYPT_INQUIRE_RESULT *pResult, IN LPCTSTR lpFileName);

private:

    CryptString CryptKeyFileName(IN const CryptString& strKeyFileName);

    void OnDoCrypt(BOOL isEncrypt);
    void OnBrowserClick(HWND hWndCtrl);
    void OnEncryptClick(HWND hWndCtrl);
    void OnDecryptClick(HWND hWndCtrl);
    void OnSetPWD(HWND hWndCtrl);
    INT_PTR OnDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

    HWND                m_hWnd;
    HINSTANCE           m_hInst;
    CryptString         m_strPWD;
    TCHAR               m_szKeyPath[MAX_PATH];
    SampleShowMessage  *m_pMsgDlg;
};

#endif // _SDKCRYPTTOOLWINDOW_H_
#endif // __cplusplus

