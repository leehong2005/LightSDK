/*!
* @file RatingView.cpp
* 
* @brief This file defines methods to show rating for media files.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/08
*/

#include "RatingView.h"
#include "ImageView.h"
#include "D2DSolidColorBrush.h"
#include "D2DRectUtility.h"

RatingView::RatingView() : m_pPathGeometry(NULL),
                           m_pD2DBrush(NULL),
                           m_pBitmapRenderTarget(NULL),
                           m_isLBDown(FALSE),
                           m_nStarsNumber(5),
                           m_nHMargin(5),
                           m_nVMargin(5),
                           m_nRating(5),
                           m_nRadius(10),
                           m_centerPoint(D2D1::Point2F(20, 20))
{
    ZeroMemory(&m_szPentaclePoints, sizeof(m_szPentaclePoints));
}

//////////////////////////////////////////////////////////////////////////

RatingView::~RatingView()
{
    SAFE_RELEASE(m_pPathGeometry);
    SAFE_RELEASE(m_pBitmapRenderTarget);
    SAFE_DELETE(m_pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void RatingView::SetStarsNumber(IN UINT32 numStars)
{
    m_nStarsNumber = numStars;
    RequestLayout();
}

//////////////////////////////////////////////////////////////////////////

void RatingView::SetRating(IN UINT32 rating)
{
    m_nRating = rating;
}

//////////////////////////////////////////////////////////////////////////

UINT32 RatingView::GetRating() const
{
    return m_nRating;
}

//////////////////////////////////////////////////////////////////////////

void RatingView::SetLayoutInfo(IN const LAYOUTINFO* pLayoutInfo)
{
    ViewElement::SetLayoutInfo(pLayoutInfo);
}

//////////////////////////////////////////////////////////////////////////

void RatingView::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    height = (FLOAT)(2 * (m_nRadius + m_nVMargin));
    width  = (FLOAT)(m_nStarsNumber * (m_nHMargin + 2 * m_nRadius) + m_nHMargin);

    if ( (m_centerPoint.x != (FLOAT)(m_nHMargin + m_nRadius)) ||
         (m_centerPoint.y != height / 2) )
    {
        m_centerPoint.x = (FLOAT)(m_nHMargin + m_nRadius);
        m_centerPoint.y = height / 2;
        SAFE_RELEASE(m_pPathGeometry);
    }

    SAFE_RELEASE(m_pBitmapRenderTarget);

    ViewElement::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void RatingView::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void RatingView::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void RatingView::SetPentacleSize(IN UINT32 radius)
{
    if (m_nRadius != radius)
    {
        m_nRadius = radius;
        SAFE_RELEASE(m_pPathGeometry);
        RequestLayout();
    }
}

//////////////////////////////////////////////////////////////////////////

void RatingView::CreatePentacleGeometry(const D2D1_POINT_2F& center, UINT32 nRadius)
{
    if (NULL != m_pPathGeometry)
    {
        return;
    }

    ID2D1Factory *pD2DFactory = NULL;
    m_pWindow->GetD2DDevices()->GetD2DFactory(&pD2DFactory);

    SAFE_RELEASE(m_pPathGeometry);
    HRESULT hr = pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
    if (SUCCEEDED(hr))
    {
        ID2D1GeometrySink *m_pSink = NULL;
        hr = m_pPathGeometry->Open(&m_pSink);
        if (SUCCEEDED(hr))
        {
            CreatePentaclePoints(center, 180, nRadius);
            m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

            m_pSink->BeginFigure(m_szPentaclePoints[0], D2D1_FIGURE_BEGIN_FILLED);
            m_pSink->AddLine(m_szPentaclePoints[2]);
            m_pSink->AddLine(m_szPentaclePoints[4]);
            m_pSink->AddLine(m_szPentaclePoints[1]);
            m_pSink->AddLine(m_szPentaclePoints[3]);

            m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
            m_pSink->Close();
        }
        SAFE_RELEASE(m_pSink);
    }
}

//////////////////////////////////////////////////////////////////////////

void RatingView::CreatePentaclePoints(IN const D2D1_POINT_2F& center, UINT32 offsetAngle, UINT32 nRadius)
{
    static const FLOAT PI = 3.1415926f;
    static const UINT uSideNumber = 5;
    FLOAT fRadians = 0.0f;
    FLOAT fAngle = (FLOAT)(360.0f / uSideNumber);

    for (UINT i = 0; i < uSideNumber; ++i)
    {
        fRadians = (FLOAT)(PI * ( (offsetAngle + i * fAngle) / 180.0f ));
        m_szPentaclePoints[i].x = center.x - (FLOAT)(nRadius * sin(fRadians));
        m_szPentaclePoints[i].y = center.y + (FLOAT)(nRadius * cos(fRadians));
    }
}

//////////////////////////////////////////////////////////////////////////

void RatingView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    ViewElement::OnDrawItem(pRenderTarget);

    if (NULL == m_pBitmapRenderTarget)
    {
        pRenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(GetWidth(), GetHeight()),
                                                    &m_pBitmapRenderTarget);
    }

    if (NULL == m_pBitmapRenderTarget)
    {
        return;
    }

    if (NULL == m_pD2DBrush)
    {
        m_pD2DBrush = new D2DSolidColorBrush();
        m_pD2DBrush->CreateBrush(pRenderTarget);
    }

    // Get the absolute rectangle of this view.
    D2D1_RECT_F absRc = { GetLeft(), GetTop() };
    GetAbsoluteRect(absRc);
    // Create the pentacle geometry path.
    CreatePentacleGeometry(m_centerPoint, m_nRadius);

    const FLOAT fOffsetX = (FLOAT)(m_nHMargin + 2 * m_nRadius);

    m_pBitmapRenderTarget->BeginDraw();
    m_pBitmapRenderTarget->Clear(D2D1::ColorF(0, 0));

    for (UINT32 i = 0; i < m_nStarsNumber; i++)
    {

        if ( (fOffsetX * (i + 1)) > ViewElement::m_layoutInfo.maxWidth )
        {
            break;
        }

        ID2D1Brush *pD2DBrush = NULL;
        ((D2DSolidColorBrush*)m_pD2DBrush)->SetColor(ColorF((i < m_nRating) ? ColorF::White : ColorF::Gray));
        m_pD2DBrush->GetD2DBrush(&pD2DBrush);

        m_pBitmapRenderTarget->SetTransform(Matrix3x2F::Translation(fOffsetX * i, 0.0f));
        m_pBitmapRenderTarget->FillGeometry(m_pPathGeometry, pD2DBrush);

        SAFE_RELEASE(pD2DBrush);
    }

    m_pBitmapRenderTarget->EndDraw();

    ID2D1Bitmap *pMemBitmap = NULL;
    m_pBitmapRenderTarget->GetBitmap(&pMemBitmap);
    if (NULL != pMemBitmap)
    {
        D2D1_SIZE_F size = pMemBitmap->GetSize();
        D2D1_RECT_F dstRc = { 0, 0, size.width, size.height };
        D2DRectUtility::OffsetD2DRectF(dstRc, absRc.left, absRc.top);
        pRenderTarget->DrawBitmap(pMemBitmap, dstRc);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL RatingView::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = FALSE;

    switch(message)
    {
    case WM_LBUTTONDOWN:
        handled = TRUE;
        break;

    case WM_LBUTTONUP:
        {
            FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
            FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
            BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
            BOOL isTouchOn = IsPtInRect(xPos, yPos);
            if (isPressed && isTouchOn)
            {
                Window *pWindow = GetWindow();
                FLOAT dpiX = 1.0, dpiY = 1.0;
                pWindow->GetDesktopDpiRatio(&dpiX, &dpiY);
                D2D1_POINT_2F desPt = ConvertPoint(D2D1::Point2F(xPos, yPos));
                D2D1_RECT_F viewRc = GetViewRect();

                D2DRectUtility::InflateD2DRectF(viewRc, -(FLOAT)(INT)m_nHMargin, -(FLOAT)(INT)m_nVMargin);
                BOOL isInPentacleRc = D2DRectUtility::PtInD2DRect(viewRc, desPt);
                if (isInPentacleRc)
                {
                    xPos = (desPt.x - viewRc.left) / dpiX;
                    yPos = (desPt.y - viewRc.top) / dpiY;

                    UINT32 nRating = (UINT32)((INT)((xPos - m_nHMargin / dpiX) / (m_nHMargin / dpiX + 2 * m_nRadius))) + 1;
                    m_nRating = (nRating == m_nRating) ? ((0 == m_nRating) ? 1 : (m_nRating - 1)) : nRating;
                    Invalidate();
                }
            }

            handled = isPressed;
        }
        break;
    }

    ViewElement::OnMouseEvent(message, wParam, lParam);

    return handled;
}
