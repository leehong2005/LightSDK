/*!
* @file MessageDialogBox.cpp
* 
* @brief This file defines class to show message dialog box.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/28
*/

#include "MessageDialogBox.h"
#include "Resource.h"

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

map<UINT, UINT> MessageDialogBox::s_mapBtnTypeToResId;

//////////////////////////////////////////////////////////////////////////

MessageDialogBox::MessageDialogBox() : m_uBtnType(MB_OK),
                                       m_pCloseView(NULL),
                                       m_uIconType(MB_ICONWARNING)
{
    InitMessageTypeTextMap();
}

//////////////////////////////////////////////////////////////////////////

MessageDialogBox::~MessageDialogBox()
{
}

//////////////////////////////////////////////////////////////////////////

INT_PTR MessageDialogBox::Show(HWND hWnd, IN LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    if (NULL != lpText)
    {
        m_strText.clear();
        m_strText.append(lpText);
    }

    if (NULL != lpCaption)
    {
        m_strCaption.clear();
        m_strCaption.append(lpCaption);
    }

    ParseMessageType(uType);

    return WindowDialog::DoModal(hWnd);
}

//////////////////////////////////////////////////////////////////////////

void MessageDialogBox::InitMessageTypeTextMap()
{
    static BOOL hasInit = FALSE;
    if (!hasInit)
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

void MessageDialogBox::CreateLayouts()
{
    ViewLayout *pContent = new ViewLayout();
    pContent->SetBackgroundImage(IDB_PNG_DIALOG, HINST_THISCOMPONENT);
    pContent->SetRoundCornerEnable(TRUE);
    pContent->SetRoundCornerRadius(10.0f, 10.0f);

    TextView *pCaption = new TextView();
    pCaption->SetLayoutInfo(10, 0, 470, 55);
    pCaption->SetTextColor(ColorF(ColorF::White));
    pCaption->SetText(m_strCaption.c_str());
    pCaption->SetTextAlign(DWRITE_TEXT_ALIGNMENT_LEADING);
    pContent->AddView(pCaption);

    ImageButton *pCloseBtn = new ImageButton();
    pCloseBtn->SetLayoutInfo(490, 15, 25, 25);
    pCloseBtn->SetId(IDCLOSE);
    pCloseBtn->SetOnClickHandler(this);
    pCloseBtn->SetBackgroundImage(IDB_BUTTON_CLOSE_N, HINST_THISCOMPONENT);
    pCloseBtn->SetFocusImage(IDB_BUTTON_CLOSE_T, HINST_THISCOMPONENT);
    pContent->AddView(pCloseBtn);
    m_pCloseView = pCloseBtn;

    ViewLayout *pTextLayout = new ViewLayout();
    pTextLayout->SetLayoutInfo(0, 55, (FLOAT)GetWidth(), 120);
    pContent->AddView(pTextLayout);

    TextView *pTextView = new TextView();
    pTextView->SetTextColor(ColorF(ColorF::White));
    pTextView->SetLayoutInfo(110, 0, 400, 120);
    pTextView->SetTextWrap(DWRITE_WORD_WRAPPING_WRAP);
    pTextView->SetTextAlign(DWRITE_TEXT_ALIGNMENT_LEADING);
    pTextView->SetText(m_strText.c_str());
    pTextLayout->AddView(pTextView);

    for (int i = 0; i < ARRAYSIZE(g_szIconTypeInfos); ++i)
    {
        if (m_uIconType == g_szIconTypeInfos[i].uIconType)
        {
            ICONTYPEINFO iconInfo = g_szIconTypeInfos[i];
            ImageView *pImageView = new ImageView();
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
                ImageButton *pButton = new ImageButton(TRUE);
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

    SetContentView(pContent);
}

//////////////////////////////////////////////////////////////////////////

INT_PTR MessageDialogBox::DoModal(HWND hWndParent)
{
    return WindowDialog::DoModal(hWndParent);
}

//////////////////////////////////////////////////////////////////////////

void MessageDialogBox::EndDialog(INT32 nResult)
{
    WindowDialog::EndDialog(nResult);
}

//////////////////////////////////////////////////////////////////////////

void MessageDialogBox::ParseMessageType(UINT uType)
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

void MessageDialogBox::OnCreated()
{
    WindowDialog::InitializeDevice();

    CreateLayouts();
}

//////////////////////////////////////////////////////////////////////////

void MessageDialogBox::OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams)
{
    WindowDialog::OnPreCreate(lpCreateWinParams);

    lpCreateWinParams->cx        = 535;
    lpCreateWinParams->cy        = 235;
    lpCreateWinParams->dwStyle   = DS_MODALFRAME | WS_POPUP | WS_BORDER;
    lpCreateWinParams->dwExStyle = WS_EX_LAYERED;
}

//////////////////////////////////////////////////////////////////////////

void MessageDialogBox::OnClick(ViewElement *pView)
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

BOOL MessageDialogBox::OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // If handled is zero, represents window has processed this message.
    BOOL handled = WindowDialog::OnWndProc(hWnd, message, wParam, lParam);

    if (WM_LBUTTONDOWN == message)
    {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        if (y >= 1 && y <= 55)
        {
            BOOL isInCloseBox = FALSE;
            if (NULL != m_pCloseView)
            {
                RECT rcView = D2DRECT_TO_RECT(m_pCloseView->GetViewRect());
                POINT pt = { x, y };
                isInCloseBox = PtInRect(&rcView, pt);
            }

            if (!isInCloseBox)
            {
                ReleaseCapture();
                SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
                handled = 0;
            }
        }
    }

    return handled;
}
