/*! 
* @file GridViewV.cpp
* 
* @brief This file defines the GridViewV layout.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author SunNing
* @date 2011/01/19
*/

#include "GridViewV.h"
#include "ScrollBar.h"

//////////////////////////////////////////////////////////////////////////

GridViewV::GridViewV() : m_fOffsetY(0),
                         m_fChildMarginX(20), 
                         m_fChildMarginY(80),
                         m_fOriginalIndent(50),
                         m_bIsMouseDown(FALSE),
                         m_fPointx(0),
                         m_fPointy(0),
                         m_fScale(0.0f),
                         m_fElementHeight(75),
                         m_fElementWidth(100),
                         m_usedAreaY(0),
                         m_bAutoSize(FALSE),
                         m_marginX(0.0),
                         m_nColumnCount(1),
                         m_nRowCount(1),
                         m_fHeight(-1),
                         m_isExternalDraw(FALSE),
                         m_cancelMove(FALSE),
                         m_pScrollBarY(NULL),
                         m_pScrollBarX(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

GridViewV::~GridViewV()
{
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetScale(FLOAT fScale)
{
    if (0.5 >= fScale && fScale >= 0)
    {
        m_fScale = fScale;
    }
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetIndent(FLOAT fIndent)
{
    m_fOriginalIndent = fIndent;
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetChildMargin(FLOAT fMarginX, FLOAT fMarginY)
{
    m_fChildMarginX = fMarginX;
    m_fChildMarginY = fMarginY;
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::AutoSize(BOOL isAutoSize)
{
    m_bAutoSize = isAutoSize;
}

//////////////////////////////////////////////////////////////////////////

BOOL GridViewV::MeasureChildren(IN OUT LAYOUTINFO *pLayoutInfo, IN INT32 nIndex)
{
    INT32 remainder = nIndex % (m_nColumnCount);
    if (remainder==0 && nIndex!=0)
    {
        m_usedAreaY=m_usedAreaY+m_fElementHeight +m_fChildMarginY;
    }
    pLayoutInfo->x = remainder*(m_fChildMarginX + m_fElementWidth) + m_marginX;
    pLayoutInfo->y = m_usedAreaY -m_fOffsetY;

    //INT32 remainder =nIndex%(2*colmunCount-1);

    //if (remainder<colmunCount)
    //{
    //    pLayoutInfo->x = remainder*(m_fChildMarginX + m_fElementWidth) + m_marginX;
    //    pLayoutInfo->y = m_usedAreaY -m_fOffsetY;
    //    if (remainder==colmunCount-1)
    //    {
    //        m_usedAreaY=m_usedAreaY+m_fElementHeight +m_fChildMarginY;
    //    }
    //}
    //else
    //{
    //    //pLayoutInfo->x = (remainder-colmunCount)*(m_fChildMarginX + m_fElementWidth) + m_marginX + m_fElementWidth/2;
    //    pLayoutInfo->x = (remainder-colmunCount)*(m_fChildMarginX + m_fElementWidth) + m_marginX + (m_fElementWidth+m_fChildMarginX)/2;
    //    pLayoutInfo->y = m_usedAreaY -m_fOffsetY;
    //    if (remainder==2*colmunCount-1-1)
    //    {
    //          m_usedAreaY=m_usedAreaY+m_fElementHeight +m_fChildMarginY;
    //    }
    //}
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

FLOAT GridViewV::GetScrollPosRatio()
{
	if ( m_fElementHeight == 0.0f )
	{
		return -1.0f;
	}
	FLOAT ratio = this->m_fOffsetY / m_parentHeight;
	return ratio;
}

FLOAT GridViewV::GetScrollLenRatio()
{
	if ( m_fElementHeight == 0.0f )
	{
		return -1.0f;
	}
	FLOAT len = this->GetHeight() - m_fChildMarginY/2.0f - m_fElementHeight;
	FLOAT ratio = len / m_parentHeight;
    if ( ratio >= 0.99999 )
    {
        ratio = 1.0f;
    }
	return ratio;
}

//////////////////////////////////////////////////////////////////////////

VOID GridViewV::CancelScroll()
{
    this->OnMouseEvent(WM_LBUTTONUP, 0, 0 );
}

//////////////////////////////////////////////////////////////////////////

BOOL GridViewV::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = ViewLayout::OnMouseEvent(message, wParam, lParam);

    if (handled)
    {
        return handled;
    }

    if ( !this->IsVisible() )
    {
        return FALSE;
    }

    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);

    switch(message)
    {
    case WM_MOUSEMOVE:

        if(m_bIsMouseDown && !m_bAutoSize && !m_cancelMove)
        {
            
            xPos = (FLOAT)GET_X_LPARAM(lParam);
            yPos = (FLOAT)GET_Y_LPARAM(lParam); 

//            FLOAT px = xPos-m_fPointx;
            FLOAT py =  m_fPointy - yPos;
            if ( abs(py) > this->GetHeight() )
            {
                m_fPointx   = xPos;
                m_fPointy   = yPos;
                return FALSE;
            }
            FLOAT tempOffsetY = m_fOffsetY;
            m_fOffsetY += py;
            FLOAT totalHeight = m_fOffsetY + this->GetHeight()-m_fElementHeight-m_fChildMarginY;
            if (!(totalHeight + m_fChildMarginY/2 > m_parentHeight || m_fOffsetY <= 0))
            {
                if ( m_pScrollBarY )
                {
                    m_pScrollBarY->SetThumbPosRatio(this->GetScrollPosRatio());
                    m_pScrollBarY->SetThumbSizeRatio(this->GetScrollLenRatio());
                }
                SendMessage(m_pWindow->GetHWnd(), WM_USER_UPDATE_SUBWINDOW, 1, -1 * (INT32)py );
                m_isExternalDraw = TRUE;
            }
            else
            {
                
                INT32 childrenCount  = this->GetChildCount();
                INT32 rowCount = childrenCount /m_nColumnCount;

                if (childrenCount%m_nColumnCount != 0)
                {
                    rowCount++;
                }
                //INT32 nCount = 0;
                //nCount = m_nRowCount;
                if (rowCount > m_nRowCount)
                {
                    if (totalHeight + m_fChildMarginY/2 > m_parentHeight)
                    {
                        m_fOffsetY = m_parentHeight - m_fChildMarginY/2
                            - this->GetHeight() + m_fElementHeight + m_fChildMarginY;
                        if ( m_pScrollBarY )
                        {
                            m_pScrollBarY->SetThumbPosRatio(this->GetScrollPosRatio());
                            m_pScrollBarY->SetThumbSizeRatio(this->GetScrollLenRatio());
                        }
                        SetMessageExtraInfo((LPARAM)WM_MESSAGE_EXTRA_GIRDVIEWV_UP);
                        SendMessage(m_pWindow->GetHWnd(), WM_USER_UPDATE_SUBWINDOW, 1, (INT32)(tempOffsetY - m_fOffsetY));
                        m_cancelMove = TRUE;

                    }
                    if (m_fOffsetY <= 0)
                    {
                        m_fOffsetY = 0;
                        if ( m_pScrollBarY )
                        {
                            m_pScrollBarY->SetThumbPosRatio(this->GetScrollPosRatio());
                            m_pScrollBarY->SetThumbSizeRatio(this->GetScrollLenRatio());
                        }
                        SetMessageExtraInfo((LPARAM)WM_MESSAGE_EXTRA_GIRDVIEWV_DOWN);
                        SendMessage(m_pWindow->GetHWnd(), WM_USER_UPDATE_SUBWINDOW, 1, (INT32)tempOffsetY );
                        m_cancelMove = TRUE;
                        
                    }
                }
                else
                {
                    m_fOffsetY = 0;
                }

                 SetMessageExtraInfo(NULL);
                 //m_fOffsetY = tempOffsetY;
            }
            m_fPointx   = xPos;
            m_fPointy   = yPos;
        }
        break;
    case WM_LBUTTONDOWN:
        if (!m_bAutoSize)
        {
            m_fPointx = (FLOAT)GET_X_LPARAM(lParam);
            m_fPointy = (FLOAT)GET_Y_LPARAM(lParam); 
            m_beginY = GET_Y_LPARAM(lParam);
            m_bIsMouseDown = TRUE;
            m_isExternalDraw = FALSE;
            m_cancelMove = FALSE;
            //this->RequestLayout();
            SendMessage(m_pWindow->GetHWnd(), WM_USER_UPDATE_SUBWINDOW, 0, 0);
        }
        break;
    case WM_LBUTTONUP:
        if (!m_bAutoSize && m_bIsMouseDown)
        {
            //this->Invalidate();
            m_bIsMouseDown = FALSE;
            SendMessage(m_pWindow->GetHWnd(), WM_USER_UPDATE_SUBWINDOW, 3, 0);
            this->RequestLayout();
            m_isExternalDraw = FALSE;
        }
        break;
    default: ;
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL GridViewV::RemoveAllChildren(BOOL isClearCache /*= TRUE*/)
{
    m_fOffsetY = 0;
    return ViewLayout::RemoveAllChildren(isClearCache);
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::CalculateChildSize()
{
    FLOAT parentHeight = m_layoutInfo.height;
    FLOAT parentWidth = m_layoutInfo.width;

    m_fElementWidth = (parentWidth - (m_nColumnCount - 1) * m_fChildMarginX) / m_nColumnCount;
    if (m_fHeight >= 0)
    {
        m_fElementHeight = (m_fHeight - (m_nRowCount /*- 1*/) * m_fChildMarginY) / m_nRowCount;
    }
    else
    {
        m_fElementHeight = (parentHeight - (m_nRowCount /*- 1*/) * m_fChildMarginY) / m_nRowCount;
    }
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::OnLayout(BOOL bChanged, 
                         FLOAT fLeft, 
                         FLOAT fTop, 
                         FLOAT fWidth, 
                         FLOAT fHeight)
{
    INT32 nIndex = 0;
    //To record if paint the child paint.
    BOOL ifContinuePaint = TRUE;
    LAYOUTINFO pLayoutInfo;
    CalculateChildSize();
    m_usedAreaY= m_fChildMarginY / 2;
    for (vector<ViewElement*>::iterator itor = m_vctChildren.begin();
        itor != m_vctChildren.end();
        ++itor)
    {
        //Get LayoutInfo.
        (*itor)->GetLayoutInfo(&pLayoutInfo);
        //calculate child width and height.
        pLayoutInfo.height = m_fElementHeight;
        pLayoutInfo.width  = m_fElementWidth;

        //Get children's layoutInfo.
        ifContinuePaint = MeasureChildren(&pLayoutInfo, nIndex);
        ++nIndex;
        (*itor)->SetLayoutInfo(&pLayoutInfo);
    }
    m_parentHeight=m_usedAreaY;
    m_usedAreaY=0;

    ViewLayout::OnLayout(bChanged, fLeft, fTop, fWidth, fHeight);

    FLOAT totalHeight = m_fOffsetY + this->GetHeight()-m_fElementHeight-m_fChildMarginY;

    if (!(totalHeight + m_fChildMarginY/2 > m_parentHeight || m_fOffsetY <= 0))
    {
    }
    else
    {
        INT32 childrenCount  = this->GetChildCount();
        INT32 rowCount = childrenCount /m_nColumnCount;

        if (childrenCount%m_nColumnCount != 0)
        {
            rowCount++;
        }
        if (rowCount > m_nRowCount)
        {
            if (totalHeight + m_fChildMarginY/2 > m_parentHeight)
            {
                m_fOffsetY = m_parentHeight - m_fChildMarginY/2
                    - this->GetHeight() + m_fElementHeight + m_fChildMarginY;

            }
            if (m_fOffsetY <= 0)
            {
                m_fOffsetY = 0;
            }
        }
        else
        {
            m_fOffsetY = 0;
        }
    }

    if ( m_pScrollBarY != NULL )
	{
		m_pScrollBarY->SetThumbPosRatio(this->GetScrollPosRatio());
		m_pScrollBarY->SetThumbSizeRatio(this->GetScrollLenRatio());
	}
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetOnlayout()
{
    if (m_bAutoSize)
    {
        LAYOUTINFO myLayoutInfo;
        myLayoutInfo.x = this->GetLeft();
        myLayoutInfo.y = this->GetTop();
        myLayoutInfo.height = m_parentHeight + m_fChildMarginY + m_fElementHeight ;
        myLayoutInfo.width = this->GetWidth();
        this->SetLayoutInfo(&myLayoutInfo);
    }
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetColumnAndRowCount(INT32 nColumnCount, INT32 nRowCount)
{
    if (nColumnCount > 0)
    {
        m_nColumnCount = nColumnCount;
    }
    if (nRowCount > 0)
    {
        m_nRowCount = nRowCount;
    }
    m_fHeight = -1;
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetColumnAndRowCountInArea(INT32 nColumnCount, INT32 nRowCount, FLOAT fHeight)
{
    if (nColumnCount > 0)
    {
        m_nColumnCount = nColumnCount;
    }
    if (nRowCount > 0)
    {
        m_nRowCount = nRowCount;
    }
    if (fHeight >= 0)
    {
        m_fHeight = fHeight;
    }


}

//////////////////////////////////////////////////////////////////////////

BOOL GridViewV::RemoveChild(ViewElement *pChild)
{
    BOOL bResult = ViewLayout::RemoveChild(pChild);
    if (!m_bAutoSize)
    {
        INT32 count = this->GetChildCount();
        INT32 row = count /   m_nColumnCount;
        INT32 am = count % m_nColumnCount;
        if (am!=0)
        {
            row+=1;
        }
        FLOAT fTemp = m_parentHeight + m_fChildMarginY + m_fElementHeight;
        m_parentHeight =  row * (m_fChildMarginY + m_fElementHeight);
        FLOAT fBottom = m_parentHeight - this->GetParent()->GetHeight();
        if (fTemp != m_parentHeight && this->GetParent()->GetHeight() <= m_parentHeight && m_fOffsetY >= fBottom)
        {
            m_fOffsetY = fBottom;
            RequestLayout();
        }
    }

    return bResult;
}

//////////////////////////////////////////////////////////////////////////

BOOL GridViewV::RemoveChildAt(UINT index)
{
    BOOL bResult = ViewLayout::RemoveChildAt(index);
    if (!m_bAutoSize)
    {
        INT32 count = this->GetChildCount();
        INT32 row = count /   m_nColumnCount;
        INT32 am = count % m_nColumnCount;
        if (am!=0)
        {
            row+=1;
        }
        FLOAT fTemp = m_parentHeight + m_fChildMarginY + m_fElementHeight;
        m_parentHeight =  row * (m_fChildMarginY + m_fElementHeight);
        FLOAT fBottom = m_parentHeight - this->GetParent()->GetHeight();
        if (fTemp != m_parentHeight && this->GetParent()->GetHeight() <= m_parentHeight && m_fOffsetY >= fBottom)
        {
            m_fOffsetY = fBottom;
            RequestLayout();
        }
    }

    return bResult;
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::CalculateLayoutOffset()
{
    INT32 nIndex = 0;
    //To record if paint the child paint.
    //BOOL ifContinuePaint = TRUE;
    LAYOUTINFO pLayoutInfo;
    CalculateChildSize();
    m_usedAreaY= m_fChildMarginY / 2;
    for (vector<ViewElement*>::iterator itor = m_vctChildren.begin();
        itor != m_vctChildren.end();
        ++itor)
    {
        //Get LayoutInfo.
        (*itor)->GetLayoutInfo(&pLayoutInfo);
        INT32 remainder = nIndex % (m_nColumnCount);
        if (remainder==0 && nIndex!=0)
        {
            m_usedAreaY=m_usedAreaY+m_fElementHeight +m_fChildMarginY;
        }
        ++nIndex;
    }
    m_parentHeight=m_usedAreaY;
    m_usedAreaY=0;
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetScrollBarY( ScrollBar* pScrollBar )
{
	m_pScrollBarY = pScrollBar;
	if ( m_pScrollBarY )
	{
		m_pScrollBarY->SetThumbPosRatio(this->GetScrollPosRatio());
		m_pScrollBarY->SetThumbSizeRatio(this->GetScrollLenRatio());
	}
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::SetScrollBarX( ScrollBar* pScrollBar )
{
	m_pScrollBarX = pScrollBar;
}

//////////////////////////////////////////////////////////////////////////

ScrollBar* GridViewV::GetScrollBarY()
{
	return m_pScrollBarY;
}

//////////////////////////////////////////////////////////////////////////

ScrollBar* GridViewV::GetScrollBarX()
{
	return m_pScrollBarX;
}

//////////////////////////////////////////////////////////////////////////

void GridViewV::OnPaint()
{
    if ( FALSE == m_isExternalDraw )
    {
        ViewLayout::OnPaint();
    }
}

void GridViewV::SetOffsetPos(FLOAT offset)
{
	m_fOffsetY = offset;
	this->RequestLayout();
}