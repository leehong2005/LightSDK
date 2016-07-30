/*!
* @file  SdkImageView.cpp
* 
* @brief This file defines the function of the SdkImageView module,
*        which can convert image into D2Dbitmap.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "stdafx.h"
#include "SdkImageView.h"
#include "SdkD2DTheme.h"
#include "D2DBitmap.h"
#include "D2DRectUtility.h"

USING_NAMESPACE_THEME

struct NAMESPACE_VIEWS::SdkImageView::_IMAGEVIEW_INTERNALDATA
{
    FLOAT                m_leftMargin;          // The left margin of source image to background.
    FLOAT                m_topMargin;           // The top margin of source image to background.
    FLOAT                m_rightMargin;         // The right margin of source image to background.
    FLOAT                m_bottomMargin;        // The bottom margin of source image to background.
    D2D1_RECT_F          m_imageDrawRect;       // The image drawing rectangle.
    IMAGE_STRETCH_MODE   m_stretchMode;         // The flag whether to Stretch the bitmap to fill all view.
    D2DBitmap           *m_pSrcD2DBitmap;       // The pointer which points to the object of D2DBitmap.
};

//////////////////////////////////////////////////////////////////////////

SdkImageView::SdkImageView()
{
    m_lpImageViewData = new _IMAGEVIEW_INTERNALDATA();
    ZeroMemory(m_lpImageViewData, sizeof(_IMAGEVIEW_INTERNALDATA));

    m_lpImageViewData->m_pSrcD2DBitmap = new D2DBitmap();
    m_lpImageViewData->m_stretchMode   = IMAGE_STRETCH_MODE_CENTER;

    SetClassName(CLASSNAME_IMAGEVIEW);
    SetClickable(FALSE);
}

//////////////////////////////////////////////////////////////////////////

SdkImageView::~SdkImageView()
{
    SAFE_DELETE(m_lpImageViewData->m_pSrcD2DBitmap);
    SAFE_DELETE(m_lpImageViewData);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImageView::SetSrcImage(LPCWSTR lpFile, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = m_lpImageViewData->m_pSrcD2DBitmap->LoadFromFile(lpFile, uDestWidth, uDestHeight);
    if ( retVal )
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImageView::SetSrcImage(IN D2DBitmap *pD2DBitmap)
{
    SAFE_DELETE(m_lpImageViewData->m_pSrcD2DBitmap);
    m_lpImageViewData->m_pSrcD2DBitmap = pD2DBitmap;

    if (NULL != m_lpImageViewData->m_pSrcD2DBitmap)
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }

    return (NULL != m_lpImageViewData->m_pSrcD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImageView::SetSrcImage(IN HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = m_lpImageViewData->m_pSrcD2DBitmap->LoadFromHBITMAP(
        hBitmap,
        uDestWidth,
        uDestHeight);

    if ( retVal )
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImageView::SetSrcImage(UINT resId, HMODULE hModule, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = m_lpImageViewData->m_pSrcD2DBitmap->LoadFromResource(
        resId,
        hModule,
        uDestWidth,
        uDestHeight);

    if ( retVal )
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

void SdkImageView::SetStretchMode(IN IMAGE_STRETCH_MODE stretchMode)
{
    m_lpImageViewData->m_stretchMode = stretchMode;
}

//////////////////////////////////////////////////////////////////////////

void SdkImageView::SetImageMargin(IN FLOAT margin)
{
    if ( margin >= 0 )
    {
        m_lpImageViewData->m_leftMargin    = margin;
        m_lpImageViewData->m_topMargin     = margin;
        m_lpImageViewData->m_rightMargin   = margin;
        m_lpImageViewData->m_bottomMargin  = margin;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImageView::SetImageMargin(IN FLOAT left, IN FLOAT top, IN FLOAT right, IN FLOAT bottom)
{
    if ( left < 0 || top < 0 || right < 0 || bottom < 0 )
    {
        return;
    }

    m_lpImageViewData->m_leftMargin    = left;
    m_lpImageViewData->m_topMargin     = top;
    m_lpImageViewData->m_rightMargin   = right;
    m_lpImageViewData->m_bottomMargin  = bottom;
}

//////////////////////////////////////////////////////////////////////////

void SdkImageView::ClearAssocData()
{
    SdkViewElement::ClearAssocData();

    SAFE_DELETE(m_lpImageViewData->m_pSrcD2DBitmap);
    m_lpImageViewData->m_pSrcD2DBitmap = new D2DBitmap();
}

//////////////////////////////////////////////////////////////////////////

void SdkImageView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F absRc = { 0, 0 };
    GetAbsoluteRect(absRc);

    // Draw view's background.
    OnDrawBackground(pRenderTarget, absRc);

    // Get the instance of SdkD2DTheme to draw source image.
    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

    pD2DTheme->OnDrawBitmap(
        this,
        pRenderTarget,
        m_lpImageViewData->m_pSrcD2DBitmap,
        GetDrawRect(absRc, m_lpImageViewData->m_pSrcD2DBitmap));

    // Draw view's border.
    OnDrawBoarder(pRenderTarget, absRc);
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkImageView::GetDrawRect(const D2D1_RECT_F& viewRc, D2DBitmap *pBitmap)
{
    D2D1_RECT_F retRc = viewRc;
    FLOAT width  = viewRc.right - viewRc.left;
    FLOAT height = viewRc.bottom - viewRc.top;

    ID2D1Bitmap *pID2D1Bitmap = NULL;
    if ( NULL != pBitmap )
    {
        pBitmap->GetD2DBitmap(&pID2D1Bitmap);
    }

    switch (m_lpImageViewData->m_stretchMode)
    {
    case IMAGE_STRETCH_MODE_CENTER:
        {
            if ( NULL != pID2D1Bitmap )
            {
                D2D_SIZE_F bitmapSize =  pID2D1Bitmap->GetSize();
                FLOAT srcWidth = bitmapSize.width;
                FLOAT srcHeight = bitmapSize.height;

                srcWidth = width < srcWidth ? width : srcWidth;
                srcHeight = height < srcHeight ? height : srcHeight;

                retRc.left += (width - srcWidth) / 2;
                retRc.top  += (height - srcHeight) / 2;
                retRc.right = retRc.left + srcWidth;
                retRc.bottom = retRc.top + srcHeight;
            }
        }
        break;

    case IMAGE_STRETCH_MODE_FILL:
        {
            // Target rectangle is same with source rectangle.
        }
        break;

    case IMAGE_STRETCH_MODE_MARGIN:
        {
            D2DRectUtility::InflateD2DRectF(
                retRc,
                -m_lpImageViewData->m_leftMargin,
                -m_lpImageViewData->m_topMargin,
                -m_lpImageViewData->m_rightMargin,
                -m_lpImageViewData->m_bottomMargin);
        }
        break;

    case IMAGE_STRETCH_MODE_FIT:
        {
            if (NULL != pID2D1Bitmap)
            {
                D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();

                FLOAT srcWidth = bitmapSize.width;
                FLOAT srcHeight = bitmapSize.height;

                ConvertToFitMode(width, height, srcWidth, srcHeight);

                retRc.left += (width - srcWidth) / 2;
                retRc.right = retRc.left + srcWidth;
                retRc.top  += (height - srcHeight) / 2;
                retRc.bottom = retRc.top + srcHeight;
            }
        }
        break;

    case IMAGE_STRETCH_MODE_FIT_TOP:
        {
            if ( NULL != pID2D1Bitmap )
            {
                D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();
                FLOAT srcWidth = bitmapSize.width;
                FLOAT srcHeight = bitmapSize.height;

                ConvertToFitMode(width, height, srcWidth, srcHeight);

                retRc.top   += 0;
                retRc.left  += (width - srcWidth) / 2;
                retRc.right  = retRc.left + srcWidth;
                retRc.bottom = retRc.top + srcHeight;
            }
        }
        break;

    case IMAGE_STRETCH_MODE_FIT_LEFT:
        {
            if ( NULL != pID2D1Bitmap )
            {
                D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();
                FLOAT srcWidth = bitmapSize.width;
                FLOAT srcHeight = bitmapSize.height;

                ConvertToFitMode(width, height, srcWidth, srcHeight);

                retRc.top   += (height - srcHeight) / 2;
                retRc.left  += 0;
                retRc.right  = retRc.left + srcWidth;
                retRc.bottom = retRc.top + srcHeight;
            }
        }
        break;

    case IMAGE_STRETCH_MODE_FIT_RIGHT:
        {
            if ( NULL != pID2D1Bitmap )
            {
                D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();
                FLOAT srcWidth = bitmapSize.width;
                FLOAT srcHeight = bitmapSize.height;

                ConvertToFitMode(width, height, srcWidth, srcHeight);

                retRc.right  = retRc.right;
                retRc.top    += (height - srcHeight) / 2;
                retRc.left   = retRc.right - srcWidth;
                retRc.bottom = retRc.top + srcHeight;
            }
        }
        break;

    case IMAGE_STRETCH_MODE_FIT_BOTTOM:
        {
            if ( NULL != pID2D1Bitmap )
            {
                D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();
                FLOAT srcWidth = bitmapSize.width;
                FLOAT srcHeight = bitmapSize.height;

                ConvertToFitMode(width, height, srcWidth, srcHeight);

                retRc.bottom = retRc.bottom;
                retRc.top    = retRc.bottom - srcHeight;
                retRc.left  += (width - srcWidth) / 2;
                retRc.right  = retRc.left + srcWidth;
            }
        }
        break;
    }

    SAFE_RELEASE(pID2D1Bitmap);

    return retRc;
}

//////////////////////////////////////////////////////////////////////////

void SdkImageView::ConvertToFitMode(IN FLOAT destWidth, IN FLOAT destHeight, IN OUT FLOAT& srcWidth, IN OUT FLOAT& srcHeight)
{
    FLOAT fsrcRatio = srcWidth / srcHeight;
    if ( fsrcRatio > 1 )
    {
        srcWidth = destWidth;
        srcHeight = srcWidth / fsrcRatio;

        if (srcHeight > destHeight)
        {
            FLOAT fHeightRatio = srcHeight / destHeight;
            srcHeight = destHeight;
            srcWidth = destWidth / fHeightRatio;
        }
    }
    else
    {
        srcHeight = destHeight;
        srcWidth = srcHeight * fsrcRatio;
        if ( srcWidth > destWidth )
        {
            FLOAT fWidthRatio = srcWidth / destHeight;
            srcWidth = destWidth;
            srcHeight = destHeight / fWidthRatio;
        }
    }
}
