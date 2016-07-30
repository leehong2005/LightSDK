/*!
* @file SeekBar.cpp
* 
* @brief This file defines the header file of the seekBar.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Zhou_Yuan_Qi.
* @date 2011/01/04.
*/

#include "SeekBar.h"
#include "Resource.h"

#define TOP_MARGIN           0.5f
#define FINAL_TWO            2.0f
#define FINAL_THREE          3.0f
#define FINAL_EIGHT          8.0f
#define TRACK_MARGIN         5.0f
#define MIN_HIGHT            6.0f
#define MAX_HIGHT            30.0f
#define MAX_VALUE            100.f
#define COLOR_R              88.0f
#define COLOR_G              89.0f
#define COLOR_B              91.0f
#define COLOR_D              255.0f
#define COLOR_A              1.0f

//////////////////////////////////////////////////////////////////////////

SeekBar::SeekBar() : m_fStep(MAX_VALUE),
                     m_pSolidBrush(NULL),
                     m_curPos(0),
                     m_radiusX(0),
                     m_radiusY(0),
                     m_sPointX(0),
                     m_sPointY(0),
                     m_offsetX(0),
                     m_offsetY(0),
                     m_topMargin(0),
                     m_isTouchDown(FALSE),
                     m_psChangeHandle(NULL),
                     m_isTouchOnTrack(FALSE),
                     m_isTouchOnSlider(FALSE),
                     m_isProgressVisible(FALSE),
                     m_viewStyle(SEEKBAR_STYLE),
                     m_brushColor(ColorF(0,0)),
                     m_seekBarState(SEEKBARCHANGE_NONE),
                     m_viewOrient(HORIZONTAL_ORIENTATION),
                     m_progressColor(ColorF((COLOR_R / COLOR_D),
                     (COLOR_G / COLOR_D),(COLOR_B / COLOR_D),COLOR_A)),
                     m_trackColor(D2D1::ColorF(ColorF::Silver))
{
    m_pSrcTrackLeft      = new D2DBitmap();
    m_pSrcTrackCenter    = new D2DBitmap();
    m_pSrcTrackRight     = new D2DBitmap();
    m_pSrcProgressLeft   = new D2DBitmap();
    m_pSrcProgressCenter = new D2DBitmap();
    m_pSrcProgressRight  = new D2DBitmap();

    m_pSrcTrackLeft->LoadFromResource(IDB_BAR_BASELEFT,HINST_THISCOMPONENT);
    m_pSrcTrackCenter->LoadFromResource(IDB_BAR_BASECENTER,HINST_THISCOMPONENT);
    m_pSrcTrackRight->LoadFromResource(IDB_BAR_BASERIGHT,HINST_THISCOMPONENT);
    m_pSrcProgressLeft->LoadFromResource(IDB_BAR_BASEL,HINST_THISCOMPONENT);
    m_pSrcProgressCenter->LoadFromResource(IDB_BAR_BASEC,HINST_THISCOMPONENT);
    m_pSrcProgressRight->LoadFromResource(IDB_BAR_BASER,HINST_THISCOMPONENT);

    m_trackCenterPt.y = 0;
    m_trackCenterPt.x = TRACK_MARGIN;

    SetMinHeight(MIN_HIGHT);
    SetViewOrientation(HORIZONTAL_ORIENTATION);
}

/////////////////////////////////////////////////////////////////////////

