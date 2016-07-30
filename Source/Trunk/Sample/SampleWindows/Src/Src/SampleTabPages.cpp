#include "StdAfx.h"
#include "SampleTabPages.h"
#include "SdkCommonInclude.h"


class MyProgressBarEventHandler : public IProgressBarEventHandler
{
    virtual void OnPercentChanged(SdkProgressBar *pProgressBar, FLOAT fCurPercent)
    {
        SdkCommonHelper::PrintDebugString(L" OnPercentChanged  fCurPercent = %f", fCurPercent);
    }
};

class MyProgressBarBtnClickHandler : public IViewOnClickHandler
{
public:

    SdkProgressBar *m_pProgressBar;

    MyProgressBarBtnClickHandler(SdkProgressBar *progressBar)
    {
        m_pProgressBar = progressBar;
    }

    void OnClick(SdkViewElement *pView)
    {
        if (NULL != m_pProgressBar)
        {
            SdkMessageBox msgBox;
            INT_PTR res = msgBox.Show((HWND)(*(pView->GetWindow())), L"Do you want to close window?",
                L"Information", MB_YESNO | MB_ICONINFORMATION);

            if (IDYES == res)
            {
                static BOOL fIncrease = TRUE;
                FLOAT percent = m_pProgressBar->GetPercent();
                fIncrease = (100 == percent) ? FALSE : fIncrease;
                fIncrease = (0   == percent) ? TRUE  : fIncrease;

                if (fIncrease)
                {
                    percent += 10;
                }
                else
                {
                    percent -= 10;
                }

                m_pProgressBar->SetPercent(percent);
            }
        }
    }

    BOOL OnLongClick(SdkViewElement *pView)
    {
        return FALSE;
    }
};

SampleTabPages::SampleTabPages(CONTROLTYPE eCtrlType, SdkTabView* pTabView)
{
    m_eCtrlType  = eCtrlType;
    m_pFunParser = new SampleFunParser();
    m_pView      = NULL;
    m_nTabWidth  = 990;
    m_nTabHeight = 590;

    if ( NULL != pTabView )
    {
        m_nTabWidth  = pTabView->GetWidth();
        m_nTabHeight = pTabView->GetHeight();
    }
}

//////////////////////////////////////////////////////////////////////////

SampleTabPages::~SampleTabPages()
{
    SAFE_DELETE(m_pFunParser);
}

//////////////////////////////////////////////////////////////////////////

