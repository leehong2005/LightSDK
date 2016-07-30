/*!
* @file SdkImageManager.cpp
* 
* @brief This file defines the Image manager.
* 
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2009/06/16 12:00
*/

#include "stdafx.h"
#include "SdkImagesManager.h"

USING_NAMESPACE_UTILITIES

/*!
* @brief The image encoder format, is associated with IMAGE_SAVETYPE enumeration
*/
const LPWSTR g_szImageFormat[] = 
{
    L"image/bmp",
    L"image/jpeg",
    L"image/gif",
    L"image/x-emf",
    L"image/x-wmf",
    L"image/tiff",
    L"image/png",
    L"image/x-icon",
};

ULONG_PTR SdkImagesManager::m_gdiplusToken = 0;

//////////////////////////////////////////////////////////////////////////

void SdkImagesManager::GdiplusInitialize()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

//////////////////////////////////////////////////////////////////////////

void SdkImagesManager::UnGdiplusInitialize()
{
    GdiplusShutdown(m_gdiplusToken);
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkImagesManager::CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
    UNREFERENCED_PARAMETER(crTransparent);
    HDC hdcMem = NULL;
    HDC hdcMemMask = NULL;
    HBITMAP hbmMask = NULL;
    BITMAP bm;

    // Create a monochrome (1 bit) mask bitmap
    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some DCs that are compatible with the display driver.
    hdcMem = CreateCompatibleDC(NULL);
    hdcMemMask = CreateCompatibleDC(NULL);

    HGDIOBJ hbmOldColour = SelectObject(hdcMem, hbmColour);
    HGDIOBJ hbmOldMask = SelectObject(hdcMemMask, hbmMask);

    // Set the background color of the color image to the color you want to be transparent.
    SetBkColor(hdcMem, 0);

    // Copy the color image to the B+W mask. Everything with the background ends up white while
    // everything else ends up black.
    BitBlt(hdcMemMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent color in our
    // original color image to black so the transparency effect will work right.
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemMask, 0, 0, SRCINVERT);

    // Clean up these GDI resources
    SelectObject(hdcMemMask, hbmOldMask);
    SelectObject(hdcMem, hbmOldColour);
    DeleteObject(hdcMemMask);
    DeleteObject(hdcMem);

    return hbmMask;
}

//////////////////////////////////////////////////////////////////////////

SdkImagesManager::SdkImagesManager(HINSTANCE hInst)
{
    m_bLoadImage    = FALSE;
    m_pGdiImage     = NULL;
    m_hInst         = hInst;
    m_imageSaveType = SAVETYPE_PNG;
}

//////////////////////////////////////////////////////////////////////////

SdkImagesManager::SdkImagesManager(IN const SdkImagesManager& imagesManagerSrc)
{
    m_pGdiImage   = NULL;
    m_bLoadImage  = imagesManagerSrc.m_bLoadImage;
    m_hInst       = imagesManagerSrc.m_hInst;
    m_imageSaveType = imagesManagerSrc.m_imageSaveType;

    if ( NULL != imagesManagerSrc.m_pGdiImage )
    {
        UINT nWidth = imagesManagerSrc.m_pGdiImage->GetWidth();
        UINT nHeight = imagesManagerSrc.m_pGdiImage->GetWidth();
        m_pGdiImage = new Bitmap(nWidth, nHeight, PixelFormat32bppARGB);
        Graphics g(m_pGdiImage);
        g.Clear(0x00FFFFFF);
        g.DrawImage(imagesManagerSrc.m_pGdiImage, 0, 0, nWidth, nHeight);
    }

    CheckLastStatus();
}

//////////////////////////////////////////////////////////////////////////

