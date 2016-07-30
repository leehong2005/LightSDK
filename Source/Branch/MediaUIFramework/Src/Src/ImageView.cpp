/*!
* @file  ImageView.cpp
* 
* @brief This file defines the function of the ImageView module,
*        which can convert image into D2Dbitmap.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Cui Shuyan
* @date 2010/12/31
*/

#include "ImageView.h"

ImageView::ImageView() : m_pSrcD2DBitmap(new D2DBitmap()),
                         m_stretchMode(IMAGE_STRETCH_MODE_CENTER),
                         m_leftMargin(0.0f),
                         m_topMargin(0.0f),
                         m_rightMargin(0.0f),
                         m_bottomMargin(0.0f)
{
    SetClickable(FALSE);
}

//////////////////////////////////////////////////////////////////////////

ImageView::~ImageView()
{
    SAFE_DELETE(m_pSrcD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageView::SetSrcImage(LPCWSTR lpFile, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = m_pSrcD2DBitmap->LoadFromFile(lpFile, uDestWidth, uDestHeight);
    if (retVal)
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageView::SetSrcImage(IN D2DBitmap *pD2DBitmap)
{
    SAFE_DELETE(m_pSrcD2DBitmap);
    m_pSrcD2DBitmap = pD2DBitmap;
    if (NULL != m_pSrcD2DBitmap)
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }
    return (NULL != m_pSrcD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageView::SetSrcImage(IN HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = m_pSrcD2DBitmap->LoadFromHBITMPA(hBitmap, uDestWidth, uDestHeight);
    if (retVal)
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageView::SetSrcImage(UINT resId, HMODULE hModule, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = m_pSrcD2DBitmap->LoadFromResource(resId, hModule, uDestWidth, uDestHeight);
    if (retVal)
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

void ImageView::SetImageStretchMode(IN IMAGE_STRETCH_MODE stretchMode)
{
    m_stretchMode = stretchMode;
}

//////////////////////////////////////////////////////////////////////////

void ImageView::SetSrcImageMargin(IN FLOAT margin)
{
    if (margin >= 0)
    {
        m_leftMargin    = margin;
        m_topMargin     = margin;
        m_rightMargin   = margin;
        m_bottomMargin  = margin;
    }
}

//////////////////////////////////////////////////////////////////////////

void ImageView::SetSrcImageMargin(IN FLOAT left, IN FLOAT top, IN FLOAT right, IN FLOAT bottom)
{
    if (left < 0 || top < 0 || right < 0 || bottom < 0)
    {
        return;
    }
    m_leftMargin    = left;
    m_topMargin     = top;
    m_rightMargin   = right;
    m_bottomMargin  = bottom;
}

//////////////////////////////////////////////////////////////////////////

void ImageView::ClearAssocData()
{
    SAFE_DELETE(m_pSrcD2DBitmap);

    ViewElement::ClearAssocData();
}

//////////////////////////////////////////////////////////////////////////

void ImageView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    //ViewElement::OnDrawItem(pRenderTarget);

    D2D1_RECT_F absRc = { 0, 0 };
    GetAbsoluteRect(absRc);

    // Draw view's background.
    OnDrawBackground(pRenderTarget, absRc);

    if (NULL == m_pSrcD2DBitmap)
    {
        m_pSrcD2DBitmap = new D2DBitmap();
    }

    // Draw source image.
    if (!m_pSrcD2DBitmap->HasInitialized())
    {
        m_pSrcD2DBitmap->Initialize(pRenderTarget);
    }

    ID2D1Bitmap *pID2D1Bitmap = NULL;
    m_pSrcD2DBitmap->GetD2DBitmap(&pID2D1Bitmap);
    if (NULL != pID2D1Bitmap && NULL != pRenderTarget)
    {
        pRenderTarget->DrawBitmap(pID2D1Bitmap, GetDrawRect(absRc, pID2D1Bitmap));
    }

    SAFE_RELEASE(pID2D1Bitmap);

    // Draw view's border.
    OnDrawBoarder(pRenderTarget, absRc);
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F ImageView::GetDrawRect(const D2D1_RECT_F& viewRc, ID2D1Bitmap *pID2D1Bitmap)
{
    D2D1_RECT_F retRc = viewRc;

    FLOAT width = viewRc.right - viewRc.left;
    FLOAT height = viewRc.bottom - viewRc.top;

    switch (m_stretchMode)
    {
    case IMAGE_STRETCH_MODE_CENTER:
        {
            if (NULL != pID2D1Bitmap)
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
        // Target rectangle is same with source rectangle.
        break;

    case IMAGE_STRETCH_MODE_MARGIN:
        {
            D2DRectUtility::InflateD2DRectF(retRc, -m_leftMargin, -m_topMargin, -m_rightMargin, -m_bottomMargin);
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
                retRc.top += (height - srcHeight) / 2;
                retRc.bottom = retRc.top + srcHeight;

                /*FLOAT fsrcRatio = srcWidth / srcHeight;
                if (fsrcRatio > 1)
                {
                    srcWidth = width;
                    srcHeight = srcWidth / fsrcRatio;

                    if (srcHeight > height)
                    {
                        FLOAT fHeightRatio = srcHeight / height;
                        srcHeight = height;
                        srcWidth = width / fHeightRatio;
                        retRc.left += (width - srcWidth) / 2;
                        retRc.right = retRc.left + srcWidth;
                    }
                    else
                    {
                        retRc.top += (height - srcHeight) / 2;
                        retRc.bottom = retRc.top + srcHeight;
                    }
                }
                else
                {
                    srcHeight = height;
                    srcWidth = srcHeight * fsrcRatio;

                    if (srcWidth > width)
                    {
                        FLOAT fWidthRatio = srcWidth / height;
                        srcWidth = width;
                        srcHeight = height / fWidthRatio;

                        retRc.top += (height - srcHeight) / 2;
                        retRc.bottom = retRc.top + srcHeight;
                    }
                    else
                    {
                        retRc.left += (width - srcWidth) / 2;
                        retRc.right = retRc.left + srcWidth;
                    }
                }*/
            }
        }
        break;

    case IMAGE_STRETCH_MODE_FIT_ONTOP:
        {
            if (NULL != pID2D1Bitmap)
            {
                D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();

                FLOAT srcWidth = bitmapSize.width;
                FLOAT srcHeight = bitmapSize.height;

                ConvertToFitMode(width, height, srcWidth, srcHeight);

                retRc.top += 0;
                retRc.left += (width - srcWidth) / 2;
                retRc.right = retRc.left + srcWidth;
                retRc.bottom = retRc.top + srcHeight;
            }
        }
        break;

    case IMAGE_STRETCH_MODE_FIT_ONLEFT:
    case IMAGE_STRETCH_MODE_FIT_ONRIGHT:
    case IMAGE_STRETCH_MODE_FIT_ONBOTTOM:
        {
            
        }
        break;

    }

    return retRc;
}

//////////////////////////////////////////////////////////////////////////

void ImageView::ConvertToFitMode(IN FLOAT destWidth,IN FLOAT destHeight, IN OUT FLOAT& srcWidth, IN OUT FLOAT& srcHeight)
{
    FLOAT fsrcRatio = srcWidth / srcHeight;
    if (fsrcRatio > 1)
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

        if (srcWidth > destWidth)
        {
            FLOAT fWidthRatio = srcWidth / destHeight;
            srcWidth = destWidth;
            srcHeight = destHeight / fWidthRatio;
        }
    }
}