CONTROLTYPE SampleTabPages::GetControlType() const
{
    return m_eCtrlType;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::GetView()
{
    if ( NULL == m_pView )
    {
        m_pView = CreateView(m_eCtrlType);
    }

    return m_pView;
}

//////////////////////////////////////////////////////////////////////////

LPCTSTR SampleTabPages::GetCtrlName()
{
    return CtrlTypeToName(m_eCtrlType);
}

//////////////////////////////////////////////////////////////////////////

void SampleTabPages::GetFunAndParams(vector<FUNCTIONINFO*>& vctFuns)
{
    switch (m_eCtrlType)
    {
    case TYPE_BUTTON:
        m_pFunParser->ParserButtonCtrl(vctFuns);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleTabPages::PerformFuns(FUNCTIONINFO *pFunInfo)
{
    if ( NULL == m_pView || NULL == pFunInfo )
    {
        return FALSE;
    }

    switch (pFunInfo->nFunId)
    {
    case _SetEnable:
        {
            int nVal = 0;
            SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nVal);
            BOOL isEnable = (nVal > 0) ? TRUE : FALSE;
            m_pView->SetEnable(isEnable);
        }
        break;

    case _SetViewPos:
        {
            int nValX = 0;
            int nValY = 0;
            SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nValX);
            SdkDataConvertor::ToInt32(pFunInfo->szParams[1].strValue.c_str(), nValY);
            m_pView->SetViewPos((FLOAT)nValX, (FLOAT)nValY);
        }
        break;

    case _SetViewSize:
        {
            int nValW = 0;
            int nValH = 0;
            SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nValW);
            SdkDataConvertor::ToInt32(pFunInfo->szParams[1].strValue.c_str(), nValH);
            m_pView->SetViewSize((FLOAT)nValW, (FLOAT)nValH);
        }
        break;

    case _SetVisible:
        {
            int nVal = 0;
            SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nVal);
            BOOL isVisible = (nVal > 0) ? TRUE : FALSE;
            m_pView->SetVisible(isVisible);
        }
        break;

    case _SetToggleStyle:
        {
            int nVal = 0;
            SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nVal);
            BOOL isToggle = (nVal > 0) ? TRUE : FALSE;
            ((SdkButton*)m_pView)->SetToggle(isToggle);
        }
        break;

    case _ShowUACIcon:
        {
            int nVal = 0;
            SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nVal);
            BOOL fShow = (nVal > 0) ? TRUE : FALSE;
            ((SdkButton*)m_pView)->ShowUACIcon(fShow);
        }
        break;

    case _SetText:
        {
            SdkTextView *pTextView = dynamic_cast<SdkTextView*>(m_pView);
            if ( NULL != pTextView )
            {
                pTextView->SetText(pFunInfo->szParams[0].strValue.c_str());
            }
        }
        break;

    case _SetTextAlign:
        {
            SdkTextView *pTextView = dynamic_cast<SdkTextView*>(m_pView);
            if ( NULL != pTextView )
            {
                int nVal = 0;
                SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nVal);
                pTextView->SetTextAlign((TEXT_ALIGNMENT)nVal);
            }
        }
        break;

    case _SetTextSize:
        {
            SdkTextView *pTextView = dynamic_cast<SdkTextView*>(m_pView);
            if ( NULL != pTextView )
            {
                int nVal = 18;
                SdkDataConvertor::ToInt32(pFunInfo->szParams[0].strValue.c_str(), nVal);
                pTextView->SetTextSize((FLOAT)nVal);
            }
        }
        break;
    }

    m_pView->Invalidate();

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateView(CONTROLTYPE eCtrlType)
{
    SdkViewElement *pView = NULL;

    switch (eCtrlType)
    {
    case TYPE_BUTTON:
        pView = CreateButton();
        break;

    case TYPE_CHECKBOX:
        pView = CreateCheckBox();
        break;

    case TYPE_COMBOBOX:
        pView = CreateComboBox();
        break;

    case TYPE_LISTBOX:
        pView = CreateListBox();
        break;

    case TYPE_GIFVIEW:
        pView = CreateGifView();
        break;

    case TYPE_GROUPBOX:
        pView = CreateGroupBox();
        break;

    case TYPE_EDITBOX:
        pView = CreateEditBox();
        break;

    case TYPE_IMAGEVIEW:
        pView = CreateImageView();
        break;

    case TYPE_OVERLAP:
        pView = CreateOverlapView();
        break;

    case TYPE_PROGRESS:
        pView = CreateProgress();
        break;

    case TYPE_RADIOBUTTON:
        pView = CreateRadioButton();
        break;

    case TYPE_RATINGVIEW:
        pView = CreateRatingView();
        break;

    case TYPE_SCROLLBAR:
        pView = CreateScrollBar();
        break;

    case TYPE_SEEKBAR:
        pView = CreateSeekBar();
        break;

    case TYPE_TABHEADER:
        pView = CreateTabHeader();
        break;

    case TYPE_TABVIEW:
        pView = CreateTabView();
        break;

    case TYPE_TEXTVIEW:
        pView = CreateTextView();;
        break;
    }

    return pView;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateButton()
{
    SdkButton *pButton = new SdkButton();
    FLOAT left = (m_nTabWidth  - pButton->GetWidth())  / 2;
    FLOAT top  = (m_nTabHeight - pButton->GetHeight()) / 2;
    pButton->SetViewPos(left, top - 50);
    pButton->SetText(NAME_BUTTON);

    return pButton;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateCheckBox()
{
    SdkCheckBox *pCheckBox = new SdkCheckBox();
    FLOAT left = (m_nTabWidth  - pCheckBox->GetWidth())  / 2;
    FLOAT top  = (m_nTabHeight - pCheckBox->GetHeight()) / 2;
    pCheckBox->SetViewPos(left, top - 50);
    pCheckBox->SetText(NAME_CHECKBOX);

    return pCheckBox;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateComboBox()
{
    SdkComboBox *pComboBox = new SdkComboBox();
    FLOAT left = (m_nTabWidth  - pComboBox->GetWidth())  / 2;
    FLOAT top  = (m_nTabHeight - pComboBox->GetHeight()) / 2;
    pComboBox->SetViewPos(left, top - 50);

    pComboBox->AddItem(L"One");
    pComboBox->AddItem(L"Two");
    pComboBox->AddItem(L"Three");
    pComboBox->SetCurSel(0);

    return pComboBox;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateEditBox()
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateListBox()
{
    SdkListBox *pListBox = new SdkListBox();

    FLOAT left = (m_nTabWidth  - pListBox->GetWidth())  / 2;
    FLOAT top  = (m_nTabHeight - pListBox->GetHeight()) / 2;
    pListBox->SetViewPos(left, top - 50);

    pListBox->AddItem(L"One");
    pListBox->AddItem(L"Two");
    pListBox->AddItem(L"Three");
    pListBox->AddItem(L"Four");
    pListBox->AddItem(L"Five");

    return pListBox;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateGifView()
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateGroupBox()
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateImageView()
{
    SdkImageView *pImageView = new SdkImageView();
    pImageView->SetViewPos(200, 200);
    pImageView->SetViewSize(200, 200);

    return pImageView;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateOverlapView()
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateProgress()
{
    SdkViewLayout *layout = new SdkViewLayout();
    layout->SetLayoutInfo(100, 100, 500, 350);
    layout->SetBkColor(ColorF::Blue);

    SdkProgressBar *pProgress = new SdkProgressBar();
    pProgress->SetViewPos(20, 20);
    pProgress->SetViewSize(400, 100);
    pProgress->SetPercent(3);

    SdkButton *pBtn = new SdkButton();
    pBtn->SetLayoutInfo(20, 60, 150, 50);
    pBtn->SetText(_T("Click Me"));
    pBtn->SetOnClickHandler(new MyProgressBarBtnClickHandler(pProgress));
 
    layout->AddView(pProgress);
    layout->AddView(pBtn);

    return layout;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateRadioButton()
{
    SdkRadioButton *pRadio = new SdkRadioButton();
    pRadio->SetViewPos(130, 130);
    pRadio->SetText(L"This is a radio button.");

    return pRadio;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateRatingView()
{
    SdkRatingView *pRating = new SdkRatingView();
    pRating->SetViewPos(200, 200);

    return pRating;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateScrollBar()
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateSeekBar()
{
    SdkViewLayout *pLayout = new SdkViewLayout();
    pLayout->SetViewPos(100, 100);
    pLayout->SetViewSize(600, 400);
    pLayout->SetBkColor(ColorF::Blue);

    SdkSeekBar *pSeekBar = new SdkSeekBar();
    pSeekBar->SetViewPos(100, 100);
    pSeekBar->SetViewSize(400, 100);
    pSeekBar->SetPercent(53);

    pSeekBar->SetProgressBarEventHandler(new MyProgressBarEventHandler());

    pLayout->AddView(pSeekBar);

    return pLayout;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateTabHeader()
{
    SdkTabHeader *pTabHeader = new SdkTabHeader();

    pTabHeader->SetViewPos(100, 100);
    pTabHeader->SetViewSize(500, 100);
    pTabHeader->SetAddTabVisible(TRUE);
    pTabHeader->InsertTab(0, L"Tab 1", 0);

    return pTabHeader;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateTabView()
{
    SdkTabView *pTabView = new SdkTabView();
    pTabView->SetViewPos(50, 50);
    pTabView->SetViewSize(800, 430);
    pTabView->SetTabDockDir(TAB_DOCK_TOP);
    pTabView->SetAddTabVisible(TRUE);

    pTabView->SetTabText(0, _T("Tab 1"));

    for (int i = 1; i < 5; ++i)
    {
        static TCHAR szBuf[100] = { 0 };
        swprintf_s(szBuf, _T("Tab %d"), (i + 1));
        pTabView->InsertTab(pTabView->GetTabCount(), szBuf);
    }

    //
    // Tab 1
    //
    SdkButton *pBtn1 = new SdkButton();
    pBtn1->SetText(L"button");
    pBtn1->SetViewPos(30, 30);

    SdkButton *pBtn2 = new SdkButton();
    pBtn2->SetText(L"toggle button");
    pBtn2->SetWrapContent(TRUE);
    pBtn2->SetViewPos(30, 100);
    pBtn2->SetToggle(TRUE);

    SdkButton *pBtn3 = new SdkButton();
    pBtn3->SetText(L"big button");
    pBtn3->SetViewPos(30, 170);
    pBtn3->SetViewSize(200, 100);
    pBtn3->SetToggle(TRUE);

    pTabView->AddTabView(0, pBtn1);
    pTabView->AddTabView(0, pBtn2);
    pTabView->AddTabView(0, pBtn3);


    //
    // Tab2
    //
    SdkRatingView *pRating1 = new SdkRatingView();
    pRating1->SetViewPos(30, 30);

    SdkRatingView *pRating2 = new SdkRatingView();
    pRating2->SetViewPos(30, 100);
    pRating2->SetStarsCount(10);
    pRating2->SetRating(3);

    pTabView->AddTabView(1, pRating1);
    pTabView->AddTabView(1, pRating2);


    return pTabView;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SampleTabPages::CreateTextView()
{
    SdkTextView *pTextView = new SdkTextView();
    pTextView->SetBkColor(ColorF::Red);
    pTextView->SetViewPos(120, 120);
    pTextView->SetViewSize(130, 40);
    pTextView->SetText(L"Text View");
    TEXT_METRICS metrics = pTextView->GetTextMetrics();

    pTextView->SetViewSize(300, 100);
    metrics = pTextView->GetTextMetrics();
    pTextView->SetTextAlign(TEXT_ALIGNMENT_LEFT);
    metrics = pTextView->GetTextMetrics();

    pTextView->SetTextAlign(TEXT_ALIGNMENT_RIGHT);
    metrics = pTextView->GetTextMetrics();

    return pTextView;
}