SdkImagesManager::~SdkImagesManager()
{
    DeleteImage();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::LoadImageFromResId(UINT resId)
{
    LPVOID presData = NULL;
    DWORD dwResSize = LoadResourceData(resId, _T("PNG"), this->m_hInst, &presData);
    if ( dwResSize <= 0 )
    {
        return FALSE;
    }

    DeleteImage();

    // +1 hglobal needs GloablFree to release
    HGLOBAL hglobal = GlobalAlloc(GMEM_FIXED, dwResSize);
    if ( NULL != hglobal )
    {
        // +2 hglobal
        LPVOID lpBuffer = GlobalLock(hglobal);
        // +3 pistream
        IStream* pistream = NULL;
        if ( NULL != lpBuffer )
        {
            CopyMemory(lpBuffer, presData, dwResSize);
            if ( SUCCEEDED(CreateStreamOnHGlobal(hglobal, FALSE, &pistream)) )
            {
                m_pGdiImage = Bitmap::FromStream(pistream);
            }
        }

        // -3 pistream
        SAFE_RELEASE(pistream);
        // -2 hglobal This function has no effect on memory objects allocated with GMEM_FIXED
        GlobalUnlock(hglobal);
        // -1 hglobal
        GlobalFree(hglobal);
    }

    SAFE_DELETE_ARRAY(presData);

    return CheckLastStatus();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::LoadImageFromFile(LPCTSTR pszfilePath)
{
    if ( (pszfilePath != NULL) && (PathFileExists(pszfilePath)) )
    {
        DeleteImage();
        m_pGdiImage = Bitmap::FromFile(pszfilePath);
    }

    return CheckLastStatus();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::LoadImageFromHBITMAP(HBITMAP hBitmap)
{
    BITMAP bmp = { 0 };
    BYTE *piexlsSrc = NULL;
    if ( (FALSE == GetBitmapBitsData(hBitmap, &bmp, (LPVOID*)&piexlsSrc)) || (0 != bmp.bmType) )
    {
        return FALSE;
    }

    DeleteImage();
    // Create an GDI+ Bitmap has the same dimensions with hbitmap
    m_pGdiImage = new Bitmap(bmp.bmWidth, bmp.bmHeight, PixelFormat32bppPARGB);

    // Access to the Gdiplus::Bitmap's pixel data
    BitmapData bitmapData = { 0 };
    Gdiplus::Rect rect(0, 0, bmp.bmWidth, bmp.bmHeight);

    Status state = m_pGdiImage->LockBits(
        &rect,
        ImageLockModeRead | ImageLockModeWrite,
        PixelFormat32bppPARGB,
        &bitmapData);

    if ( Ok != state )
    {
        SAFE_DELETE_ARRAY(piexlsSrc);
        SAFE_DELETE(this->m_pGdiImage);
        return FALSE;
    }

    BYTE *pixelsDest = (BYTE*)bitmapData.Scan0;
    int nLinesize = bmp.bmWidth * sizeof(UINT);
    int nHeight = bmp.bmHeight;

    // Copy pixel data from HBITMAP by bottom-up.
    for ( int y = 0; y < nHeight; y++ )
    {
        memcpy_s(
            (pixelsDest + y * nLinesize),
            nLinesize,
            (piexlsSrc + (nHeight - y - 1) * nLinesize),
            nLinesize);
    }

    // Copy the data in temporary buffer to this->m_pGdiImage
    if ( Ok != m_pGdiImage->UnlockBits(&bitmapData) )
    {
        SAFE_DELETE(this->m_pGdiImage);
    }

    SAFE_DELETE_ARRAY(piexlsSrc);
    return this->CheckLastStatus();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::LoadImageFromHICON(HICON hIcon)
{
    if ( NULL != hIcon )
    {
        DeleteImage();
        m_pGdiImage = Bitmap::FromHICON(hIcon);
    }

    return CheckLastStatus();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::SaveImageToFile(IN LPCWSTR lpfileName)
{
    return SaveImageToFile(lpfileName, m_imageSaveType);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::SaveImageToFile(IN LPCWSTR lpfileName, IMAGE_SAVETYPE imageSaveType)
{
    if ( (NULL == this->m_pGdiImage) || (NULL == lpfileName) )
    {
        return FALSE;
    }

    WCHAR szFolderPath[MAX_PATH] = { 0 };
    wcscpy_s(szFolderPath, MAX_PATH, lpfileName);
    PathRemoveFileSpec(szFolderPath);

    // If the folder is not existing.
    if ( FALSE == PathFileExists(szFolderPath) )
    {
        SHCreateDirectory(NULL, szFolderPath);
    }

    CLSID imgClsid;
    WCHAR szFileExtension[10] = { 0 };
    if ( !GetEncoderClsid(g_szImageFormat[imageSaveType], &imgClsid, szFileExtension, 10) )
    {
        return FALSE;
    }

    WCHAR szFilePath[MAX_PATH] = { 0 };
    wcscpy_s(szFilePath, MAX_PATH, lpfileName);
    // The file has no extension
    if ( 0 == wcslen(PathFindExtension(lpfileName)) )
    {
        _wcslwr_s(szFileExtension, _countof(szFileExtension) / sizeof(TCHAR) + 1);
        PathAddExtension(szFilePath, &szFileExtension[1]);
    }

    Status status = m_pGdiImage->Save(szFilePath, &imgClsid, NULL);
    return (status == Gdiplus::Ok) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::ResizeTo(FLOAT fWidth, FLOAT fHeight)
{
    if ( NULL == this->m_pGdiImage )
    {
        return FALSE;
    }

    UINT32 nWidth = (UINT32)fWidth;
    UINT32 nHeight = (UINT32)fHeight;

    // +1 newImg
    Bitmap *newImg = new Bitmap(nWidth, nHeight, PixelFormat32bppARGB);
    if ( (NULL == newImg) || (Gdiplus::Ok != newImg->GetLastStatus()) )
    {
        return FALSE;
    }

    // +2 g
    Graphics *g = Graphics::FromImage(newImg);
    g->DrawImage(m_pGdiImage, 0, 0, nWidth, nHeight);
    this->DeleteImage();
    this->m_pGdiImage = newImg->Clone(0, 0, nWidth, nHeight, PixelFormatDontCare);

    // -2 g
    SAFE_DELETE(g);
    // -1 newImg
    SAFE_DELETE(newImg);

    return this->CheckLastStatus();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::ResizeTo(FLOAT fRatio)
{
    if ( m_bLoadImage && (m_pGdiImage != NULL) && (fRatio > 0) )
    {
        REAL fWidth = (REAL)(m_pGdiImage->GetWidth() * fRatio);
        REAL fHeight = (REAL)(m_pGdiImage->GetHeight() * fRatio);
        return ResizeTo(fWidth, fHeight);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagesManager::DrawImage(IN HDC hDC, IMAGE_WRAPMODE imageMode)
{
    if ( NULL == m_pGdiImage )
    {
        return;
    }

    HWND hWnd = WindowFromDC(hDC);
    if ( (NULL == hWnd) || (FALSE == IsWindow(hWnd)) )
    {
        return;
    }

    DrawImage(hDC, hWnd, imageMode);
}

//////////////////////////////////////////////////////////////////////////

void SdkImagesManager::DrawImage(HDC hDC, IN HWND hWnd, IMAGE_WRAPMODE imageMode)
{
    if ( (NULL == m_pGdiImage) || (FALSE == IsWindow(hWnd)) )
    {
        return;
    }

    RECT rect;
    GetClientRect(hWnd, &rect);

    LONG nLeft = 0;
    LONG nTop = 0;
    LONG nWidth = RECT_WIDTH(rect);
    LONG nHeight = RECT_HEIGHT(rect);

    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hMemBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
    HGDIOBJ hOldObj = SelectObject(hMemDC, hMemBitmap);
    Graphics *g = new Graphics(hMemDC);
    g->Clear((ARGB)Color::White);

    Gdiplus::Brush *pimgBrush = NULL;

    switch (imageMode)
    {
    case MODE_TILE:
        pimgBrush = new Gdiplus::TextureBrush(this->m_pGdiImage, WrapModeTile);
        g->FillRectangle(pimgBrush, 0, 0, nWidth, nHeight);
        break;

    case MODE_TILEFLIPX:
        pimgBrush = new Gdiplus::TextureBrush(this->m_pGdiImage, WrapModeTileFlipX);
        g->FillRectangle(pimgBrush, 0, 0, nWidth, nHeight);
        break;

    case MODE_TILEFLIPY:
        pimgBrush = new Gdiplus::TextureBrush(this->m_pGdiImage, WrapModeTileFlipY);
        g->FillRectangle(pimgBrush, 0, 0, nWidth, nHeight);
        break;

    case MODE_TILEFLIPXY:
        pimgBrush = new Gdiplus::TextureBrush(this->m_pGdiImage, WrapModeTileFlipXY);
        g->FillRectangle(pimgBrush, 0, 0, nWidth, nHeight);
        break;

    case MODE_CLAMP:
        pimgBrush = new Gdiplus::TextureBrush(this->m_pGdiImage, WrapModeClamp);
        nWidth = MIN((UINT)nWidth, m_pGdiImage->GetWidth());
        nHeight = MIN((UINT)nHeight, m_pGdiImage->GetHeight());
        g->DrawImage(m_pGdiImage, nLeft, nTop, nWidth, nHeight);
        break;

    case MODE_STRETCH:
        g->DrawImage(m_pGdiImage, nLeft, nTop, nWidth, nHeight);
        break;

    case MODE_CENTER:
        nLeft = (nWidth - (LONG)m_pGdiImage->GetWidth()) / 2;
        nTop = (nHeight - (LONG)m_pGdiImage->GetHeight()) / 2;
        g->DrawImage(m_pGdiImage, nLeft, nTop);
        break;
    }

    BitBlt(hDC, 0, 0, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldObj);
    DeleteObject(hMemBitmap);
    DeleteObject(hMemDC);

    SAFE_DELETE(pimgBrush);
    SAFE_DELETE(g);
}

//////////////////////////////////////////////////////////////////////////

inline BOOL SdkImagesManager::HaveLoadedImage()
{
    return m_bLoadImage;
}

//////////////////////////////////////////////////////////////////////////

inline void SdkImagesManager::SetImageSaveType(IMAGE_SAVETYPE imageSaveType)
{
    m_imageSaveType = imageSaveType;
}

//////////////////////////////////////////////////////////////////////////

inline IMAGE_SAVETYPE SdkImagesManager::GetImageSaveType()
{
    return m_imageSaveType;
}

//////////////////////////////////////////////////////////////////////////

inline DWORD SdkImagesManager::GetWidth()
{
    DWORD dwWidth = 0;
    if ( NULL != m_pGdiImage )
    {
        dwWidth = m_pGdiImage->GetWidth();
    }

    return dwWidth;
}

//////////////////////////////////////////////////////////////////////////

inline DWORD SdkImagesManager::GetHeight()
{
    DWORD dwHeight = 0;
    if ( NULL != m_pGdiImage )
    {
        dwHeight = m_pGdiImage->GetHeight();
    }

    return dwHeight;
}

//////////////////////////////////////////////////////////////////////////

inline Bitmap* SdkImagesManager::GetImage() const
{
    return m_pGdiImage;
}

//////////////////////////////////////////////////////////////////////////

SdkImagesManager& SdkImagesManager::operator =(IN const SdkImagesManager& pImageManagerSrc)
{
    if ( this == &pImageManagerSrc )
    {
        return *this;
    }

    if ( NULL != pImageManagerSrc.m_pGdiImage )
    {
        DeleteImage();
        UINT nWidth = pImageManagerSrc.m_pGdiImage->GetWidth();
        UINT nHeight = pImageManagerSrc.m_pGdiImage->GetWidth();

        m_pGdiImage = new Bitmap(nWidth, nHeight, PixelFormat32bppARGB);

        Graphics g(m_pGdiImage);
        g.Clear(0x00FFFFFF);
        g.DrawImage(pImageManagerSrc, 0, 0, nWidth, nHeight);

        CheckLastStatus();
    }

    return *this;
}

SdkImagesManager::operator Bitmap*() const
{
    return m_pGdiImage;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkImagesManager::GetHBITMAP()
{
    HBITMAP bitmap = NULL;
    if (m_pGdiImage != NULL)
    {
        m_pGdiImage->GetHBITMAP(0x00FFFFFF, &bitmap);    // Color::Transparent
    }

    return bitmap;
}

//////////////////////////////////////////////////////////////////////////

inline void SdkImagesManager::DeleteImage()
{
    SAFE_DELETE(m_pGdiImage);
    m_bLoadImage = FALSE;
}

//////////////////////////////////////////////////////////////////////////

inline BOOL SdkImagesManager::CheckLastStatus()
{
    if ( (NULL != m_pGdiImage) && (Gdiplus::Ok == m_pGdiImage->GetLastStatus()) )
    {
        m_bLoadImage = TRUE;
    }
    else
    {
        DeleteImage();
    }

    return m_bLoadImage;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::GetBitmapBitsData(IN HBITMAP hBitmap, OUT LPBITMAP lpbitmap, OUT LPVOID* pbitbuf)
{
    if ( (NULL == hBitmap) || (NULL == pbitbuf) )
    {
        return FALSE;
    }

    *pbitbuf  = NULL;
    BITMAP bmp = { 0 };
    if ( 0 == GetObject(hBitmap, sizeof(BITMAP), &bmp) )
    {
        return FALSE;
    }

    // Although we can get bitmap data address by bmp.bmBits member of
    // BITMAP which is got by GetObject function sometime,
    // we can determine the bitmap data in the HBITMAP is arranged bottom-up or top-down,
    // so we should always use GetDIBits to get bitmap data.
    *pbitbuf = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];
    if ( NULL == *pbitbuf )
    {
        return FALSE;
    }

    BITMAPINFO bmpInfo = { 0 };
    // We should initialize the first six members of BITMAPINFOHEADER structure.
    // A bottom-up DIB is specified by setting the height to a positive number,
    // while a top-down DIB is specified by setting the height to a negative number.
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = bmp.bmWidth;
    bmpInfo.bmiHeader.biHeight = bmp.bmHeight;
    bmpInfo.bmiHeader.biPlanes = bmp.bmPlanes;
    bmpInfo.bmiHeader.biBitCount = bmp.bmBitsPixel;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    HDC hdcScreen = CreateDC(L"DISPLAY", NULL, NULL,NULL);
    LONG cbCopied = GetDIBits(
        hdcScreen,
        hBitmap,
        0,
        bmp.bmHeight,
        *pbitbuf,
        &bmpInfo,
        DIB_RGB_COLORS);

    DeleteDC(hdcScreen);

    if ( NULL != lpbitmap )
    {
        *lpbitmap = bmp;
    }

    return ( 0 != cbCopied );
}

//////////////////////////////////////////////////////////////////////////

DWORD SdkImagesManager::LoadResourceData(
    UINT nResID,
    IN LPCWSTR lpcsType,
    IN HMODULE hModule,
    OUT LPVOID* ppData)
{
    *ppData = NULL;

    LPCWSTR lpName = MAKEINTRESOURCE(nResID);
    if ( FALSE == IS_INTRESOURCE(lpName) )
    {
        return 0;
    }

    HRSRC hrsrc = FindResource(hModule, lpName, lpcsType);
    if ( NULL == hrsrc )
    {
        return 0;
    }

    DWORD dwDataSize = SizeofResource(hModule, hrsrc);
    if ( dwDataSize <= 0 )
    {
        return 0;
    }

    HGLOBAL hGlobal = LoadResource(hModule, hrsrc);
    if ( NULL != hGlobal )
    {
        // Need NOT to free pResData
        LPVOID pResData = LockResource(hGlobal);
        if ( NULL != pResData )
        {
            *ppData = new BYTE[dwDataSize];
            if ( NULL != *ppData )
            {
                memcpy(*ppData, pResData, dwDataSize);
            }
        }

        UnlockResource(hGlobal);
        FreeResource(hGlobal);
    }

    return (NULL != *ppData) ? dwDataSize : 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagesManager::GetEncoderClsid(
    LPCTSTR pszformat,
    OUT CLSID* pclsid,
    OUT LPWSTR szFileNameExtension,
    IN DWORD dwSize)
{
    if ( NULL == pszformat )
    {
        return FALSE;
    }

    BOOL bResult = FALSE;
    UINT numEncoders = 0;                   // number of image decoders
    UINT size = 0;                          // size, in bytes, of the image decoder array
    ImageCodecInfo* pImageCodecInfo = NULL;

    // How many decoders are there? How big (in bytes) is the array of all ImageCodecInfo objects?
    GetImageEncodersSize(&numEncoders, &size);

    if ( (0 == size) || (0 == numEncoders) )
    {
        return FALSE;
    }

    pImageCodecInfo = (ImageCodecInfo*)malloc(size);
    if ( (NULL == pImageCodecInfo) ||
         (Gdiplus::Ok != GetImageEncoders(numEncoders, size, pImageCodecInfo)) )
    {
        return FALSE;
    }

    for ( UINT i = 0; i < numEncoders; i++ )
    {
        if ( 0 == wcscmp(pImageCodecInfo[i].MimeType, pszformat) )
        {
            if ( (NULL != szFileNameExtension) && (dwSize > 0) )
            {
                wstring strFileExtension(pImageCodecInfo[i].FilenameExtension);
                int length = strFileExtension.find_first_of(L';');
                length = (length < 0) ? strFileExtension.length() : length;
                wcsncpy_s(
                    szFileNameExtension,
                    dwSize, pImageCodecInfo[i].FilenameExtension,
                    length);

                bResult = TRUE;
            }

            if ( NULL != pclsid )
            {
                *pclsid = pImageCodecInfo[i].Clsid;
                bResult = TRUE;
            }
            break;
        }
    }

    free(pImageCodecInfo);
    return bResult;
}