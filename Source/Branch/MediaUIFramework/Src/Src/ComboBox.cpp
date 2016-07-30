/*!
* @file ComboBox.cpp
* 
* @brief This file defines the ComboBox class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/02/11
*/

#include "ComboBox.h"
#include "Resource.h"

#define DEFAULT_WIDTH               115     // Default width of this view.
#define DEFAULT_HEIGHT              40      // Default height of this view.
#define DROP_BUTTON_ID              101     // The Drop_down button id.
#define VIEW_GAP                    10      // The gap of comboBox and MenuList.
#define PADDING_LEFT                20      // The distance of menuList left edge to comboBox left edge.
#define TEXTVIEW_RATIO              0.82    // The ratio of textView's width to whole view's width.

ComboBox::ComboBox() : m_pTextView(NULL),
                       m_pImgBtn(NULL),
                       m_pMenuList(NULL),
                       m_isDeleteList(TRUE),
                       m_pComboBoxEventHandler(NULL)
{
    SetRoundCornerEnable(TRUE);
    SetRoundCornerRadius(5, 5);
    SetBorderWidth(2);
    Initialize();
    SetViewSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

//////////////////////////////////////////////////////////////////////////

ComboBox::~ComboBox()
{
    if (m_isDeleteList)
    {
        SAFE_DELETE(m_pMenuList);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::Initialize()
{
    m_pTextView = new TextView();
    m_pTextView->SetText(L"");
    m_pTextView->SetTextColor(ColorF(ColorF::White));
    m_pTextView->SetBackgroundColor(ColorF(ColorF(0, 0)));
    m_pTextView->SetFontWeight(DWRITE_FONT_WEIGHT_BLACK);
    m_pTextView->SetEnable(FALSE);

    m_pImgBtn = new ImageButton();
    m_pImgBtn->SetId(DROP_BUTTON_ID);
    m_pImgBtn->SetBackgroundImage(IDB_PNG_DROP_BOX_NORMAL, HINST_THISCOMPONENT);
    m_pImgBtn->SetFocusImage(IDB_PNG_DROP_BOX_TOUCH, HINST_THISCOMPONENT);
    m_pImgBtn->SetRoundCornerEnable(FALSE);
    m_pImgBtn->SetRoundCornerRadius(20, 20);
    m_pImgBtn->SetOnClickHandler(this);

    m_pMenuList = new ListBox();
    m_pMenuList->SetPopupEnable(TRUE);
    m_pMenuList->SetListEventHandler(this);

    this->AddView(m_pImgBtn);
    this->AddView(m_pTextView);
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::AddItem(IN LPCWSTR lpItemText)
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->AddItem(lpItemText);
    }
}

void ComboBox::AddItem(UINT32 uResId, HMODULE hInst)
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->AddItem(uResId, hInst);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::AddItem(IN LPCWSTR lpItemText, UINT32 uIndex)
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->AddItem(lpItemText, uIndex);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::DeleteItem(UINT32 uIndex)
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->DeleteItem(uIndex);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::RemoveAllItem()
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->RemoveAllItem();
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::SetComboBoxText(IN LPWSTR lpText)
{
    if (NULL != lpText)
    {
        m_pTextView->SetText(lpText);
        ShowPopupView(FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::SetItemText(IN INT32 menuItemIndex, IN LPWSTR lpText)
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->SetItemText(menuItemIndex, lpText);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::SetItemText(IN INT32 menuItemIndex, IN UINT32 uResId, HMODULE hInst)
{
    TCHAR szBuffer[1024] = { 0 };
    hInst = (NULL == hInst) ? GetModuleHandle(NULL) : hInst;
    INT32 nLen = LoadString(hInst, uResId, szBuffer, 1024);

    if (nLen > 0)
    {
        SetItemText(menuItemIndex, szBuffer);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::SetCurSel(INT32 nIndex)
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->SetCurSel(nIndex);

        // Set combobox text.
        UINT32 nLength = m_pMenuList->GetSelItemTextLength();
        if (nLength > 0)
        {
            nLength += 1;
            LPWSTR lpText = new TCHAR[nLength];
            ZeroMemory(lpText, nLength);
            m_pMenuList->GetSelItemText(lpText, nLength);
            SetComboBoxText(lpText);
            SAFE_DELETE_ARRAY(lpText);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

INT32 ComboBox::GetCurSel()
{
    if (NULL != m_pMenuList)
    {
        return m_pMenuList->GetSelIndex();
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////////

BOOL ComboBox::GetSelItemText(OUT LPWSTR lpText, IN UINT32 uSize)
{
    if (NULL != m_pMenuList)
    {
        return m_pMenuList->GetSelItemText(lpText, uSize);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

UINT32 ComboBox::GetSelItemTextLength()
{
    if (NULL != m_pMenuList)
    {
        return m_pMenuList->GetSelItemTextLength();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::SetParent(ViewLayout *pParentView)
{
    if (NULL != pParentView)
    {
        if (NULL != m_pMenuList)
        {
            m_pMenuList->SetVisible(FALSE);
            pParentView->AddView(m_pMenuList);
            m_isDeleteList = FALSE;
        }
    }

    ViewLayout::SetParent(pParentView);
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::SetItemFocusColor(const D2D1_COLOR_F& color)
{
    if (NULL != m_pMenuList)
    {
        m_pMenuList->SetItemFocusColor(color);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::ShowPopupView(BOOL isShow)
{
    if (NULL != m_pMenuList)
    {
        FLOAT xPos_MenuList = GetLeft() - PADDING_LEFT;
        FLOAT yPos_MenuList = GetTop() + GetHeight() + VIEW_GAP;
        FLOAT width_MenuList = GetWidth() + 2 * PADDING_LEFT;

        m_pMenuList->SetLayoutInfo(xPos_MenuList,yPos_MenuList, width_MenuList, m_pMenuList->GetHeight());
        m_pMenuList->ShowView(isShow);
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::SetComboBoxEventHandler(IN ComboBoxEventHandler *pComboBoxEventHandler)
{
    m_pComboBoxEventHandler = pComboBoxEventHandler;
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    LAYOUTINFO pLayoutInfo = { 0 };
    GetLayoutInfo(&pLayoutInfo);

    FLOAT fWidth = pLayoutInfo.width;
    FLOAT fHeight = pLayoutInfo.height;

    m_pTextView->SetLayoutInfo(0, 0, (FLOAT)TEXTVIEW_RATIO * fWidth, fHeight);
    m_pImgBtn->SetLayoutInfo(0, 0, fWidth, fHeight);

    ViewLayout::OnLayout(fChanged, left, top, width, height);
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::OnClick(ViewElement* view)
{
    switch(view->GetId())
    {
    case DROP_BUTTON_ID:
        {
            if (NULL != m_pMenuList)
            {
                ShowPopupView(!m_pMenuList->IsVisible());
            }
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::OnSelectedChanged(ListBox *pView)
{
    if (NULL != pView)
    {
        UINT32 nLength = pView->GetSelItemTextLength();
        if (nLength > 0)
        {
            nLength += 1;
            LPWSTR lpText = new TCHAR[nLength];
            ZeroMemory(lpText, nLength);
            pView->GetSelItemText(lpText, nLength);
            SetComboBoxText(lpText);
            SAFE_DELETE_ARRAY(lpText);
        }
        else
        {
            SetComboBoxText(L"");
        }

        pView->ShowView(FALSE);

        if (NULL != m_pComboBoxEventHandler)
        {
            m_pComboBoxEventHandler->OnSelectedChanged(this);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void ComboBox::OnVisibleChanged(ListBox *pView)
{
    if (NULL != m_pImgBtn)
    {
        UINT uResId = (pView->IsVisible()) ? IDB_PNG_DROP_BOX_OPEN : IDB_PNG_DROP_BOX_NORMAL;
        m_pImgBtn->SetBackgroundImage(uResId, HINST_THISCOMPONENT);
    }
}
