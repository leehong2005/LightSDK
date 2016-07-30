/*!
* @file SpecialView.cpp
* 
* @brief This file defines SpecialView for special functions. This view would clear
*        the drawing rectangle without painting.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/31
*/

#include "SpecialView.h"

SpecialView::SpecialView()
{
}

//////////////////////////////////////////////////////////////////////////

SpecialView::~SpecialView()
{
}

//////////////////////////////////////////////////////////////////////////

void SpecialView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F drawRc = GetDrawingRect();
    pRenderTarget->PushAxisAlignedClip(drawRc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    pRenderTarget->Clear(ColorF(0, 0.01f));
    pRenderTarget->PopAxisAlignedClip();
}
