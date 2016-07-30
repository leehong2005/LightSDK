
// SampleAssocHandlerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SampleAssocHandler.h"
#include "SampleAssocHandlerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleAssocHandlerDlg dialog

static const wchar_t* g_szExtensionList[] = 
{
    L".txt",
    L".png",
    L".docx",
    L".doc",
    L".pptx",
    L".ppt",
    L".xlsx",
    L".xls",
    L".html",
    L".mht",
    L".jpg",
    L".bmp",
    L".sln",
    L".h",
    L".cpp",
    L".xaml",
    L".xml",
    L".avi",
    L".wmv",
    L".mp3",
    L".mp4",
};



CSampleAssocHandlerDlg::CSampleAssocHandlerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSampleAssocHandlerDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_isBitmapChecked = TRUE;
}

void CSampleAssocHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_EXTENSION, m_comboBoxExt);
}

BEGIN_MESSAGE_MAP(CSampleAssocHandlerDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSampleAssocHandlerDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CSampleAssocHandlerDlg message handlers
BOOL CSampleAssocHandlerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon


    // Initialize data of combobox
    int size = ARRAYSIZE(g_szExtensionList);
    for (int i = 0; i < size; ++i)
    {
        m_comboBoxExt.AddString(g_szExtensionList[i]);
    }

    m_comboBoxExt.SetCurSel(0);

    CheckRadioButton(IDC_RADIO_ICON, IDC_RADIO_BITMAP, IDC_RADIO_BITMAP);
    CheckDlgButton(IDC_CHECK_RECOMMENDED, 1);

    HWND hBtnWnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_APPLY);
    Button_SetElevationRequiredState(hBtnWnd, TRUE);

    m_isBitmapChecked = TRUE;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSampleAssocHandlerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSampleAssocHandlerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////

void CSampleAssocHandlerDlg::OnBnClickedButtonApply()
{
    int nSelIndex = m_comboBoxExt.GetCurSel();
    if ( -1 != nSelIndex )
    {
        WCHAR szText[MAX_PATH] = { 0 };
        m_comboBoxExt.GetLBText(nSelIndex, szText);

        ClearList();

        BOOL isRecommended = (1 == IsDlgButtonChecked(IDC_CHECK_RECOMMENDED));
        SdkAssocHandler::GetAssocHandlers(m_assocHandlerList, szText, isRecommended);

        ResetHandlerItems();

        UpdateWindow();
    }
}

//////////////////////////////////////////////////////////////////////////

void CSampleAssocHandlerDlg::ClearList()
{
    for ( vector<SampleAssocHandlerItem*>::iterator itor = m_vctItemList.begin();
          itor != m_vctItemList.end(); ++itor)
    {
        SAFE_DELETE(*itor);
    }

    m_vctItemList.clear();
    m_assocHandlerList.clear();
}

//////////////////////////////////////////////////////////////////////////

void CSampleAssocHandlerDlg::ResetHandlerItems()
{
    int size = (int)m_assocHandlerList.size();
    int left = 12;
    int top = 100;
    int width = 128;
    int height = 150;
    int margin = 20;
    RECT rcClient = { 0 };
    GetClientRect(&rcClient);

    BOOL isIcon = (1 == IsDlgButtonChecked(IDC_RADIO_ICON));

    for (int i = 0; i < size; ++i)
    {
        SampleAssocHandlerItem *pItem = new SampleAssocHandlerItem(i);
        pItem->m_isIcon = isIcon;

        //left = i * (width + margin) + margin;
        if (left + width > rcClient.right)
        {
            top += (margin + height);
            left = 12;
        }

        pItem->Initialize(this, left, top, width, height, m_assocHandlerList[i]);
        m_vctItemList.push_back(pItem);

        left += (width + margin);
    }

    //wstring str(L"D:\\AssocImg");
    //str.append(m_assocHandlerList[0]->GetUIName()).append(L".png");
    //m_assocHandlerList[0]->SaveTo(str.c_str());
}