/*!
* @file D3DViewElement.h 
* 
* @brief This file defines the base functions of D3DViewElement.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/
#include "Common.h"
#include "D3DViewElement.h"
#include "D3DDevice.h"
#include "Animation.h"
#include "Window.h"
#include "ID3DViewEventHandler.h"
#include "D3DUtility.h"
#include "D3DViewLayout.h"
//////////////////////////////////////////////////////////////////////////

D3DViewElement::D3DViewElement():m_dwDiffuseColor(0xffff00ff),m_pWindow(NULL),m_pAnimation(NULL),m_isShown(TRUE),m_pParent(NULL),m_pEventHandler(NULL),m_pCamera(NULL)
{
    ::D3DXMatrixIdentity(&m_matWorld);
}

//////////////////////////////////////////////////////////////////////////

D3DViewElement::~D3DViewElement()
{

}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::OnPaint()
{

}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::Invalidate()
{
    if (NULL != m_pWindow)
    {
        InvalidateRect(m_pWindow->GetHWND(), NULL, FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////

Window* D3DViewElement::GetWindow()
{
    return this->m_pWindow;
}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::SetAnimation(Animation *pAnimation)
{
    m_pAnimation = pAnimation;
}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::StartAnimation()
{
    if ( m_pAnimation != NULL )
    {
        m_pAnimation->Start();
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::ClearAnimation()
{
    m_pAnimation = NULL;
}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::OnAnimationStart()
{

}

void D3DViewElement::OnAnimationEnd()
{

}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::SetWorldMatrix( IN const D3DXMATRIX* matValue )
{
    if ( matValue != NULL )
    {
        m_matWorld = *matValue;
    }
    
}

//////////////////////////////////////////////////////////////////////////

D3DXMATRIX* D3DViewElement::GetWorldMatrix( OUT D3DXMATRIX* matOut )
{
    if ( matOut != NULL )
    {
        *matOut = m_matWorld;
    }
    return &m_matWorld;
}

//////////////////////////////////////////////////////////////////////////

BOOL D3DViewElement::OnTouchEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D3DViewElement::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( m_pEventHandler != NULL  && this->m_pCamera != NULL )
    {
        POINT screen = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (D3DUtility::IsHitTest( this, this->m_pCamera, screen ))
        {
            return m_pEventHandler->OnMouseMessage( this, this, message, wParam, lParam );
        }
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D3DViewElement::OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::SetWindow(Window *pWindow)
{
    m_pWindow = pWindow;
}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::GetPoints( OUT vector<D3DXVECTOR3>& pOutVectors )
{
    UNREFERENCED_PARAMETER(pOutVectors);
}

//////////////////////////////////////////////////////////////////////////

D3DDevice* D3DViewElement::GetD3DDevice(D3DViewElement *pViewElement)
{
    Window* pWindow = pViewElement->GetWindow();
    if ( pWindow != NULL )
    {
        return pWindow->GetD3DDevice();
    }
    return NULL;
}

void D3DViewElement::CalcWorldMatrix( D3DXMATRIX* outMatrix )
{
    if ( outMatrix != NULL )
    {
        D3DXMatrixIdentity(outMatrix);
        D3DViewElement* pParent = NULL;
        pParent = this->m_pParent;
        *outMatrix = *outMatrix * this->m_matWorld;
        while ( pParent != NULL )
        {
            *outMatrix *= *(pParent->GetWorldMatrix(NULL));
            pParent = pParent->m_pParent;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DViewElement::BringViewToTop()
{
    if ( this->m_pParent != NULL )
    {
        D3DViewLayout* pVL = dynamic_cast<D3DViewLayout*>(this->m_pParent);
        if ( pVL != NULL )
        {
            pVL->BringChildViewToTop(this);
        }
    }
}