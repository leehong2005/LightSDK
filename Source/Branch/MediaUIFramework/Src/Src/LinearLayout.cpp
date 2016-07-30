/*!
* @file LinearLayout.cpp.
* 
* @brief This file defines the linear layout 
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Sun Ning, Cao Chen, Liu Qiao
* @date 2011/01/06
*/

#include "LinearLayout.h"

#define PADDING_DEFAULT_VALUE   4

//////////////////////////////////////////////////////////////////////////

LinearLayout::LinearLayout(): m_nOrientation(ORIENTATION_VERTICAL),
                              m_nStyle(LAYOUT_STYLE_NOTCENTER | LAYOUT_STYLE_FORWARD),
                              m_fOffsetX(0),
                              m_fOffsetY(0),
                              m_fTempParentHeight(0),
                              m_fTempParentWidth(0)
{
    //Initialize paddings value
    m_Padding.paddingLeft   = PADDING_DEFAULT_VALUE;
    m_Padding.paddingTop    = PADDING_DEFAULT_VALUE;
    m_Padding.paddingRight  = PADDING_DEFAULT_VALUE;
    m_Padding.paddingBottom = PADDING_DEFAULT_VALUE;
}

//////////////////////////////////////////////////////////////////////////

LinearLayout::~LinearLayout()
{

}

//////////////////////////////////////////////////////////////////////////

void LinearLayout::OnLayout(BOOL ifChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    LAYOUTINFO layoutInfo = {0};

    if((INT32)m_vctChildren.size() > 0)
    {
        m_vctChildren[0]->GetLayoutInfo(&layoutInfo);

        switch(m_nOrientation)
        {
        case ORIENTATION_VERTICAL:
            {
                if (m_nStyle & LAYOUT_STYLE_FORWARD)
                {
                    FLOAT tempTop    = MAX(m_Padding.paddingTop, layoutInfo.topMargin);
                    m_fTempParentHeight = height - tempTop - m_Padding.paddingBottom
                                           + layoutInfo.topMargin;
                }
                else if (m_nStyle & LAYOUT_STYLE_BACKWORD)
                {
                    FLOAT tempBottom = MAX(m_Padding.paddingBottom, layoutInfo.bottomMargin);
                    m_fTempParentHeight = height - tempBottom - m_Padding.paddingTop
                                           + layoutInfo.bottomMargin;
                }
            }
            break;
        case ORIENTATION_HORIZONTAL:
            {
                if (m_nStyle & LAYOUT_STYLE_FORWARD)
                {
                    FLOAT tempLeft   = MAX(m_Padding.paddingLeft, layoutInfo.leftMargin);
                    m_fTempParentWidth = width - tempLeft - m_Padding.paddingRight
                                          + layoutInfo.leftMargin;
                }
                else if (m_nStyle & LAYOUT_STYLE_BACKWORD)
                {
                    FLOAT tempRight  = MAX(m_Padding.paddingRight, layoutInfo.rightMargin);
                    m_fTempParentWidth = width - tempRight - m_Padding.paddingLeft
                                          + layoutInfo.rightMargin;
                }
            }
            break;
        default:
            break;
        }

        for (vector<ViewElement*>::iterator itor = m_vctChildren.begin();
            itor != m_vctChildren.end();
            ++itor)
        {
            //Get LayoutInfo.
            (*itor)->GetLayoutInfo(&layoutInfo);
            //Get children's layoutInfo.
            MeasureChildren(&layoutInfo);
            (*itor)->SetLayoutInfo(&layoutInfo); 
        }
    }

    ViewLayout::OnLayout(ifChanged, left, top, width, height);
}

//////////////////////////////////////////////////////////////////////////

void LinearLayout::SetOrientation(ORIENTATION orientation)
{
    m_nOrientation = orientation;
}

//////////////////////////////////////////////////////////////////////////

void LinearLayout::SetStyle(INT32 nStyle)
{
    //if style is forward or backward
    if(nStyle & LAYOUT_STYLE_FORWARD)
    {
        m_nStyle = m_nStyle &~LAYOUT_STYLE_BACKWORD | LAYOUT_STYLE_FORWARD ;
    }
    else if(nStyle & LAYOUT_STYLE_BACKWORD)
    {
        m_nStyle = m_nStyle &~LAYOUT_STYLE_FORWARD | LAYOUT_STYLE_BACKWORD ;
    }

    //if style is center or not
    if(nStyle & LAYOUT_STYLE_NOTCENTER)
    {
        m_nStyle = m_nStyle & ~LAYOUT_STYLE_CENTER | LAYOUT_STYLE_NOTCENTER ;
    }
    else if (nStyle & LAYOUT_STYLE_CENTER)
    {
        m_nStyle = m_nStyle &~LAYOUT_STYLE_NOTCENTER | LAYOUT_STYLE_CENTER ;
    }
}

//////////////////////////////////////////////////////////////////////////

void LinearLayout::SetPadding(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    m_Padding.paddingLeft   = fabs(left);
    m_Padding.paddingTop    = fabs(top);
    m_Padding.paddingRight  = fabs(right);
    m_Padding.paddingBottom = fabs(bottom);
}

//////////////////////////////////////////////////////////////////////////

