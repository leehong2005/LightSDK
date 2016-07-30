/*!
* @file D3DCamera.cpp
* 
* @brief This file defines the base functions of D3DCamera.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/
#include "D3DCamera.h"
#include "D3DDevice.h"

//////////////////////////////////////////////////////////////////////////

D3DCamera::D3DCamera()
{
    D3DXMatrixIdentity(&m_MatView);
    D3DXMatrixIdentity(&m_MatProj);
}

//////////////////////////////////////////////////////////////////////////

D3DCamera::~D3DCamera()
{

}

//////////////////////////////////////////////////////////////////////////

const D3DXMATRIX* D3DCamera::setupProj(float fov, 
                                       float aspect, 
                                       float zn, 
                                       float zf, 
                                       D3DDevice* pDevice )
{
    LPDIRECT3DDEVICE9 pDrawingDevice = pDevice->GetDrawingDevice();
    D3DXMatrixPerspectiveFovRH( &m_MatProj, fov, aspect, zn, zf);
    pDrawingDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);
    return &m_MatProj;
}

//////////////////////////////////////////////////////////////////////////

const D3DXMATRIX* D3DCamera::setupView( 
                            const D3DXVECTOR3& vectEye, 
                            const D3DXVECTOR3& vectLookat,
                            const D3DXVECTOR3& vectUp,
                            D3DDevice* pD3DDevice  )
{
    LPDIRECT3DDEVICE9 pDrawingDevice = pD3DDevice->GetDrawingDevice();
    D3DXMatrixLookAtRH( &m_MatView, &vectEye, &vectLookat, &vectUp );
    pDrawingDevice->SetTransform(D3DTS_VIEW, &m_MatView);
    return &m_MatView;
}