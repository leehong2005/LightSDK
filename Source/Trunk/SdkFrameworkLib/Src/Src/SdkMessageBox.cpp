/*!
* @file SdkMessageBox.cpp
* 
* @brief This file defines class to show message dialog box.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/28
*/

#include "stdafx.h"
#include "Resource.h"
#include "SdkMessageBox.h"
#include "SdkViewElement.h"
#include "SdkViewLayout.h"
#include "SdkButton.h"
#include "SdkTextView.h"
#include "SdkImageView.h"
#include "SdkResManager.h"
#include "SdkWindowDialog.h"
#include "SdkCommonInclude.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_WINDOW

BEGIN_NAMESPACE_WINDOW

/*!
* @brief Button type information.
*/
typedef struct _BUTTONTYPEINFO
{
    UINT    uBtnType;           // Button type, MB_OK, MB_YESNO, etc.
    UINT    uBtnNum;            // Button number, such as 1 when type is MB_OK.
    UINT    szBtnIds[3];        // Button ids, maximum button number is 3.

} BUTTONTYPEINFO;


/*!
* @brief Icon type information.
*/
typedef struct _ICONTYPEINFO
{
    UINT    uIconType;          // Icon type, such as MB_ICONERROR, etc.
    UINT    uIconResId;         // Icon resource id.

} ICONTYPEINFO;

END_NAMESPACE_WINDOW


/*!
* @brief Global button type array.
*/
static const BUTTONTYPEINFO g_szBtnTypeInfos[] = 
{
    { MB_RETRYCANCEL,       2, { IDRETRY, IDCANCEL, 0 } },
    { MB_YESNO,             2, { IDYES, IDNO, 0 } },
    { MB_YESNOCANCEL,       3, { IDYES, IDNO, IDCANCEL } },
    { MB_ABORTRETRYIGNORE,  3, { IDABORT, IDRETRY, IDIGNORE } },
    { MB_OKCANCEL,          2, { IDOK , IDCANCEL, 0 } },
    { MB_OK,                1, { IDOK,  0,    0 } },
};

/*!
* @brief Global icon type array.
*/
static const ICONTYPEINFO g_szIconTypeInfos[] = 
{
    { MB_ICONINFORMATION,   IDB_PNG_INFORMATION },
    { MB_ICONWARNING,       IDB_PNG_WARNING },
    { MB_ICONQUESTION,      IDB_PNG_QUESTION },
    { MB_ICONERROR,         IDB_PNG_ERROR },
};

map<UINT, UINT> SdkMessageBox::s_mapBtnTypeToResId;

//////////////////////////////////////////////////////////////////////////

SdkMessageBox::SdkMessageBox() : m_uBtnType(MB_OK),
                                 m_pCloseView(NULL),
                                 m_uIconType(MB_ICONWARNING)
{
    InitMessageTypeTextMap();
}

//////////////////////////////////////////////////////////////////////////