SeekBar::~SeekBar()
{
    SAFE_DELETE(m_pSolidBrush);
    SAFE_DELETE(m_pSrcTrackLeft);
    SAFE_DELETE(m_pSrcTrackCenter);
    SAFE_DELETE(m_pSrcTrackRight);
    SAFE_DELETE(m_pSrcProgressLeft);
    SAFE_DELETE(m_pSrcProgressCenter);
    SAFE_DELETE(m_pSrcProgressRight);
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::SetCurPos(IN FLOAT fPos)
{
    m_curPos = (fPos < 0) ? 0 : ((fPos > m_fStep) ? m_fStep : fPos);

    if (SEEKBAR_STYLE == m_viewStyle)
    {
        if (m_viewOrient == HORIZONTAL_ORIENTATION)
        {
            InitializeCenterPx();
        }
        else
        {
            InitializeCenterPy();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::SetProgressSrcImage(UINT resLeftId, UINT resCenterId, UINT resRightId)
{
    m_pSrcProgressLeft->LoadFromResource(resLeftId, HINST_THISCOMPONENT);
    m_pSrcProgressCenter->LoadFromResource(resCenterId, HINST_THISCOMPONENT);
    m_pSrcProgressRight->LoadFromResource(resRightId, HINST_THISCOMPONENT);
}

////////////////////////////////////////////////////////////////////////////

void SeekBar::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    switch (m_viewOrient)
    {
    case VERTICAL_ORIENTATION:
        width = (width < 0) ? 0:((width > MAX_HIGHT) ? MAX_HIGHT: width);
        break;

    case HORIZONTAL_ORIENTATION:
        height = (height < 0) ? 0:((height > MAX_HIGHT) ? MAX_HIGHT: height);
        break;
    }

    ViewElement::SetLayoutInfo(x, y, width, height);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void SeekBar::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    if (fChanged)
    {
        if (m_viewOrient == HORIZONTAL_ORIENTATION)
        {
            POINT outPt = { (LONG)left, (LONG)top };
            GetAbsolutePoint(outPt);
            m_trackCenterPt.y  = height / FINAL_TWO;
            m_layoutInfo.width = width;
        }
        else
        {
            POINT outPt = { (LONG)left + (LONG)width, (LONG)top + (LONG)height };
            GetAbsolutePoint(outPt);
            m_trackCenterPt.x   = width / FINAL_TWO;
            m_layoutInfo.height = height;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void SeekBar::SetViewStyle(SEEKBAR_VIEWSTYLE eStyle)
{
    m_viewStyle = eStyle;
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::InitializeCenterPx()
{
    FLOAT width = GetWidth() - FINAL_TWO * TRACK_MARGIN;
    m_trackCenterPt.x = TRACK_MARGIN + (width / (FLOAT)m_fStep) * m_curPos;
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::InitializeCenterPy()
{
    FLOAT width       = GetHeight() - FINAL_TWO * TRACK_MARGIN;
    FLOAT viewbottom  = GetTop() + GetHeight() - TRACK_MARGIN;
    m_trackCenterPt.y = viewbottom - (width / (FLOAT)m_fStep) * m_curPos;
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::SetViewOrientation(SEEKBAR_ORIENT eOrient)
{
    if (m_viewOrient != eOrient)
    {
        m_viewOrient = eOrient;

        switch (m_viewOrient)
        {
        case VERTICAL_ORIENTATION:
            {
                SetViewSize(GetHeight(), GetWidth());
                InitializeCenterPy();
            }
            break;

        case HORIZONTAL_ORIENTATION:
            {
                SetViewSize(GetHeight(), GetWidth());
                InitializeCenterPx();
            }
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::SetProgressColor(IN const ColorF &color)
{
    m_progressColor = color;
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::SetTrackColor(IN const ColorF &color)
{
    m_trackColor = color;
}

////////////////////////////////////////////////////////////////////////

void SeekBar::SetStep(IN FLOAT step)
{
    m_fStep = step;
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::SetChangeHandler(ISeekBarChangeHandler *pSeekBarListener)
{
    m_psChangeHandle = pSeekBarListener;
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::SetProgressVisible(IN BOOL isVisible)
{
    m_isProgressVisible = isVisible;
}

///////////////////////////////////////////////////////////////////////////

void SeekBar::OnProgressChange(FLOAT progress)
{
    if (NULL != m_psChangeHandle)
    {
        m_psChangeHandle->OnProgressChanged(this,progress);
    }
}

///////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    ViewElement::OnDrawItem(pRenderTarget);

    if (NULL == m_pSolidBrush)
    {
        m_pSolidBrush = new D2DSolidColorBrush();
        m_pSolidBrush->CreateBrush(pRenderTarget);
    }

    ID2D1Brush *pD2DBrush = NULL;
    m_pSolidBrush->GetD2DBrush(&pD2DBrush);
    if (NULL != pD2DBrush)
    {
        SetProgressByCurPos();
        D2D1_RECT_F outRc = { 0 };
        GetAbsoluteRect(outRc);
        OnDrawViewBk(outRc, pD2DBrush, pRenderTarget);
        OnDrawTrack(outRc, pD2DBrush, pRenderTarget);
        OnDrawProgress(outRc, pD2DBrush, pRenderTarget);
        OnDrawSlider(outRc, pD2DBrush, pRenderTarget);
        Invalidate();
    }

    SAFE_RELEASE(pD2DBrush);
}

///////////////////////////////////////////////////////////////////////////////////

void SeekBar::SetProgressByCurPos()
{
    if (!((m_isTouchDown && m_isTouchOnSlider) || m_isTouchDown))
    {
        if (PROGRESSBAR_STYLE == m_viewStyle)
        {
            m_isProgressVisible = TRUE;
            FLOAT viewWidth   = m_layoutInfo.width  - FINAL_TWO * TRACK_MARGIN;
            FLOAT viewHeight  = m_layoutInfo.height - FINAL_TWO * TRACK_MARGIN;

            if (0 <= m_curPos && m_curPos <= m_fStep)
            {
                if (m_viewOrient == HORIZONTAL_ORIENTATION)
                {
                    m_trackCenterPt.x  = TRACK_MARGIN;
                    m_trackCenterPt.x += ((viewWidth / (FLOAT)m_fStep) * m_curPos);
                }
                else
                {
                    FLOAT viewBottom  = m_layoutInfo.y + m_layoutInfo.height - TRACK_MARGIN;
                    m_trackCenterPt.y = viewBottom;
                    m_trackCenterPt.y -= ((viewHeight / (FLOAT)m_fStep) * m_curPos);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawViewBk(IN const D2D1_RECT_F &outRc,IN ID2D1Brush *pD2DBrush,
                           IN ID2D1RenderTarget *pRenderTarget)
{
    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        m_radiusX   = m_layoutInfo.height / FINAL_THREE;
        m_radiusY   = m_layoutInfo.height / FINAL_THREE ;
        m_topMargin = (m_layoutInfo.height / 5 )* 2;
    }
    else
    {
        m_radiusY    = m_layoutInfo.width / FINAL_THREE;
        m_radiusX    = m_layoutInfo.width / FINAL_THREE;
        m_topMargin  = m_layoutInfo.width / FINAL_EIGHT;
    }

    (static_cast<D2DSolidColorBrush*>(m_pSolidBrush))->SetColor(m_brushColor);
    pRenderTarget->DrawRectangle(outRc, pD2DBrush);
    pRenderTarget->FillRectangle(outRc, pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawTrackBitmap(IN D2DBitmap *pD2DBitmap,IN ID2D1RenderTarget *pRenderTarget,
                                IN const D2D1_RECT_F &outRc)
{
    if (!pD2DBitmap->HasInitialized())
    {
        pD2DBitmap->Initialize(pRenderTarget);
    }

    ID2D1Bitmap *pSrcID2D1Bitmap = NULL;
    pD2DBitmap->GetD2DBitmap(&pSrcID2D1Bitmap);
    if (NULL != pD2DBitmap)
    {
        pRenderTarget->DrawBitmap(pSrcID2D1Bitmap, outRc);
    }
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawHorTrackBk(IN ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F leftRect =
    {
        m_trackRc.left, m_trackRc.top, m_trackRc.left + 4, m_trackRc.bottom,
    };

    D2D1_RECT_F cretRect = 
    {
        leftRect.right, m_trackRc.top,m_trackRc.right - 4, m_trackRc.bottom,
    };

    D2D1_RECT_F rightRect =
    {
        cretRect.right, m_trackRc.top, m_trackRc.right, m_trackRc.bottom,
    };

    OnDrawTrackBitmap(m_pSrcTrackLeft, pRenderTarget, leftRect);
    OnDrawTrackBitmap(m_pSrcTrackCenter, pRenderTarget, cretRect);
    OnDrawTrackBitmap(m_pSrcTrackRight, pRenderTarget, rightRect);
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawProgressBk(IN const D2D1_RECT_F &outRc,IN ID2D1RenderTarget *pRenderTarget)
{
    FLOAT ofset   = outRc.right - outRc.left;
    FLOAT dxLeft  = (ofset <= 3.0f) ?  0 : 3.f;
    FLOAT dxRight = (ofset <= 3.0f ) ? 0 : 0.4f;

    D2D1_RECT_F leftRc = 
    {
        outRc.left, outRc.top, outRc.left + dxLeft, outRc.bottom,
    };

    D2D1_RECT_F centRc = 
    {
        leftRc.right, outRc.top, outRc.right - dxLeft, outRc.bottom,
    };

    D2D1_RECT_F rightRc = 
    {
        centRc.right - dxRight, outRc.top, outRc.right, outRc.bottom,
    };

    OnDrawTrackBitmap(m_pSrcProgressLeft, pRenderTarget, leftRc);
    OnDrawTrackBitmap(m_pSrcProgressCenter, pRenderTarget, centRc);
    OnDrawTrackBitmap(m_pSrcProgressRight, pRenderTarget, rightRc);
}

///////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawTrack(IN const D2D1_RECT_F &outRc, IN ID2D1Brush *pD2DBrush,
                          IN ID2D1RenderTarget *pRenderTarget)
{
    FLOAT radius = 0;
    m_trackRc    = outRc;

    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        m_trackRc.left   += TRACK_MARGIN ;
        m_trackRc.top    += m_topMargin ;
        m_trackRc.right  -= TRACK_MARGIN;
        m_trackRc.bottom -= m_topMargin;
        radius = (m_trackRc.bottom - m_trackRc.top) / FINAL_TWO;
        OnDrawHorTrackBk(pRenderTarget);
    }
    else
    {
        m_trackRc.left   += m_topMargin;
        m_trackRc.top    += TRACK_MARGIN;
        m_trackRc.right  -= m_topMargin;
        m_trackRc.bottom -= TRACK_MARGIN;
        radius = (m_trackRc.right - m_trackRc.left) / FINAL_TWO;
        D2D1_ROUNDED_RECT rect1 = {m_trackRc, radius, radius};
        (static_cast<D2DSolidColorBrush*>(m_pSolidBrush))
                     ->SetColor(D2D1::ColorF(ColorF::Black));
        pRenderTarget->DrawRoundedRectangle(rect1, pD2DBrush,0.5f);
        pRenderTarget->FillRoundedRectangle(rect1, pD2DBrush);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawSlider(IN const D2D1_RECT_F &outRc, IN ID2D1Brush *pD2DBrush,
                           IN ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F sliderRc = 
    {
        m_trackCenterPt.x - m_radiusX - 10,
        m_trackCenterPt.y - m_radiusY - 15,
        m_trackCenterPt.x + m_radiusX + 10,
        m_trackCenterPt.y + m_radiusY + 15,
    };

    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        D2DRectUtility::OffsetD2DRectF(sliderRc, outRc.left, outRc.top);
    }
    else
    {
        D2DRectUtility::OffsetD2DRectF(sliderRc, outRc.left, 0);
    }

    (static_cast<D2DSolidColorBrush*>(m_pSolidBrush))->SetColor(ColorF(0,0));
    pRenderTarget->DrawRectangle(sliderRc, pD2DBrush);
}

/////////////////////////////////////////////////////////////////////////////////////////

void SeekBar::OnDrawProgress(IN const D2D1_RECT_F &outRc, IN ID2D1Brush *pD2DBrush,
                             IN ID2D1RenderTarget *pRenderTarget)
{
    FLOAT radius = 0;
    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        D2D1_RECT_F  activeRc =
        {
            outRc.left    +  TRACK_MARGIN ,
            outRc.top     +  m_topMargin + TOP_MARGIN,
            outRc.left    +  m_trackCenterPt.x,
            outRc.bottom  -  m_topMargin - TOP_MARGIN 
        };

        OnDrawProgressBk(activeRc,pRenderTarget);
    }
    else
    {
        D2D1_RECT_F  activeRc =
        {
            outRc.left    + m_topMargin + TOP_MARGIN ,
            m_trackCenterPt.y,
            outRc.right   - (m_topMargin + TOP_MARGIN),
            outRc.bottom  -  TRACK_MARGIN 
        };

        radius = (activeRc.right - activeRc.left) / FINAL_TWO ;
        D2D1_ROUNDED_RECT rect = {activeRc, radius, radius};
        (static_cast<D2DSolidColorBrush*>(m_pSolidBrush))->SetColor(m_progressColor);
        pRenderTarget->FillRoundedRectangle(rect,pD2DBrush);
    }
}

//////////////////////////////////////////////////////////////////////////////////////

void SeekBar::MouseDownXCal(IN FLOAT xPos, IN FLOAT maxSize, IN const D2D1_RECT_F &OutRc)
{
    FLOAT fLeftSide    = TRACK_MARGIN;
    FLOAT fViewWidth   = OutRc.right - (2*TRACK_MARGIN + OutRc.left);
    FLOAT fStartX      = m_trackCenterPt.x + OutRc.left ;
    FLOAT fOffsetX     = xPos - fStartX;
    FLOAT fNewCenter   = m_trackCenterPt.x + fOffsetX;
    m_trackCenterPt.x = (fNewCenter < fLeftSide ) ? fLeftSide :
        (fNewCenter > maxSize) ? maxSize :
         m_trackCenterPt.x += fOffsetX ;

    fStartX  = xPos;
    m_curPos = ((m_trackCenterPt.x - TRACK_MARGIN) / (fViewWidth / m_fStep));
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::MouseDownYCal(IN FLOAT yPos, IN FLOAT maxSize, IN const D2D1_RECT_F &rc)
{
    FLOAT fCurWidth  = 0; // The width of current progress.
    FLOAT fWidth     = rc.bottom - (2*TRACK_MARGIN + rc.top);
    FLOAT fBottom    = maxSize - fWidth; 
    FLOAT fStartY    = m_trackCenterPt.y;
    FLOAT fOffsetY   = yPos - fStartY;
    FLOAT fNewCenter = m_trackCenterPt.y + fOffsetY;
    
    m_trackCenterPt.y = (fNewCenter < fBottom) ? fBottom :
        (fNewCenter > maxSize) ? maxSize :
        m_trackCenterPt.y += fOffsetY;

    fStartY   = yPos;
    fCurWidth = rc.bottom - m_trackCenterPt.y - TRACK_MARGIN;
    m_curPos  = (fCurWidth / (fWidth / m_fStep));
}

/////////////////////////////////////////////////////////////////////////////

void SeekBar::MouseMoveXCal( IN FLOAT xPos, IN FLOAT maxSize, IN const D2D1_RECT_F &OutRc )
{
    if ( (xPos >= OutRc.left + TRACK_MARGIN) && (xPos <= OutRc.right - TRACK_MARGIN) )
    {
        FLOAT  fCurWidth   = 0;
        FLOAT  fViewWidth  = OutRc.right - (2*TRACK_MARGIN + OutRc.left);
        FLOAT  fLeftSide   = TRACK_MARGIN;
        m_offsetX = xPos   - m_sPointX;
        FLOAT  fNewCenter  = m_trackCenterPt.x + m_offsetX;

        m_trackCenterPt.x = (fNewCenter < fLeftSide ) ? fLeftSide :
            (fNewCenter > maxSize) ? maxSize :
            m_trackCenterPt.x += m_offsetX ;
        m_seekBarState = SEEKBARCHANGE;

        fCurWidth = m_trackCenterPt.x - TRACK_MARGIN;
        m_curPos  = (fCurWidth / (fViewWidth / m_fStep));
        //return the current progress.
        OnProgressChange(m_curPos);
        Invalidate();
        m_sPointX = xPos;
    }
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::MouseMoveYCal( IN FLOAT yPos, IN FLOAT maxSize, IN const D2D1_RECT_F &OutRc)
{
    if ( (yPos >= OutRc.top + TRACK_MARGIN) && (yPos <= OutRc.bottom - TRACK_MARGIN) )
    {
        FLOAT fCurWidth  = 0;
        FLOAT fViewWidth = OutRc.bottom - (2*TRACK_MARGIN + OutRc.top);
        FLOAT fBottom    = maxSize - fViewWidth;
        m_offsetY = yPos - m_sPointY;
        FLOAT fNewCenter = m_trackCenterPt.y + m_offsetY;

        m_trackCenterPt.y = (fNewCenter < fBottom) ? fBottom :
                            (fNewCenter > maxSize) ? maxSize :
                             m_trackCenterPt.y += m_offsetY;
        m_seekBarState = SEEKBARCHANGE;

        fCurWidth = OutRc.bottom - m_trackCenterPt.y - TRACK_MARGIN;
        m_curPos = (fCurWidth / (fViewWidth / m_fStep));
        //return the current progress.
        OnProgressChange(m_curPos);
        Invalidate();
        m_sPointY = yPos;
    }
}

//////////////////////////////////////////////////////////////////////////////////////

void SeekBar::OnMouseDownHandler(IN FLOAT xPos,IN FLOAT yPos, IN FLOAT maxSize,
                                 IN const D2D1_RECT_F &rc)
{
    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        MouseDownXCal(xPos,maxSize,rc);
        OnProgressChange(m_curPos);
        Invalidate();
        //get the start point.
        m_sPointX = m_trackCenterPt.x + rc.left;
    }
    else
    {
        MouseDownYCal(yPos,maxSize,rc);
        OnProgressChange(m_curPos);
        Invalidate();
        m_sPointY = m_trackCenterPt.y;
    }
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::OnMouseMoveHandler(IN FLOAT xPos, IN FLOAT yPos, IN FLOAT maxSize,
                                 IN const D2D1_RECT_F &rc)
{
    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        MouseMoveXCal(xPos, maxSize, rc);
    }
    else
    {
        MouseMoveYCal(yPos, maxSize, rc);
    }
}

/////////////////////////////////////////////////////////////////////////////

void SeekBar::GetTrackRect(IN FLOAT xPos,IN FLOAT yPos, IN D2D1_RECT_F OutRc)
{
    D2D1_RECT_F tickerRc = 
    {
        m_trackCenterPt.x - m_radiusX - 10,
        m_trackCenterPt.y - m_radiusY - 15,
        m_trackCenterPt.x + m_radiusX + 10,
        m_trackCenterPt.y + m_radiusY + 15,
    };

    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        D2DRectUtility::OffsetD2DRectF(tickerRc, OutRc.left, OutRc.top);
    }
    else
    {
        D2DRectUtility::OffsetD2DRectF(tickerRc, OutRc.left, 0);
    }

    if ( (xPos >= tickerRc.left ) && (xPos <= (tickerRc.right)) &&
         (yPos >= tickerRc.top) && (yPos <= (tickerRc.bottom)) )
    {
        m_isTouchOnSlider = TRUE;
    }
    if ( (xPos>=  m_trackRc.left) &&(xPos <=  m_trackRc.right) &&
         (yPos>= m_trackRc.top) &&(yPos <= m_trackRc.bottom) )
    {
        m_isTouchOnTrack = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void SeekBar::GetMaxSize(IN const D2D1_RECT_F &outRc,FLOAT &size )
{
    if (m_viewOrient == HORIZONTAL_ORIENTATION)
    {
        size = outRc.right - (TRACK_MARGIN + outRc.left);
    }
    else
    {
        size = outRc.bottom - TRACK_MARGIN;
    }
}

///////////////////////////////////////////////////////////////////////

void SeekBar::GetConvertPoint(IN D2D1_RECT_F outRc, IN FLOAT &xPos, IN FLOAT &yPos)
{
    Window *pWindow = GetWindow();
    FLOAT dpiX = 1.0f, dpiY = 1.0f;
    pWindow->GetDesktopDpiRatio(&dpiX, &dpiY);
    D2D1_POINT_2F desPt = ConvertPoint(D2D1::Point2F(xPos, yPos));
    D2D1_RECT_F viewRc  = GetViewRect();
    xPos  = (desPt.x - viewRc.left) / dpiX;
    yPos  = (desPt.y - viewRc.top)  / dpiY;
    xPos  += outRc.left;
    yPos  += outRc.top;
}

///////////////////////////////////////////////////////////////////////

BOOL SeekBar::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isTouchOn    = IsPtInRect(xPos, yPos);
    BOOL handled      = FALSE;
    FLOAT maxSize     = 0;
    D2D1_RECT_F outRc = { 0 };

    GetAbsoluteRect(outRc);
    GetConvertPoint(outRc, xPos, yPos);
    GetTrackRect(xPos, yPos, outRc);
    GetMaxSize(outRc, maxSize);

    switch(message)
    {
    case WM_LBUTTONDOWN:
        {
            if (isTouchOn)
            {
                AddFlag(VIEW_STATE_PRESSED);
                m_isTouchDown  = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
                m_seekBarState = SEEKBARCHANGE_BEGIN;
                OnMouseDownHandler(xPos, yPos, maxSize, outRc);
                handled = isTouchOn;
            }
        }
        break;

    case WM_MOUSEMOVE:
        {
            {
                if (m_isTouchDown && m_isTouchOnSlider)
                {
                    xPos = (FLOAT)GET_X_LPARAM(lParam);
                    yPos = (FLOAT)GET_Y_LPARAM(lParam);
                    GetConvertPoint(outRc, xPos, yPos);
                    OnMouseMoveHandler(xPos, yPos, maxSize, outRc);
                }
            }
            handled = TRUE;
        }
        break;

    case WM_LBUTTONUP:
        {
            if ( m_isTouchDown)
            {
                m_isTouchOnTrack  = FALSE;
                m_offsetX         = 0;
                RemoveFlag(VIEW_STATE_PRESSED);
                m_seekBarState    = SEEKBARCHANGE_END;
                OnProgressChange(m_curPos);
                m_isTouchDown     = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
                m_isTouchOnSlider = FALSE;
                Invalidate();
                handled = TRUE;
            }
        }
        break;
    }
    return handled;
}

//////////////////////////////////////////////////////////////////////////

SEEKBARCHAANGE_STATE SeekBar::GetSeekBarState()
{
    return m_seekBarState;
}