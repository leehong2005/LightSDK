/*!
* @file TFPUCryptFile.h 
* 
* @brief the class defines the progress dialog. it supplies the funtion to display the percent.
* 
* Copyright (C) 2008, LZT Corporation.
* 
* @author Li Hong
* @date 2009/01/18
*/

#include "stdafx.h"
#include "SdkProgressDialog.h"
#include "SdkCommonHelper.h"

USING_NAMESPACE_COMMON
USING_NAMESPACE_UTILITIES

SdkProgressDialog::SdkProgressDialog()
{
    m_pProgressDlg  = NULL;
    m_hProgDlgWnd   = NULL;
    m_isShowTopMost = FALSE;
}

//////////////////////////////////////////////////////////////////////////

SdkProgressDialog::~SdkProgressDialog()
{
    if (NULL != m_pProgressDlg)
    {
        m_pProgressDlg->StopProgressDialog();
    }

    SAFE_RELEASE(m_pProgressDlg);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkProgressDialog::Start(BOOL isShowTopMost)
{
    m_isShowTopMost = isShowTopMost;

    SAFE_RELEASE(m_pProgressDlg);
    CoCreateInstance(
        CLSID_ProgressDialog,
        NULL,
        CLSCTX_ALL,
        IID_PPV_ARGS(&m_pProgressDlg));

    HRESULT hr = (NULL != m_pProgressDlg) ? S_OK : E_FAIL;
    if ( SUCCEEDED(hr) )
    {
        // Auto calculate left time.
        hr = m_pProgressDlg->StartProgressDialog(
            NULL,
            NULL,
            PROGDLG_NORMAL | PROGDLG_AUTOTIME,
            NULL);

        if ( SUCCEEDED(hr) && m_isShowTopMost )
        {
            CryptString strGuid = SdkCommonHelper::CreateNewGuid();
            m_pProgressDlg->SetTitle(strGuid.c_str());

            // Bring the window to topmost
            m_hProgDlgWnd = ::FindWindow(L"#32770", strGuid.c_str());
            ::SetWindowPos(
                m_hProgDlgWnd,
                HWND_TOPMOST, 
                0,
                0,
                0,
                0,
                SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

            m_pProgressDlg->SetTitle(m_szTitle.c_str());
        }
    }

    return (S_OK == hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkProgressDialog::Stop()
{
    HRESULT hr = (NULL != m_pProgressDlg) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        hr = m_pProgressDlg->StopProgressDialog();
        SAFE_RELEASE(m_pProgressDlg);
    }

    return (S_OK == hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkProgressDialog::SetProgress(IN DWORD dwCompleted, IN DWORD dwTotal)
{
    if (m_isShowTopMost)
    {
        // Make the progress dialog topmost.
        ::SetWindowPos(
            m_hProgDlgWnd,
            HWND_TOPMOST,
            0,
            0,
            0,
            0,
            SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
    }

    HRESULT hr = (NULL != m_pProgressDlg) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        hr = m_pProgressDlg->SetProgress(dwCompleted, dwTotal);
    }

    return (S_OK == hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkProgressDialog::HasCancelled()
{
    if (NULL != m_pProgressDlg)
    {
        return m_pProgressDlg->HasUserCancelled();
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkProgressDialog::SetLines(IN DWORD dwLineNum, IN const CryptString& szString)
{
    HRESULT hr = (NULL != m_pProgressDlg) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        hr = m_pProgressDlg->SetLine(dwLineNum, szString.c_str(), TRUE, NULL);
    }

    return (S_OK == hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkProgressDialog::SetTitle(IN const CryptString& szTitle)
{
    m_szTitle = szTitle;

    HRESULT hr = (NULL != m_pProgressDlg) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        hr = m_pProgressDlg->SetTitle(szTitle.c_str());
    }

    return (S_OK == hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkProgressDialog::SetCancelMessage(IN const CryptString& szCancelMsg)
{
    HRESULT hr = (NULL != m_pProgressDlg) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        hr = m_pProgressDlg->SetCancelMsg(szCancelMsg.c_str(), NULL);
    }

    return (S_OK == hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

HWND SdkProgressDialog::GetProgressDialogHwnd() const
{
    return m_hProgDlgWnd;
}