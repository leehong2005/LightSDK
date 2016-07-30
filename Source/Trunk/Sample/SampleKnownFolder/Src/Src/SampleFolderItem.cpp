#include "StdAfx.h"
#include "SampleFolderItem.h"

SampleFolderItem::SampleFolderItem()
{
    m_hItemIcon = NULL;
    m_szDesc[0] = 0;
    m_hIcon = NULL;
    ZeroMemory(&m_rcPaint, sizeof(RECT));
    ZeroMemory(&m_rcClip,  sizeof(RECT));
}

//////////////////////////////////////////////////////////////////////////

SampleFolderItem::~SampleFolderItem()
{
}

//////////////////////////////////////////////////////////////////////////

void SampleFolderItem::SetClipRect(LPRECT lprcClip)
{
    if (NULL != lprcClip)
    {
        m_rcClip = *lprcClip;
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFolderItem::Initialize(HBITMAP hBitmap, HICON hIcon, LPCTSTR lpDesc, LPRECT lpRect)
{
    m_hIcon = hIcon;
    m_hItemIcon = hBitmap;

    if (NULL != hBitmap)
    {
        _tcscpy_s(m_szDesc, MAX_PATH, lpDesc);
    }

    if (NULL != lpRect)
    {
        m_rcPaint = *lpRect;
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFolderItem::OnPaint(HDC hDC, BOOL drawBitmap, int dy)
{
    RECT rc = m_rcPaint;
    OffsetRect(&rc, 0, dy);

    if (drawBitmap)
    {
        BLENDFUNCTION ftn = { 0 };
        ftn.BlendOp = AC_SRC_OVER;
        ftn.AlphaFormat = AC_SRC_ALPHA;
        ftn.BlendFlags = 0;
        ftn.SourceConstantAlpha = 255;

        HDC hMemDC = CreateCompatibleDC(hDC);
        HGDIOBJ hOldObj = SelectObject(hMemDC, m_hItemIcon);


        BITMAP bmp;
        GetObject(m_hItemIcon, sizeof(bmp), (LPVOID)&bmp);

        AlphaBlend(
            hDC,
            rc.left,
            rc.top,
            (rc.right - rc.left),
            (rc.right - rc.left),
            hMemDC,
            0,
            0,
            bmp.bmWidth,
            bmp.bmHeight,
            ftn);

        SelectObject(hMemDC, hOldObj);
        DeleteDC(hMemDC);
    }
    else
    {
        static int nIconCx = ::GetSystemMetrics(SM_CXICON);

        ::DrawIconEx(
            hDC,
            rc.left + ((rc.right - rc.left) - nIconCx) / 2,
            rc.top + ((rc.right - rc.left) - nIconCx) / 2,
            m_hIcon,
            0,
            0,
            0,
            NULL,
            DI_IMAGE | DI_MASK | DI_DEFAULTSIZE);
    }


    //RECT rcText = { 0 };
    //rcText.left = rc.left;
    //rcText.right = rc.right;
    //rcText.bottom = rc.bottom;
    //rcText.top = rc.top + (rc.right - rc.left);
    //DrawText(hDC, m_szDesc, _tcslen(m_szDesc), &rcText, DT_CENTER | DT_END_ELLIPSIS);
}
