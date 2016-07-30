/*!
* @file ProcessBar.cpp 
* 
* @brief This file defines the class ProcessBar, it's a control for display process.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/01/04
*/

#include "ProgressBar.h"

#define VIEW_MAX_HEIGHT         15              // The progressBar max height.
#define MIN_PROCESS             0.0f            // The progressBar min process.
#define MAX_PROCESS             100.0f          // The progressBar max process.
#define MAX_SCALEVALUE          100             // The progressBar max scale value.


ProgressBar::ProgressBar() : m_pD2DBrush(NULL),
                             m_fCurProgress(0.0f),
                             m_fPreProcess(0.0f),
                             m_processColor(D2D1::ColorF(ColorF::Red))
{
    SetMinHeight(5.0f);
}

//////////////////////////////////////////////////////////////////////////

ProgressBar::~ProgressBar()
{
    SAFE_DELETE(m_pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void ProgressBar::OnDrawItem( ID2D1RenderTarget *pRenderTarget )
{
    ViewElement::OnDrawItem(pRenderTarget);

    if (NULL == m_pD2DBrush)
    {
        m_pD2DBrush = new D2DSolidColorBrush();
        m_pD2DBrush->CreateBrush(pRenderTarget);
    }

    ID2D1Brush *pD2DBrush = NULL;
    m_pD2DBrush->GetD2DBrush(&pD2DBrush);

    if (NULL != pD2DBrush)
    {
        D2D1_RECT_F rc;
        GetAbsoluteRect(rc);

        FLOAT width = rc.right - rc.left;
        FLOAT process = (width / MAX_SCALEVALUE) * m_fCurProgress;
        m_pD2DBrush->SetColor(ColorF(ColorF::Black));
        pRenderTarget->DrawRectangle(rc,pD2DBrush, 3.0f, NULL);

        if (m_fCurProgress <= MAX_SCALEVALUE)
        {
            m_pD2DBrush->SetColor(m_processColor);
            pRenderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top,
                                         rc.left + process, rc.bottom), pD2DBrush);

            m_pD2DBrush->SetColor(D2D1::ColorF(ColorF::White));
            pRenderTarget->FillRectangle(D2D1::RectF(rc.left + process, rc.top,
                                         rc.right, rc.bottom), pD2DBrush);
        }
        else
        {
            m_pD2DBrush->SetColor(m_processColor);
            pRenderTarget->FillRectangle(rc,pD2DBrush);
        }

        m_fPreProcess = m_fCurProgress;
    }

    SAFE_RELEASE(pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void ProgressBar::SetProcess(IN FLOAT process)
{
    if (process >= MIN_PROCESS && process <= MAX_PROCESS)
    {
        m_fCurProgress = process;
    }
}

//////////////////////////////////////////////////////////////////////////

FLOAT ProgressBar::GetProcess()
{
    return m_fCurProgress;
}

//////////////////////////////////////////////////////////////////////////

void ProgressBar::SetStep(IN FLOAT step)
{
    m_fCurProgress += step;
}

//////////////////////////////////////////////////////////////////////////

void ProgressBar::SetProcessColor(IN const D2D1_COLOR_F& color)
{
    m_processColor = color;
}

//////////////////////////////////////////////////////////////////////////

void ProgressBar::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    if (height >= VIEW_MAX_HEIGHT)
    {
        height = VIEW_MAX_HEIGHT;
    }

    ViewElement::SetLayoutInfo(x, y, width, height);
}
