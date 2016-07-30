/*!
* @file D3DPlaneView.h 
* 
* @brief This file defines the base functions of D3DUtility.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DUTILITY_H_
#define _D3DUTILITY_H_

#include "CommonInclude.h"
#include <d3d9.h>
#include <d3dx9.h>

class D3DViewElement;
class D3DCamera;


class CLASS_DECLSPEC D3DUtility
{
public:
    
    typedef struct _DEVICECONTEXT
    {
        HDC surfaceDC;
        RECT rect;
        IDirect3DSurface9* pSurface;

    } DEVICECONTEXT, *LPDEVICECONTEXT;

    /*!
    * @brief The constructor function.
    */
    D3DUtility();

    /*!
    * @brief The destructor function.
    */
    virtual ~D3DUtility();

    /*!
    * @brief test 2D point hit to the 3d geometry
    * 
    * @param pViewElement
    * @param pCamera
    * @param ptScreen
    *
    * @return TRUE hit, FALSE not hit
    *
    */
    static BOOL IsHitTest( D3DViewElement* pViewElement, D3DCamera* pCamera, POINT ptScreen );
    
    /*!
    * @brief convert 2D to 3D.
    *
    * @param ptScreen        screen 2D coordinate position
    * @param pCamera        camera matrix
    * @param pClipView        world geometry 
    * @param pOutPoint        return 3D coordinate position
    *
    */
    static BOOL Convert2Dto3D( 
        IN POINT ptScreen,  
        IN D3DCamera* pCamera,
        IN D3DViewElement* pClipView,
        OUT D3DXVECTOR3* pOutPoint,
        IN float ptX = 0.0f,
        IN float ptY = 0.0f,
        IN float ptZ = 0.0f);

    /*!
    * @brief convert 3D to 2D.
    *
    * @param pCamera        camera
    * @param pViewElement    world geometry
    * @param p3DPoint        3D point 
    * @param pOutPt            return 2D coordinate position
    *
    */
    static BOOL Convert3Dto2D(
        IN D3DCamera* pCamera,
        IN D3DViewElement* pViewElement,
        IN D3DXVECTOR3* p3DPoint,
        OUT POINT* pOutPt
        );


    static void CreateEmptyTexture( 
        IN const LPDIRECT3DDEVICE9 pDevice,
        SIZE size,
        OUT LPDIRECT3DTEXTURE9* ppTexture );


    static void BeginUpdateTexture( 
        IN LPDIRECT3DTEXTURE9 pTexture,
        OUT DEVICECONTEXT& deviceContext);

    static void CopyTexture( 
        IN LPDIRECT3DTEXTURE9 pTextureSrc,
        IN LPDIRECT3DTEXTURE9 pTextureDst,
        IN const LPRECT pRectSrc = NULL,
        IN const LPRECT pRectDst = NULL);

    static void EndUpdateTexture( 
        IN LPDIRECT3DTEXTURE9 pTexture,
        IN DEVICECONTEXT& deviceContext);

    static void updateDesktopTexture( IN LPDIRECT3DTEXTURE9 pTexture,
        IN HDC hdc,
        IN const RECT& rect,
        IN HWND hwnd);

    static void CreateBMPFile( LPTSTR pszFile,
        HBITMAP hBMP, HDC hDC);

};

#endif //_D3DANIMATION_H_
#endif //__cplusplus