// SampleAssocHandlerItem.cpp : implementation file
//

#include "stdafx.h"
#include "SampleAssocHandler.h"
#include "SampleAssocHandlerItem.h"


// SampleAssocHandlerItem

IMPLEMENT_DYNAMIC(SampleAssocHandlerItem, CWnd)

SampleAssocHandlerItem::SampleAssocHandlerItem(int nIndex)
{
    m_isIcon        = FALSE;
    m_pAssocHandler = NULL;
    m_nIndex = nIndex;
}

SampleAssocHandlerItem::~SampleAssocHandlerItem()
{
    SAFE_DELETE(m_pAssocHandler);
}


BEGIN_MESSAGE_MAP(SampleAssocHandlerItem, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()

// SampleAssocHandlerItem message handlers

BOOL SampleAssocHandlerItem::Initialize(CWnd *pParent, int x, int y, int width, int height, SdkAssocHandler *pData)
{
    if ( NULL == pParent )
    {
        return FALSE;
    }

    m_pAssocHandler = pData;
    RECT rect = { x, y, x + width, y + height };
    DWORD dwStyle = WS_CHILD | WS_VISIBLE;

    BOOL isOK = CWnd::Create(NULL, L"AssocHandlerItem", dwStyle, rect, pParent, 1000);
    return isOK;
}

//////////////////////////////////////////////////////////////////////////

void SampleAssocHandlerItem::OnPaint()
{
    PAINTSTRUCT ps = { 0 };
    HDC hDC = ::BeginPaint(m_hWnd, &ps);

    RECT clientRc = { 0 };
    GetClientRect(&clientRc);

    if ( TRUE == m_isIcon )
    {
        int left = (clientRc.right - 32) / 2;
        int top = (clientRc.right - 32) / 2;
        ::DrawIconEx(hDC,
            left,
            top,
            m_pAssocHandler->GetIcon(),
            0,
            0,
            0,
            NULL,
            DI_IMAGE | DI_MASK | DI_DEFAULTSIZE);
    }
    else
    {
        SdkImagesManager image;
        image.LoadImageFromHBITMAP(m_pAssocHandler->GetBitmap(128, 128));
        int left = (clientRc.right - image.GetWidth()) / 2;
        int top = (clientRc.right - image.GetHeight()) / 2;

        Graphics g(hDC);
        g.DrawImage(image.GetImage(), left, top, clientRc.right, clientRc.right);
    }

    ::EndPaint(m_hWnd, &ps);
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleAssocHandlerItem::OnEraseBkgnd(CDC* pDC)
{
    UNREFERENCED_PARAMETER(pDC);
    return TRUE;
}
