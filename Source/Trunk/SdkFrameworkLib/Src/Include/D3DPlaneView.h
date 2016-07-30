/*!
* @file D3DPlaneView.h 
* 
* @brief This file defines the base functions of D3DPlaneView.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DPLANEVIEW_H_
#define _D3DPLANEVIEW_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "D3DViewElement.h"

BEGIN_NAMESPACE_D3D

typedef struct _PLANEVERTEX
{
    FLOAT x, y, z;          // The untransformed, 3D position for the vertex
    D3DCOLOR color;         // The vertex color
    FLOAT u,v;

} PLANEVERTEX, *LPPLANEVERTEX;


class CLASS_DECLSPEC D3DPlaneView : public D3DViewElement
{
public:

    friend class SdkWindow;

    /*!
    * @brief
    * the construction function.
    */
    D3DPlaneView();
    /*!
    * @brief
    * the destruction function.
    */
    virtual ~D3DPlaneView();

    /*!
    * @brief Get Points.
    *
    * @param pOutVectors        [ /O] the points
    *
    */
    virtual void GetPoints( OUT vector<D3DXVECTOR3>& pOutVectors );

    /*!
    * @brief OnPaint.
    *
    */
    virtual void OnPaint();

    /*!
    * @brief Get Points.
    *
    * @param ptA,        [I/ ] the point
    * @param ptB,         [I/ ] the point
    * @param ptC,         [I/ ] the point
    * @param ptD,         [I/ ] the point
    *  A----------B
    *  |          |
    *  C----------D
    *
    */
    virtual void InitParams(
        const D3DXVECTOR3& ptA, 
        const D3DXVECTOR3& ptB,
        const D3DXVECTOR3& ptC,
        const D3DXVECTOR3& ptD);

    virtual void InitParams(
        const D3DXVECTOR3& ptA, 
        const float uA,
        const float vA,
        D3DCOLOR diffuseColorA,
        const D3DXVECTOR3& ptB,
        const float uB,
        const float vB,
        D3DCOLOR diffuseColorB,
        const D3DXVECTOR3& ptC,
        const float uC,
        const float vC,
        D3DCOLOR diffuseColorC,
        const D3DXVECTOR3& ptD,
        const float uD,
        const float vD,
        D3DCOLOR diffuseColorD
        );


    /*!
    * @brief init vertex buffer
    */
    virtual void InitVertexBuffer();


    void SetTexture( LPDIRECT3DTEXTURE9 pTex );

    void SetFocusTexture( LPDIRECT3DTEXTURE9 pFocusTex );

    LPDIRECT3DTEXTURE9 GetTexture();

    LPDIRECT3DTEXTURE9 GetFocusTexture();

    void SetTFACTOR( DWORD val )
    {
        m_TFACOTR = val;
    }

    void SetFocus( BOOL isFocus )
    {
        m_isFocus = isFocus;
    }

    BOOL GetFocus()
    {
        return m_isFocus;
    }

protected:
    // plane points
    LPPLANEVERTEX m_pPlanes;
    // plane vertex buffer
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    // plane texture
    LPDIRECT3DTEXTURE9        m_pTex;
    LPDIRECT3DTEXTURE9        m_pFocusTex;
    DWORD         m_TFACOTR;
    BOOL                      m_isFocus;

};

END_NAMESPACE_D3D

#endif // _D3DPLANEVIEW_H_
#endif // __cplusplus
