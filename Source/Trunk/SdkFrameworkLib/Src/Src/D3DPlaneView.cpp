/*!
* @file D3DPlaneView.cpp 
* 
* @brief This file defines the base functions of D3DPlaneView.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#include "stdafx.h"
#include "D3DPlaneView.h"

USING_NAMESPACE_D3D

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//////////////////////////////////////////////////////////////////////////

D3DPlaneView::D3DPlaneView():D3DViewElement(),m_pPlanes(NULL),m_pVB(NULL),m_pTex(NULL),m_TFACOTR(0xFFFFFFFF),m_pFocusTex(NULL),m_isFocus(FALSE)
{
}

//////////////////////////////////////////////////////////////////////////

D3DPlaneView::~D3DPlaneView()
{
    SAFE_DELETE_ARRAY(m_pPlanes);
    SAFE_RELEASE(m_pVB);
    SAFE_RELEASE(m_pTex);
    SAFE_RELEASE(m_pFocusTex);
}

//////////////////////////////////////////////////////////////////////////

void D3DPlaneView::GetPoints( OUT vector<D3DXVECTOR3>& pOutVectors )
{
    for ( UINT32 i = 0; i < 6 * 2 ; i++ )
    {
        D3DXVECTOR3 temp;
        temp.x = m_pPlanes[i].x;
        temp.y = m_pPlanes[i].y;
        temp.z = m_pPlanes[i].z;
        pOutVectors.push_back(temp);
    }
}

//////////////////////////////////////////////////////////////////////////

void D3DPlaneView::OnPaint()
{
    if ( FALSE == m_isShown )
    {
        return;
    }

    LPDIRECT3DDEVICE9 pDevice = NULL;
    pDevice = GetD3DDevice(this)->GetDrawingDevice();
    if ( NULL == m_pVB )
    {
        InitVertexBuffer();
    }
    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );
    //D3DXMatrixIdentity(&m_matWorld);
    // Render the vertex buffer contents
    if ( m_isFocus )
    {
        pDevice->SetTexture(0, m_pFocusTex);
    }
    else
    {
        pDevice->SetTexture(0, m_pTex);
    }
    
    pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_TFACOTR );
    D3DXMATRIX worldMat;
    this->CalcWorldMatrix(&worldMat);
    pDevice->SetTransform(D3DTS_WORLD, &worldMat);
    pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(PLANEVERTEX) );
    pDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 * 2);
}

//////////////////////////////////////////////////////////////////////////

void D3DPlaneView::InitParams(const D3DXVECTOR3 &ptA, const D3DXVECTOR3 &ptB, const D3DXVECTOR3 &ptC, const D3DXVECTOR3 &ptD)
{
    if ( m_pPlanes != NULL )
    {
        SAFE_DELETE_ARRAY(m_pPlanes);
        SAFE_RELEASE(m_pVB);
    }

    m_pPlanes = new PLANEVERTEX[6 * 2];

    PLANEVERTEX temp[4] = 
    {
        {    ptA.x,ptA.y,ptA.z, 0xffffffff, 0,0 },
        {    ptB.x,ptB.y,ptB.z, 0xffffffff, 1,0 },
        {    ptC.x,ptC.y,ptC.z, 0xffffffff, 0,1 },
        {    ptD.x,ptD.y,ptD.z, 0xffffffff, 1,1 },
    };

    //m_pPlanes[0] = temp[2];
    //m_pPlanes[1] = temp[3];
    //m_pPlanes[2] = temp[1];
    //m_pPlanes[3] = temp[2];
    //m_pPlanes[4] = temp[1];
    //m_pPlanes[5] = temp[0];

    //m_pPlanes[0 + 6] = temp[2];
    //m_pPlanes[1 + 6] = temp[0];
    //m_pPlanes[2 + 6] = temp[1];
    //m_pPlanes[3 + 6] = temp[2];
    //m_pPlanes[4 + 6] = temp[1];
    //m_pPlanes[5 + 6] = temp[3];

    m_pPlanes[0] = temp[2];
    m_pPlanes[1] = temp[3];
    m_pPlanes[2] = temp[1];
    m_pPlanes[3] = temp[2];
    m_pPlanes[4] = temp[1];
    m_pPlanes[5] = temp[0];

    m_pPlanes[0 + 6] = temp[2];
    m_pPlanes[1 + 6] = temp[0];
    m_pPlanes[2 + 6] = temp[1];
    m_pPlanes[3 + 6] = temp[2];
    m_pPlanes[4 + 6] = temp[1];
    m_pPlanes[5 + 6] = temp[3];
}

//////////////////////////////////////////////////////////////////////////

void D3DPlaneView::InitParams( const D3DXVECTOR3& ptA, const float uA, const float vA, D3DCOLOR diffuseColorA, const D3DXVECTOR3& ptB, const float uB, const float vB, D3DCOLOR diffuseColorB, const D3DXVECTOR3& ptC, const float uC, const float vC, D3DCOLOR diffuseColorC, const D3DXVECTOR3& ptD, const float uD, const float vD, D3DCOLOR diffuseColorD )
{
    if ( m_pPlanes != NULL )
    {
        SAFE_DELETE_ARRAY(m_pPlanes);
        SAFE_RELEASE(m_pVB);
    }

    m_pPlanes = new PLANEVERTEX[6 * 2];

    PLANEVERTEX temp[4] = 
    {
        {    ptA.x,ptA.y,ptA.z, diffuseColorA, uA,vA },
        {    ptB.x,ptB.y,ptB.z, diffuseColorB, uB,vB },
        {    ptC.x,ptC.y,ptC.z, diffuseColorC, uC,vC },
        {    ptD.x,ptD.y,ptD.z, diffuseColorD, uD,vD },
    };

    m_pPlanes[0] = temp[2];
    m_pPlanes[1] = temp[3];
    m_pPlanes[2] = temp[1];
    m_pPlanes[3] = temp[2];
    m_pPlanes[4] = temp[1];
    m_pPlanes[5] = temp[0];

    m_pPlanes[0 + 6] = temp[2];
    m_pPlanes[1 + 6] = temp[0];
    m_pPlanes[2 + 6] = temp[1];
    m_pPlanes[3 + 6] = temp[2];
    m_pPlanes[4 + 6] = temp[1];
    m_pPlanes[5 + 6] = temp[3];

}

//////////////////////////////////////////////////////////////////////////

void D3DPlaneView::InitVertexBuffer()
{
    // Create the vertex buffer.
    m_pVB = NULL;
    if( FAILED(  GetD3DDevice(this)->GetDrawingDevice()->CreateVertexBuffer( 
        6 * 2 * sizeof(PLANEVERTEX),
        D3DUSAGE_WRITEONLY, 
        D3DFVF_CUSTOMVERTEX,
        D3DPOOL_MANAGED, 
        &m_pVB, 
        NULL ) ) )
    {
        return;
    }

    // Fill the vertex buffer.
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, 6 * 2 * sizeof(PLANEVERTEX), (void**)&pVertices, 0 ) ) )
    {
        SAFE_RELEASE(m_pVB);
        return;
    }
    memcpy( pVertices, m_pPlanes, 6 * 2 * sizeof(PLANEVERTEX) );

    m_pVB->Unlock();

}

void D3DPlaneView::SetTexture( LPDIRECT3DTEXTURE9 pTex )
{
    this->m_pTex = pTex;
}

LPDIRECT3DTEXTURE9 D3DPlaneView::GetTexture()
{
    return this->m_pTex;
}

void D3DPlaneView::SetFocusTexture( LPDIRECT3DTEXTURE9 pFocusTex )
{
    this->m_pFocusTex = pFocusTex;
}


LPDIRECT3DTEXTURE9 D3DPlaneView::GetFocusTexture()
{
    return this->m_pFocusTex;
}