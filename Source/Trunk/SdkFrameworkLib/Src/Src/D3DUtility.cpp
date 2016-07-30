/*!
* @file D3DUtility.h 
* 
* @brief This file defines the base functions of D3DUtility.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#include "stdafx.h"
#include "D3DUtility.h"
#include "D3DViewElement.h"
#include "D3DViewLayout.h"
#include "D3DCamera.h"
#include "SdkUICommon.h"

USING_NAMESPACE_D3D

D3DUtility::D3DUtility()
{

}

//////////////////////////////////////////////////////////////////////////

D3DUtility::~D3DUtility()
{

}

//////////////////////////////////////////////////////////////////////////

BOOL D3DUtility::IsHitTest(D3DViewElement *viewElement, D3DCamera *pCamera, POINT ptScreen)
{

    int x = ptScreen.x;
    int y = ptScreen.y;

    D3DXMATRIX viewMatrix =*(pCamera->getViewMatrix()) ;

    D3DXVECTOR3 v;
    v.x = ((2.0f * x ) / (FLOAT)D3DViewElement::GetD3DDevice(viewElement)->GetWidth() - 1) / (*(pCamera->getProjMatrix()))._11;
    v.y = -((2.0f * y ) / (FLOAT)D3DViewElement::GetD3DDevice(viewElement)->GetHeight() - 1 ) / (*(pCamera->getProjMatrix()))._22;
    v.z = -1.0f;


    D3DXMATRIX worldView;
    viewElement->CalcWorldMatrix(&worldView);
    //=(*viewElement->GetWorldMatrix(NULL)) * viewMatrix  ;
    worldView *=  viewMatrix;
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, &worldView);

    D3DXVECTOR3 matPickRayDir;
    D3DXVECTOR3 matPickOrig;


    // Transform the screen space pick ray into 3D space
    matPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    matPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    matPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    matPickOrig.x = m._41;
    matPickOrig.y = m._42;
    matPickOrig.z = m._43;


    vector<D3DXVECTOR3> pointList;
    viewElement->GetPoints(pointList);

    BOOL isHit = FALSE;
    for ( UINT32 i = 0; i < pointList.size()/3; i += 3 )
    {
        isHit = D3DXIntersectTri( &pointList[i], &pointList[i+1], &pointList[i+2],
            &matPickOrig, &matPickRayDir, NULL, NULL, NULL );
        if ( isHit )
        {
            break;
        }
    }
    return isHit;
}

//////////////////////////////////////////////////////////////////////////

BOOL D3DUtility::Convert2Dto3D(POINT ptScreen, D3DCamera *pCamera, D3DViewElement *pClipView, D3DXVECTOR3 *pOutPoint, float ptX, float ptY, float ptZ )
{
	if ( dynamic_cast<D3DViewLayout*>(pClipView) != NULL )
	{
		return FALSE;
	}

    //::GetCursorPos(&p);
    //::ScreenToClient(hWnd, &p);
    LPDIRECT3DDEVICE9 pDevice = D3DViewElement::GetD3DDevice(pClipView)->GetDrawingDevice();
    D3DVIEWPORT9 viewport;
    pDevice->GetViewport(&viewport);
    //// Get matrices and the viewport.
    //D3DXMATRIX pm; // projection matrix
    //D3DXMATRIX vm; // view matrix
    //D3DXMATRIX wm; // world matrix
    //D3DVIEWPORT9 vp; // viewport

    //g_Device->GetTransform(D3DTS_PROJECTION, &pm);
    //g_Device->GetTransform(D3DTS_VIEW, &vm);
    //D3DXMatrixIdentity(&wm); // make the world matrix an identity matrix
    //g_Device->GetViewport(&vp);

    // Call the D3DXVec3Unproject function twice to determine two points
    // in our world space. Having two points we will be able to find
    // a line in the world space. One point is for the z-near plane
    // (which is usually z=0.0f) and another point is for z-far plane
    // (which is usually z=1.0f). We grab the z-planes from the viewport.
    D3DXVECTOR3 p1;
	D3DXVECTOR3 v1((FLOAT)ptScreen.x-0.5f, (FLOAT)ptScreen.y-0.5f, viewport.MinZ);
    D3DXMATRIX worldMat;
    pClipView->CalcWorldMatrix(&worldMat);
    D3DXVec3Unproject(&p1, &v1, 
        &viewport, 
        pCamera->getProjMatrix(), 
        pCamera->getViewMatrix(), 
        &worldMat);

    D3DXVECTOR3 p2;
	D3DXVECTOR3 v2((FLOAT)ptScreen.x-0.5f, (FLOAT)ptScreen.y-0.5f, viewport.MaxZ);
    D3DXVec3Unproject(&p2, &v2,         
        &viewport, 
        pCamera->getProjMatrix(), 
        pCamera->getViewMatrix(), 
        &worldMat);

    // We calculate an intersection of the line found previously
    // with a plane z=0.0f. Equations of a line in 3-D space can
    // be found on the following websites:
    // http://mathforum.org/dr.math/faq/formulas/faq.ag3.html#threelines
    // http://mathforum.org/library/drmath/view/65721.html
    // Here, wx, wy, and wz represent coordinates in the world space.
    float wz = worldMat._43 * 1 + worldMat._33 * ptZ + worldMat._23 * ptY + worldMat._13 * ptX;
    float wx = ((wz-p1.z)*(p2.x-p1.x))/(p2.z-p1.z) + p1.x;
    float wy = ((wz-p1.z)*(p2.y-p1.y))/(p2.z-p1.z) + p1.y;

    // w is the point in the world space corresponding to the p point
    // in the screen coordinates.
    D3DXVECTOR3 w(wx, wy, wz);

    if ( pOutPoint != NULL )
    {
        *pOutPoint = w;
    }

	return TRUE;

}

//////////////////////////////////////////////////////////////////////////

BOOL D3DUtility::Convert3Dto2D(D3DCamera *pCamera, D3DViewElement *pViewElement, D3DXVECTOR3 *p3DPoint, POINT *pOutPt)
{
	if ( dynamic_cast<D3DViewLayout*>(pViewElement) != NULL )
	{
		return FALSE;
	}

    LPDIRECT3DDEVICE9 pDevice = D3DViewElement::GetD3DDevice(pViewElement)->GetDrawingDevice();
    D3DVIEWPORT9 viewport;
    pDevice->GetViewport(&viewport);
    //D3DVIEWPORT9 pvport = {0,0,
    //    (DWORD)g_pAppData->m_D3DscreenWidth,
    //    (DWORD)g_pAppData->m_D3DscreenHeight,
    //    0.0f,1.0f};
    D3DXVECTOR3 out;

    D3DXMATRIX worldMat;
    pViewElement->CalcWorldMatrix(&worldMat);
    
    //change the 3d to 2d by the projMatrix ,viewMatrix,WorldMatrix;
    D3DXVec3Project( &out, 
        p3DPoint, 
        &viewport, 
        pCamera->getProjMatrix(), 
        pCamera->getViewMatrix(), 
        &worldMat );
    //change float to long
    POINT pt;
    pt.x = (LONG)(out.x + 0.5f);
    pt.y = (LONG)(out.y + 0.5f);
    if ( pOutPt != NULL )
    {
        *pOutPt = pt;
    }
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void D3DUtility::CreateEmptyTexture( const LPDIRECT3DDEVICE9 pDevice, 
                                    SIZE size,
                                    LPDIRECT3DTEXTURE9* ppTexture )
{
    if ( ppTexture == NULL )
    {
        return;
    }
    LPDIRECT3DTEXTURE9 pTexture = NULL ;
    pDevice->CreateTexture(
        size.cx, 
        size.cy, 
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_MANAGED,
        &pTexture,
        NULL);
    *ppTexture = pTexture;
}

void D3DUtility::BeginUpdateTexture( 
                               IN LPDIRECT3DTEXTURE9 pTexture,
                               OUT DEVICECONTEXT& deviceContext)
{
    HDC hdc = NULL;
    IDirect3DSurface9 * pTheSurface;
    pTexture->AddDirtyRect(NULL);

    if ( FAILED(pTexture->GetSurfaceLevel(0, &pTheSurface)))
    {
        return;
    }
    if (FAILED(pTheSurface->GetDC( &hdc )))
    {
        pTheSurface->Release();
        return;
    }
    
    deviceContext.surfaceDC = hdc;
    D3DSURFACE_DESC surfaceDesc;
    pTheSurface->GetDesc(&surfaceDesc);
    deviceContext.rect.left = deviceContext.rect.top = 0;
    deviceContext.rect.right = surfaceDesc.Width;
    deviceContext.rect.bottom = surfaceDesc.Height;
    deviceContext.pSurface = pTheSurface;
}

void D3DUtility::EndUpdateTexture( 
                              IN LPDIRECT3DTEXTURE9 pTexture,
                             IN DEVICECONTEXT& deviceContext)
{
    UNREFERENCED_PARAMETER(pTexture);
    if ( deviceContext.surfaceDC != NULL )
    {
        deviceContext.pSurface->ReleaseDC( deviceContext.surfaceDC );
    }
    SAFE_RELEASE(( deviceContext.pSurface));
    //pTexture->SetAutoGenFilterType(D3DTEXF_ANISOTROPIC);
    //pTexture->GenerateMipSubLevels();
}

void D3DUtility::updateDesktopTexture( IN LPDIRECT3DTEXTURE9 pTexture,
                                       IN HDC hdc,
                                       IN const RECT& rect,
                                       IN HWND hwnd)
{
	UNREFERENCED_PARAMETER(pTexture);
    //AnfangD3DUtility::updateSurface(TheSurface);
    HWND hDesktopWnd = NULL;//GetDesktopWindow();
    hDesktopWnd = FindWindow(MEDIAUIWINDOWNAME, WINDOWFORMCLASSNAME);
    //hDesktopWnd = (HWND)0x10010;
    HDC hdcScreen = GetDC(hwnd);
    RECT rectScr = {0};
    ::GetWindowRect( hwnd, &rectScr );
    

    ::StretchBlt( hdc, 
        0,0,
        rect.right - rect.left, 
        rect.bottom - rect.top,
        hdcScreen,
        rect.left,rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        SRCCOPY|CAPTUREBLT
        );

    ReleaseDC(hwnd, hdcScreen);
    //if (!BitBlt(hdc, 
    //    0,0, 
    //    rect.right - rect.left, 
    //    rect.bottom - rect.top,
    //    hdcScreen, 
    //    0,0,
    //    SRCCOPY))
    //{
    //    return;
    //}
}

void D3DUtility::CopyTexture( 
                        IN LPDIRECT3DTEXTURE9 pTextureSrc,
                        IN LPDIRECT3DTEXTURE9 pTextureDst,
                        IN const LPRECT pRectSrc,
                        IN const LPRECT pRectDst)
{
    HDC hdcSrc = NULL;
    RECT rectSrc = {0};
    HDC hdcDst = NULL;
    RECT rectDst = {0};
    IDirect3DSurface9 * pTheSurfaceSrc;
    IDirect3DSurface9 * pTheSurfaceDst;

    if ( FAILED(pTextureSrc->GetSurfaceLevel(0, &pTheSurfaceSrc)))
    {
        return;
    }
    if (FAILED(pTheSurfaceSrc->GetDC( &hdcSrc )))
    {
        pTheSurfaceSrc->Release();
        return;
    }


    D3DSURFACE_DESC surfaceDesc;
    pTheSurfaceSrc->GetDesc(&surfaceDesc);
    rectSrc.right = surfaceDesc.Width;
    rectSrc.bottom = surfaceDesc.Height;

    pTextureDst->AddDirtyRect(NULL);
    if ( FAILED(pTextureDst->GetSurfaceLevel(0, &pTheSurfaceDst)))
    {
        pTheSurfaceSrc->ReleaseDC(hdcSrc);
        pTheSurfaceSrc->Release();
        return;
    }
    if (FAILED(pTheSurfaceDst->GetDC( &hdcDst )))
    {
        pTheSurfaceSrc->ReleaseDC(hdcSrc);
        pTheSurfaceSrc->Release();
        pTheSurfaceDst->Release();
        return;
    }

    pTheSurfaceDst->GetDesc(&surfaceDesc);
    rectDst.right = surfaceDesc.Width;
    rectDst.bottom = surfaceDesc.Height;
    if ( pRectSrc != NULL )
    {
        rectSrc = *pRectSrc;
    }

    if ( pRectDst != NULL )
    {
        rectDst = *pRectDst;
    }
    
    ::StretchBlt( hdcDst, 
        rectDst.left,
        rectDst.top,         
        rectDst.right - rectDst.left, 
        rectDst.bottom - rectDst.top,
        hdcSrc,
        rectSrc.left,
        rectSrc.top,
        rectSrc.right - rectSrc.left,
        rectSrc.bottom - rectSrc.top,
        SRCCOPY
        );

    pTheSurfaceSrc->ReleaseDC(hdcSrc);
    pTheSurfaceSrc->Release();
    pTheSurfaceDst->ReleaseDC(hdcDst);
    pTheSurfaceDst->Release();
}

BYTE GetPixel ( BYTE *lpBits, int x,int y, int nImageWidth,int BitsPerPixel );
PBITMAPINFO CreateBitmapInfoStruct( HBITMAP hBmp);
VOID D3DUtility::CreateBMPFile( LPTSTR pszFile,
                               HBITMAP hBMP, HDC hDC)
{

    PBITMAPINFO pbi = CreateBitmapInfoStruct( hBMP );

    {
        HANDLE hf;                 // file handle  
        BITMAPFILEHEADER hdr;       // bitmap file-header  
        PBITMAPINFOHEADER pbih;     // bitmap info-header  
        LPBYTE lpBits;              // memory pointer  
        DWORD dwTotal = 0;              // total count of bytes  
        BYTE *hp = NULL;                   // byte pointer  
        DWORD dwTmp = 0; 

        pbih = (PBITMAPINFOHEADER) pbi; 
        lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);


        // Retrieve the color table (RGBQUAD array) and the bits  
        // (array of palette indices) from the DIB.  
        GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
            DIB_RGB_COLORS);

        // Create the .BMP file.  
        hf = CreateFile(pszFile, 
            GENERIC_READ | GENERIC_WRITE, 
            (DWORD) 0, 
            NULL, 
            CREATE_ALWAYS, 
            FILE_ATTRIBUTE_NORMAL, 
            (HANDLE) NULL); 

        hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
        // Compute the size of the entire file.  
        hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
            pbih->biSize + pbih->biClrUsed 
            * sizeof(RGBQUAD) + pbih->biSizeImage); 
        hdr.bfReserved1 = 0; 
        hdr.bfReserved2 = 0; 

        // Compute the offset to the array of color indices.  
        hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
            pbih->biSize + pbih->biClrUsed 
            * sizeof (RGBQUAD); 

        // Copy the BITMAPFILEHEADER into the .BMP file.  
        WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
            (LPDWORD) &dwTmp,  NULL); 

        // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
        WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
            + pbih->biClrUsed * sizeof (RGBQUAD), 
            (LPDWORD) &dwTmp, ( NULL));
        dwTotal = pbih->biSizeImage;
        hp = lpBits; 
       WriteFile(hf, (LPSTR) hp, pbih->biSizeImage, (LPDWORD) &dwTmp,NULL);


        // Close the .BMP file.  
        CloseHandle(hf);


        // Free memory.  
        GlobalFree((HGLOBAL)lpBits);

    }
    LocalFree(pbi);


}

PBITMAPINFO CreateBitmapInfoStruct( HBITMAP hBmp)
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height.  
    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);


    // Convert the color format to a count of bits.  
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure  
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
    // data structures.)  

    if (cClrBits < 24) 
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
        sizeof(BITMAPINFOHEADER) + 
        sizeof(RGBQUAD) * (1<< cClrBits)); 

    // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

    else 
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
        sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure.  

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag.  
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color  
    // indices and store the result in biSizeImage.  
    // The width must be DWORD aligned unless the bitmap is RLE 
    // compressed. 
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
        * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the  
    // device colors are important.  
    pbmi->bmiHeader.biClrImportant = 0; 
    return pbmi; 
};
