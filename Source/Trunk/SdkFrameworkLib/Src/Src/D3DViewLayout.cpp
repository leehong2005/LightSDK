/*!
* @file D3DViewElement.h 
* 
* @brief This file defines the base functions of D3DViewLayout.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#include "stdafx.h"
#include "D3DViewLayout.h"
#include "D3DUtility.h"
#include "D3DCamera.h"
#include "ID3DViewEventHandler.h"

USING_NAMESPACE_D3D

//////////////////////////////////////////////////////////////////////////

D3DViewLayout::D3DViewLayout()
{

}

//////////////////////////////////////////////////////////////////////////

D3DViewLayout::~D3DViewLayout()
{
    for ( UINT32 i = 0; i < m_vctChildren.size(); i++ )
    {
        SAFE_DELETE(m_vctChildren[i]);
    }
    m_vctChildren.clear();
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::AddChild(D3DViewElement *pViewElement)
{
    this->m_vctChildren.push_back(pViewElement);
    pViewElement->SetParent(this);
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::GetChild(UINT32 index, D3DViewElement **ppOutViewElement)
{
    if ( index >= 0 && index < this->m_vctChildren.size() )
    {
        if ( ppOutViewElement != NULL )
        {
            *ppOutViewElement = m_vctChildren[index];
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::OnLayout()
{

}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::OnPaint()
{
    for ( UINT32 i = 0; i < m_vctChildren.size(); i++ )
    {
        m_vctChildren[i]->OnPaint();
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::SetWindow( SdkWindow *pWindow )
{
    D3DViewElement::SetWindow(pWindow);
    for ( UINT32 i = 0; i < m_vctChildren.size(); i++ )
    {
        m_vctChildren[i]->SetWindow(pWindow);
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::SetShowView( BOOL val )
{ 
    m_isShown = val;
    for ( UINT32 i = 0; i < m_vctChildren.size(); i++ )
    {
        m_vctChildren[i]->SetShowView(val);
    }
};

//////////////////////////////////////////////////////////////////////////

BOOL D3DViewLayout::OnMouseEvent( IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
{
    BOOL isProcess = FALSE;
    BOOL isHit = FALSE;
    D3DViewElement* pHitTarget = NULL;
    for ( UINT32 i = 0; i < m_vctChildren.size(); i++ )
    {
        if ( this->m_pCamera != NULL )
        {
            POINT screen = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            D3DViewLayout* pLayout = dynamic_cast<D3DViewLayout*>(m_vctChildren[i]);
            if ( pLayout != NULL )
            {
                if (pLayout->OnMouseEvent( message, wParam, lParam ))
                {
                    break;
                }
            }
            else
            {
                if ( D3DUtility::IsHitTest( m_vctChildren[i], this->m_pCamera, screen ) )
                {
                    isHit = TRUE;
                    pHitTarget = m_vctChildren[i];
                    if (m_vctChildren[i]->OnMouseEvent(message, wParam, lParam ))
                    {
                        isProcess = TRUE;
                        break;
                    }
                }
            }
        }
    }
    if ( isHit && !isProcess )
    {
        if ( m_pEventHandler != NULL )
        {
            //POINT screen = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            //D3DUtility::IsHitTest( this, this->m_pCamera, screen );
            D3DViewElement* pViewElement = this;
            while( pViewElement != NULL )
            {
                if ( pViewElement->GetEventHandler() != NULL )
                {
                    if (pViewElement->GetEventHandler()->OnMouseMessage( pHitTarget, 
                        pViewElement, message, wParam, lParam ))
                    {
                        isProcess = TRUE;
                        break;
                    }
                    pViewElement = pViewElement->GetParent();
                }
            }
        }
    }
    return isProcess;
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::SetCamera(D3DCamera* pCamera)
{
    D3DViewElement::SetCamera(pCamera);
    for ( UINT32 i = 0; i < m_vctChildren.size(); i++ )
    {
        m_vctChildren[i]->SetCamera(pCamera);
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::RemoveChild( D3DViewElement* pElement )
{
    for ( UINT32 i = 0; i < m_vctChildren.size(); i++ )
    {
        if ( pElement == m_vctChildren[i] )
        {
            m_vctChildren.erase(m_vctChildren.begin() + i );
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DViewLayout::BringChildViewToTop( D3DViewElement* pViewElement )
{
    INT32 index = -1;
    for ( INT32 i = 0; i < (INT32)m_vctChildren.size(); i++ )
    {
        if ( pViewElement == m_vctChildren[i] )
        {
            index = i;
            break;
        }
    }

    if ( index >= 0 && index < (INT32)m_vctChildren.size() - 1 )
    {
        m_vctChildren.erase(m_vctChildren.begin() + index);
        m_vctChildren.push_back(const_cast<D3DViewElement*>(pViewElement));
    }
}