void LinearLayout::SetPadding(FLOAT paddingValue)
{
    m_Padding.paddingLeft   = fabs(paddingValue);
    m_Padding.paddingTop    = fabs(paddingValue);
    m_Padding.paddingRight  = fabs(paddingValue);
    m_Padding.paddingBottom = fabs(paddingValue);
}

//////////////////////////////////////////////////////////////////////////

void LinearLayout::OnPaint()
{
    ViewLayout::OnPaint();
}

//////////////////////////////////////////////////////////////////////////

BOOL LinearLayout::MeasureChildren(LAYOUTINFO *pLayoutInfo)
{    
    BOOL ifContinue = FALSE;

    if (NULL != pLayoutInfo)
    {
        switch(m_nOrientation)
        {
        case ORIENTATION_VERTICAL:
            ifContinue = MeasureVertical(pLayoutInfo);
            break;
        case ORIENTATION_HORIZONTAL:
            ifContinue = MeasureHorizontal(pLayoutInfo);
            break;
        default:
            break;
        }
    }

    return ifContinue;
}

//////////////////////////////////////////////////////////////////////////

BOOL LinearLayout::MeasureVertical(LAYOUTINFO *pLayoutInfo)
{
    BOOL ifContinue = FALSE;

    if (NULL != pLayoutInfo)
    {
        
        // Not centered sorting.
        if (m_nStyle & LAYOUT_STYLE_NOTCENTER)
        {
            // Get the big one in padding and margin.
            FLOAT tempLeft   = MAX(m_Padding.paddingLeft, pLayoutInfo->leftMargin);
            pLayoutInfo->x = tempLeft;

            // Positive sequence arrangement.
            if (m_nStyle & LAYOUT_STYLE_FORWARD)
            {
                pLayoutInfo->y = m_layoutInfo.height - m_fTempParentHeight + pLayoutInfo->topMargin;
                m_fTempParentHeight -= (pLayoutInfo->height + pLayoutInfo->topMargin);
            }
            // Transferable arrangement.
            else if(m_nStyle & LAYOUT_STYLE_BACKWORD)
            {
                pLayoutInfo->y = m_fTempParentHeight - pLayoutInfo->height - pLayoutInfo->bottomMargin;
                m_fTempParentHeight -= (pLayoutInfo->height + pLayoutInfo->bottomMargin);
            }
        } 
        // Centered sorting.
        else if(m_nStyle & LAYOUT_STYLE_CENTER)
        {
            pLayoutInfo->x = (m_layoutInfo.width - pLayoutInfo->width) / 2;

            // Positive sequence arrangement.
            if (m_nStyle & LAYOUT_STYLE_FORWARD)
            {
                pLayoutInfo->y = m_layoutInfo.height - m_fTempParentHeight + pLayoutInfo->topMargin;
                m_fTempParentHeight -= (pLayoutInfo->height + pLayoutInfo->topMargin);
            }
            // Transferable arrangement.
            else if(m_nStyle & LAYOUT_STYLE_BACKWORD)
            {
                pLayoutInfo->y = m_fTempParentHeight - pLayoutInfo->height - pLayoutInfo->bottomMargin;
                m_fTempParentHeight -= (pLayoutInfo->height + pLayoutInfo->bottomMargin);
            }
        }
    }
    return ifContinue;
}

//////////////////////////////////////////////////////////////////////////

BOOL LinearLayout::MeasureHorizontal(LAYOUTINFO *pLayoutInfo)
{
    BOOL ifContinue = FALSE;

    if (NULL != pLayoutInfo)
    {

        // Not centered sorting.
        if (m_nStyle & LAYOUT_STYLE_NOTCENTER)
        {
            // Get the big one in padding and margin.
            FLOAT tempTop    = MAX(m_Padding.paddingTop, pLayoutInfo->topMargin);
            pLayoutInfo->y = tempTop;

            // Positive sequence arrangement.
            if (m_nStyle & LAYOUT_STYLE_FORWARD)
            {
                pLayoutInfo->x = m_layoutInfo.width - m_fTempParentWidth + pLayoutInfo->leftMargin;
                m_fTempParentWidth -= (pLayoutInfo->width + pLayoutInfo->leftMargin);
            }
            // Transferable arrangement.
            else if(m_nStyle & LAYOUT_STYLE_BACKWORD)
            {
                pLayoutInfo->x = m_fTempParentWidth - pLayoutInfo->width - pLayoutInfo->rightMargin;
                m_fTempParentWidth -= (pLayoutInfo->width + pLayoutInfo->rightMargin);
            }
        } 
        // Centered sorting.
        else if(m_nStyle & LAYOUT_STYLE_CENTER)
        {
            pLayoutInfo->y = (m_layoutInfo.height - pLayoutInfo->height) / 2;

            // Positive sequence arrangement.
            if (m_nStyle & LAYOUT_STYLE_FORWARD)
            {
                pLayoutInfo->x = m_layoutInfo.width - m_fTempParentWidth + pLayoutInfo->leftMargin;
                m_fTempParentWidth -= (pLayoutInfo->width + pLayoutInfo->leftMargin);
            }
            // Transferable arrangement.
            else if(m_nStyle & LAYOUT_STYLE_BACKWORD)
            {
                pLayoutInfo->x = m_fTempParentWidth - pLayoutInfo->width - pLayoutInfo->rightMargin;
                m_fTempParentWidth -= (pLayoutInfo->width + pLayoutInfo->rightMargin);
            }
        }
    }
    return ifContinue;
}
