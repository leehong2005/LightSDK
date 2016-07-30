/*!
* @file OverlapView.cpp
/*!
* @file  OverlapView.cpp
* 
* @brief This file overlap view class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Jianbin
* @date 2011/01/04
*/

#include "OverlapView.h"
#include "ViewLayout.h"

#define RATIOX     20 / 175
#define RATIOY     20 / 135

typedef enum _OVWELAP_LAYOUTINFO
{
    OVERLAP_MAXWIDTH  = 600,                // The max width for overlap view.
    OVERLAP_MAXHEIGHT = 600,                // The max height for overlap view.
    OVERLAP_MINWIDTH  = 20,                 // The min width for overlap view.
    OVERLAP_MINHEIGHT = 20,                 // The min height for overlap view.

} OVERLAP_LAYOUTINFO;

// None style's transform.
const OVERLAP_TRANSFORMINFO g_szNoneOverlayTransforms[] = 
{
    {  0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    {  0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    {  0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
};

// Default first style's transform.
const OVERLAP_TRANSFORMINFO g_szFristOverlayTransforms[] = 
{
    {   0.0f, -5.0f, 1.0f, 1.0f, -5, 0,  -8.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    {  15.0f, 10.0f, 1.0f, 1.0f, -5, 0,  10.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { -10.0f,  5.0f, 1.0f, 1.0f, -5, 0,   0.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
};

// Default second style's transform.
const OVERLAP_TRANSFORMINFO g_szSecondOverlayTransforms[] = 
{
    {   0.0f,  0.0f, 1.0f, 1.0f, 0, 6, 0.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    {   0.0f,  0.0f, 1.0f, 1.0f, 0, 6, 0.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    {   0.0f,  0.0f, 1.0f, 1.0f, 0, 6, 0.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
};

const OVERLAP_TRANSFORMINFO g_szThirdOverlapTransforms[] = 
{
    {  10.0f, 0.0f, 1.0f, 1.0f, 0, 0, 10.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    {   5.0f, 0.0f, 1.0f, 1.0f, 0, 0,  5.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    {   0.0f, 0.0f, 1.0f, 1.0f, 0, 0, -3.0f, 1.0f, { 0, 0 }, { 0, 0 }, { 0, 0 } },
};

//////////////////////////////////////////////////////////////////////////

OverlapView::OverlapView():m_pBitmapRenderTarget(NULL),
                           m_overlayStyle(OVERLAP_STYLE_SECOND),
                           m_leftMargin(0.0f),
                           m_topMaragin(0.0f),
                           m_rightMargin(0.0f),
                           m_bottomMargin(0.0f)
{
    ZeroMemory(&m_szOverlapTransforms, sizeof(OVERLAP_TRANSFORMINFO) * OVERLAP_TRANSFORMINFO_COUNT);
    ZeroMemory(&m_preSize, sizeof(D2D1_SIZE_F));
    SetStyle(m_overlayStyle);
    //SetupOverlapLayout();
}

//////////////////////////////////////////////////////////////////////////

OverlapView::~OverlapView()
{
    Clear();
    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::Clear()
{
    for each(D2DBitmap *pBitmap in m_vctBitmaps)
    {
        SAFE_DELETE(pBitmap);
    }

    m_vctBitmaps.clear();
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetStyle(OVERLAP_STYLE style)
{
    m_overlayStyle = style;

    switch (m_overlayStyle)
    {
    case OVERLAP_STYLE_NONE:
        SetupNoneStyle();
        RequestLayout();
        break;

    case OVERLAP_STYLE_FIRST:
        SetupFirstStyle();
        RequestLayout();
        break;

    case OVERLAP_STYLE_SECOND:
        SetupSecondStyle();
        RequestLayout();
        break;

    case OVERLAP_STYLE_THIRD:
        SetupThirdStyle();
        RequestLayout();
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL OverlapView::RemoveImage(UINT32 index)
{
    if (index < m_vctBitmaps.size())
    {
        SAFE_DELETE(m_vctBitmaps[index]);
        m_vctBitmaps.erase(m_vctBitmaps.begin() + index);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL OverlapView::SetImage(IN LPCWSTR lpfileName, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = FALSE;

    if (::PathFileExists(lpfileName))
    {
        D2DBitmap *pBitmap = new D2DBitmap();
        m_vctBitmaps.push_back(pBitmap);
        retVal = pBitmap->LoadFromFile(lpfileName, uDestWidth, uDestHeight);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL OverlapView::SetImage(IN HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = FALSE;

    if (NULL != hBitmap)
    {
        D2DBitmap *pBitmap = new D2DBitmap();
        m_vctBitmaps.push_back(pBitmap);
        retVal = pBitmap->LoadFromHBITMPA(hBitmap, uDestWidth, uDestHeight);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL OverlapView::SetImage(IN UINT32 uResId, IN HMODULE hModule, UINT32 uDestWidth, UINT32 uDestHeight)
{
    D2DBitmap *pBitmap = new D2DBitmap();
    m_vctBitmaps.push_back(pBitmap);

    return pBitmap->LoadFromResource(uResId, hModule, uDestWidth, uDestHeight);
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetImageMargin(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    m_leftMargin    = left;
    m_topMaragin    = top;
    m_rightMargin   = right;
    m_bottomMargin  = bottom;
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetImageMargin(FLOAT margin)
{
    m_leftMargin    = margin;
    m_topMaragin    = margin;
    m_rightMargin   = margin;
    m_bottomMargin  = margin;
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    if (!CreateCompatibleTarget(pRenderTarget))
    {
        return;
    }

    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    m_pBitmapRenderTarget->BeginDraw();
    m_pBitmapRenderTarget->Clear(D2D1::ColorF(0, 0));
    OnDrawOverlap(pRenderTarget, rc);
    m_pBitmapRenderTarget->EndDraw();

    ID2D1Bitmap *pD2DMemBitmap = NULL;
    m_pBitmapRenderTarget->GetBitmap(&pD2DMemBitmap);
    if (NULL != pD2DMemBitmap)
    {
        D2D1_SIZE_F size = m_pBitmapRenderTarget->GetSize();
        D2D1_RECT_F rcBmp = { 0, 0, size.width, size.height };
        FLOAT offsetX = (size.width - GetWidth()) / 2;
        FLOAT offsetY = (size.height - GetHeight()) / 2;
        D2DRectUtility::OffsetD2DRectF(rcBmp, rc.left - offsetX, rc.top - offsetY);
        D2DRectUtility::InflateD2DRectF(rcBmp, -m_leftMargin, -m_topMaragin, -m_rightMargin, -m_bottomMargin);
        pRenderTarget->DrawBitmap(pD2DMemBitmap, rcBmp);
    }

    SAFE_RELEASE(pD2DMemBitmap);
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    ViewElement::OnLayout(fChanged, left, top, width, height);

    int size = ARRAYSIZE(m_szOverlapTransforms);
    for (int i = 0; i < size; ++i)
    {
        OVERLAP_TRANSFORMINFO &transform = m_szOverlapTransforms[i];
        transform.rotateCenterPt.x = width;
        transform.rotateCenterPt.y = height;

        transform.skewCenterPt.x = width;
        transform.skewCenterPt.y = height;

        if (OVERLAP_STYLE_SECOND == m_overlayStyle)
        {
            transform.dx = (width * RATIOX) * ((size - 1) - i);
            transform.dy = (height * RATIOY) * i;
        }

        transform.matrixTranform = CombineMatrix(transform);
    }

    if ( (width != m_preSize.width) || (height != m_preSize.height) )
    {
        m_preSize.width  = width;
        m_preSize.height = height;
        SAFE_RELEASE(m_pBitmapRenderTarget);
    }
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    ViewElement::OnD2DDeviceStateChange(pDevice, stateVal);

    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    ViewElement::OnD2DDeviceResize(pDevice, stateVal);

    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::OnDrawOverlap(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &rect)
{
    UNREFERENCED_PARAMETER(rect);

    if (OVERLAP_STYLE_NONE == m_overlayStyle)
    {
        ViewElement::OnDrawItem(pRenderTarget);
    }

    D2D1_SIZE_F sizeRT = m_pBitmapRenderTarget->GetSize();
    FLOAT viewWidth = GetWidth();
    FLOAT wiewHeight = GetHeight();
    FLOAT offsetX = (sizeRT.width - viewWidth) / 2;
    FLOAT offsetY = (sizeRT.height - wiewHeight) / 2;
    D2D1_RECT_F dstRc = { 0, 0, viewWidth, wiewHeight };

    D2DRectUtility::OffsetD2DRectF(dstRc, offsetX, offsetY);

    int size1 = ARRAYSIZE(m_szOverlapTransforms);
    int size = min(size1, (int)m_vctBitmaps.size());
    size = (OVERLAP_STYLE_NONE == m_overlayStyle) ? min(size, 1) : size;

    D2D1_RECT_F bmpRc = SetBitmapRect(dstRc);

    for (int i = 0; i < size; ++i)
    {
        ID2D1Bitmap *pD2DBitmap = NULL;
        D2DBitmap *pBitmap = m_vctBitmaps[i];
        if (!pBitmap->HasInitialized())
        {
            pBitmap->Initialize(pRenderTarget);
        }

        pBitmap->GetD2DBitmap(&pD2DBitmap);
        if (NULL != pD2DBitmap)
        {
            OVERLAP_TRANSFORMINFO &transform = m_szOverlapTransforms[i + (size1 - size)];
            m_pBitmapRenderTarget->SetTransform(transform.matrixTranform);
            m_pBitmapRenderTarget->DrawBitmap(pD2DBitmap, bmpRc, transform.opacity);
        }

        SAFE_RELEASE(pD2DBitmap);
    }
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F OverlapView::CombineMatrix(IN const OVERLAP_TRANSFORMINFO& transform)
{
    Matrix3x2F matTrans  = Matrix3x2F::Translation(transform.dx, transform.dy);
    Matrix3x2F matScale  = Matrix3x2F::Scale(transform.sx, transform.sy, transform.scaleCenterPt);
    Matrix3x2F matSkew   = Matrix3x2F::Skew(transform.skewx, transform.skewy, transform.skewCenterPt);
    Matrix3x2F matRotate = Matrix3x2F::Rotation(transform.angle, transform.rotateCenterPt);

    return matTrans * matScale * matRotate * matSkew;
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetupNoneStyle()
{
    int size1 = ARRAYSIZE(m_szOverlapTransforms);
    int size2 = ARRAYSIZE(g_szNoneOverlayTransforms);

    for (int i = 0; i < size1 && i < size2; ++i)
    {
        OVERLAP_TRANSFORMINFO &transform = m_szOverlapTransforms[i];
        transform = g_szNoneOverlayTransforms[i];
        transform.matrixTranform = Matrix3x2F::Identity();
    }
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetupFirstStyle()
{
    int size1 = ARRAYSIZE(m_szOverlapTransforms);
    int size2 = ARRAYSIZE(g_szFristOverlayTransforms);

    for (int i = 0; i < size1 && i < size2; ++i)
    {
        OVERLAP_TRANSFORMINFO &transform = m_szOverlapTransforms[i];
        transform = g_szFristOverlayTransforms[i];
        transform.matrixTranform = CombineMatrix(transform);
    }
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetupSecondStyle()
{
    int size1 = ARRAYSIZE(m_szOverlapTransforms);
    int size2 = ARRAYSIZE(g_szSecondOverlayTransforms);
    int size = min(size1, size2);

    D2D1_RECT_F rect = { 0 };
    GetAbsoluteRect(rect);
    FLOAT width = rect.right - rect.left;
    FLOAT height = rect.bottom - rect.top;

    for (int i = 0; i < size1 && i < size2; ++i)
    {
        OVERLAP_TRANSFORMINFO &transform = m_szOverlapTransforms[i];
        transform = g_szSecondOverlayTransforms[i];
        transform.dx = (width * RATIOX) * ((size - 1) - i);
        transform.dy = (height * RATIOY) * i;
        transform.matrixTranform = CombineMatrix(transform);
    }
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetupThirdStyle()
{
    int size1 = ARRAYSIZE(m_szOverlapTransforms);
    int size2 = ARRAYSIZE(g_szThirdOverlapTransforms);

    for (int i = 0; i < size1 && i < size2; ++i)
    {
        OVERLAP_TRANSFORMINFO &transform = m_szOverlapTransforms[i];
        transform = g_szThirdOverlapTransforms[i];
        transform.matrixTranform = CombineMatrix(transform);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL OverlapView::CreateCompatibleTarget(ID2D1RenderTarget *pRenderTarget)
{
    if (NULL == m_pBitmapRenderTarget)
    {
        FLOAT width  = GetWidth();
        FLOAT height = GetHeight();
        FLOAT val = MAX(width, height);

        pRenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(val * 2, val * 2),
                                                    &m_pBitmapRenderTarget);
    }

    return (NULL != m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void OverlapView::SetupOverlapLayout()
{
    SetMaxWidth(OVERLAP_MAXWIDTH);
    SetMaxHeight(OVERLAP_MAXHEIGHT);
    SetMinWidth(OVERLAP_MINWIDTH);
    SetMinHeight(OVERLAP_MINHEIGHT);
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F OverlapView::SetBitmapRect(const D2D1_RECT_F &rect)
{
    static const FLOAT PI = 3.1415926f;
    FLOAT fRadian = 0;
    D2D1_RECT_F destRc = rect;
    int size = ARRAYSIZE(m_szOverlapTransforms);

    switch (m_overlayStyle)
    {
    case OVERLAP_STYLE_SECOND:
        {
            OVERLAP_TRANSFORMINFO &transform1 = m_szOverlapTransforms[0];
            OVERLAP_TRANSFORMINFO &transform2 = m_szOverlapTransforms[size - 1];
            fRadian = PI * transform1.skewy / 180.0f;
            destRc.top += (destRc.right - destRc.left) * tan(fRadian) / 4;
            destRc.bottom -= (transform2.dy + (destRc.right - destRc.left) * tan(fRadian) / 4);
            destRc.right -= transform1.dx;
        }
        break;

    }

    return destRc;
}