SdkMessageBox::~SdkMessageBox()
{
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SdkMessageBox::Show(HWND hWnd, IN LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    if ( NULL != lpText )
    {
        m_strText.clear();
        m_strText.append(lpText);
    }

    if ( NULL != lpCaption )
    {
        m_strCaption.clear();
        m_strCaption.append(lpCaption);
    }

    ParseMessageType(uType);

    return SdkWindowDialog::DoModal(hWnd);
}

//////////////////////////////////////////////////////////////////////////

void SdkMessageBox::InitMessageTypeTextMap()
{
    static BOOL hasInit = FALSE;

    if ( !hasInit )
    {
        hasInit = TRUE;

        s_mapBtnTypeToResId.insert(make_pair(IDOK,      IDS_BUTTON_OK     ));
        s_mapBtnTypeToResId.insert(make_pair(IDCANCEL,  IDS_BUTTON_CANCEL ));
        s_mapBtnTypeToResId.insert(make_pair(IDABORT,   IDS_BUTTON_ABORT  ));
        s_mapBtnTypeToResId.insert(make_pair(IDRETRY,   IDS_BUTTON_IDRETRY));
        s_mapBtnTypeToResId.insert(make_pair(IDIGNORE,  IDS_BUTTON_IGNORE ));
        s_mapBtnTypeToResId.insert(make_pair(IDYES,     IDS_BUTTON_YES    ));
        s_mapBtnTypeToResId.insert(make_pair(IDNO,      IDS_BUTTON_NO     ));
        s_mapBtnTypeToResId.insert(make_pair(IDCLOSE,   IDS_BUTTON_CLOSE  ));
        s_mapBtnTypeToResId.insert(make_pair(IDHELP,    IDS_BUTTON_HELP   ));
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkMessageBox::CreateLayouts()
{
    SdkViewLayout *pContent = new SdkViewLayout();
    pContent->SetBkImage(IDB_PNG_DIALOG, HINST_THISCOMPONENT);
    pContent->SetRoundCornerEnable(TRUE);
    pContent->SetRoundCornerRadius(10.0f, 10.0f);

    SdkTextView *pCaption = new SdkTextView();
    pCaption->SetLayoutInfo(10, 0, 470, 55);
    pCaption->SetTextColor(ColorF(ColorF::White));
    pCaption->SetText(m_strCaption.c_str());
    pCaption->SetTextAlign(TEXT_ALIGNMENT_LEFT);
    pContent->AddView(pCaption);

    SdkButton *pCloseBtn = new SdkButton(FALSE);
    pCloseBtn->SetLayoutInfo(490, 15, 25, 25);
    pCloseBtn->SetId(IDCLOSE);
    pCloseBtn->SetOnClickHandler(this);
    pCloseBtn->SetImage(IDB_CLOSE_BTN_BK_NORMAL, VIEW_STYLE_NORMAL, SdkResManager::GetResModule());
    pCloseBtn->SetImage(IDB_CLOSE_BTN_BK_DOWN, VIEW_STYLE_MOUSEHOVER, SdkResManager::GetResModule());
    pCloseBtn->SetImage(IDB_CLOSE_BTN_BK_DOWN, VIEW_STYLE_MOUSEDOWN, SdkResManager::GetResModule());
    pContent->AddView(pCloseBtn);
    m_pCloseView = pCloseBtn;

    SdkViewLayout *pTextLayout = new SdkViewLayout();
    pTextLayout->SetLayoutInfo(0, 55, (FLOAT)GetWidth(), 120);
    pContent->AddView(pTextLayout);

    SdkTextView *pTextView = new SdkTextView();
    pTextView->SetTextColor(ColorF(ColorF::White));
    pTextView->SetLayoutInfo(110, 0, 400, 120);
    pTextView->SetTextWrap(DWRITE_WORD_WRAPPING_WRAP);
    pTextView->SetTextAlign(TEXT_ALIGNMENT_LEFT);
    pTextView->SetText(m_strText.c_str());
    pTextLayout->AddView(pTextView);

    for (int i = 0; i < ARRAYSIZE(g_szIconTypeInfos); ++i)
    {
        if (m_uIconType == g_szIconTypeInfos[i].uIconType)
        {
            ICONTYPEINFO iconInfo = g_szIconTypeInfos[i];
            SdkImageView *pImageView = new SdkImageView();
            pImageView->SetLayoutInfo(30, 35, 50, 50);
            pImageView->SetSrcImage(iconInfo.uIconResId, HINST_THISCOMPONENT);
            pTextLayout->AddView(pImageView);
        }
    }

    for (int i = 0; i < ARRAYSIZE(g_szBtnTypeInfos); ++i)
    {
        if (m_uBtnType == g_szBtnTypeInfos[i].uBtnType)
        {
            BUTTONTYPEINFO btnInfo = g_szBtnTypeInfos[i];
            for (int j = 0; j < (int)btnInfo.uBtnNum; ++j)
            {
                UINT uResId = 0;
                UINT uLength = 0;
                map<UINT, UINT>::iterator itor = s_mapBtnTypeToResId.find(btnInfo.szBtnIds[j]);
                if (itor != s_mapBtnTypeToResId.end())
                {
                    uResId = itor->second;
                }
                SdkButton *pButton = new SdkButton(TRUE);
                pButton->SetOnClickHandler(this);
                pButton->SetTextColor(ColorF(ColorF::White));
                pButton->SetText(uResId, HINST_THISCOMPONENT);
                pButton->SetId(btnInfo.szBtnIds[j]);
                if (1 != (int)btnInfo.uBtnNum - j)
                {
                    uLength = 15;
                }
                else
                {
                    uLength = 20;
                }
                pButton->SetLayoutInfo((FLOAT)GetWidth() - ((int)btnInfo.uBtnNum - j) * (uLength + 100), 185, 100, 32);
                pContent->AddView(pButton);
            }
            break;
        }
    }

    RECT rcClient = { 0 };
    GetClientRect(m_hWnd, &rcClient);
    pContent->SetLayoutInfo(0, 0, (FLOAT)rcClient.right, (FLOAT)rcClient.bottom);

    AddView(pContent);
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SdkMessageBox::DoModal(HWND hWndParent)
{
    return SdkWindowDialog::DoModal(hWndParent);
}

//////////////////////////////////////////////////////////////////////////

void SdkMessageBox::EndDialog(INT32 nResult)
{
    SdkWindowDialog::EndDialog(nResult);
}

//////////////////////////////////////////////////////////////////////////

void SdkMessageBox::ParseMessageType(UINT uType)
{
    for (int i = 0; i < ARRAYSIZE(g_szBtnTypeInfos); ++i)
    {
        if (g_szBtnTypeInfos[i].uBtnType == (g_szBtnTypeInfos[i].uBtnType & uType))
        {
            m_uBtnType = g_szBtnTypeInfos[i].uBtnType;
            break;
        }
    }

    for (int i = 0; i < ARRAYSIZE(g_szIconTypeInfos); ++i)
    {
        if (g_szIconTypeInfos[i].uIconType == (g_szIconTypeInfos[i].uIconType & uType))
        {
            m_uIconType = g_szIconTypeInfos[i].uIconType;
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkMessageBox::OnCreated()
{
    SdkWindowDialog::OnCreated();

    // Set the round corner radius.
    SetRoundCornerRadius(10, 10);

    // Create the layouts of message box.
    CreateLayouts();
}

//////////////////////////////////////////////////////////////////////////

void SdkMessageBox::OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams)
{
    SdkWindowDialog::OnPreCreate(lpCreateWinParams);

    lpCreateWinParams->cx        = 535;
    lpCreateWinParams->cy        = 235;
    lpCreateWinParams->dwStyle   = DS_MODALFRAME | WS_POPUP | WS_BORDER;
    lpCreateWinParams->dwExStyle = WS_EX_LAYERED;
}

//////////////////////////////////////////////////////////////////////////

void SdkMessageBox::OnClick(SdkViewElement *pView)
{
    INT32 nId = pView->GetId();

    switch (nId)
    {
    case IDCLOSE:
    case IDOK:
    case IDCANCEL:
    case IDYES:
    case IDNO:
    case IDABORT:
    case IDIGNORE:
    case IDRETRY:
        EndDialog(nId);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkMessageBox::OnLongClick(SdkViewElement *pView)
{
    UNREFERENCED_PARAMETER(pView);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkMessageBox::OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // If handled is zero, represents window has processed this message.
    BOOL handled = SdkWindowDialog::OnWndProc(hWnd, message, wParam, lParam);

    if ( WM_LBUTTONDOWN == message )
    {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);

        // If the point is located in the title rectangle.
        if ( y >= 1 && y <= 55 )
        {
            BOOL isInCloseBox = FALSE;

            if ( NULL != m_pCloseView )
            {
                RECT rcView = D2DRECT_TO_RECT(m_pCloseView->GetViewRect());
                POINT pt = { x, y };
                isInCloseBox = ::PtInRect(&rcView, pt);
            }

            // Not on the close button.
            if ( !isInCloseBox )
            {
                ReleaseCapture();
                // Move the window when user drag the window with mouse left button pressing down.
                SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
                handled = 0;
            }
        }
    }

    return handled;
}
