/*!
* @file SdkWICImageHelper.h 
* 
* @brief This file defines functions to operate image file with WIC.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/03
*/

#ifdef __cplusplus
#ifndef _SDKWICIMAGEHELPER_H_
#define _SDKWICIMAGEHELPER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief The type of image which can be processed WIC.
*/
typedef enum _WIC_IMAGE_TYPE
{
    WIC_IMAGE_TYPE_UNKNOWN  = 0,
    WIC_IMAGE_TYPE_PNG      = 1,
    WIC_IMAGE_TYPE_IMAGE    = 2,
    WIC_IMAGE_TYPE_BITMAP   = 3,
    WIC_IMAGE_TYPE_GIF      = 4,

} WIC_IMAGE_TYPE;

/*!
* @brief SdkWICImageHelper class definition.
*/
class CLASS_DECLSPEC SdkWICImageHelper
{
public:

    /*!
    * @brief If you want to use WIC, you should first call this method before you use
    *        any interfaces of WIC.
    *
    * @return S_OK if succeeds, otherwise E_FAIL.
    */
    static HRESULT WICInitialize();

    /*!
    * @brief Call this method to release memory of WIC, if you no longer use WIC interfaces.
    */
    static void WICUninitialize();

    /*!
    * @brief Save WIC Bitmap to file.
    *
    * @param pFileName      [I/ ] The file name.
    * @param pWICBitmap     [I/ ] The bitmap source.
    *
    * @return TRUE if succeeds, otherwise FALSE.
    */
    static BOOL SaveWICBitmapToFile(PCWSTR pFileName, IWICBitmapSource *pWICBitmap);

    /*!
    * @brief Create HBITMAP from specified WIC Bitmap source.
    *
    * @param hDC                [I/ ] The handle to Device context.
    * @param pWICBitmapSource   [I/ ] The WIC bitmap source.
    *
    * @return non-zero if succeed, otherwise returns zero.
    */
    static HBITMAP CreateHBITMAPFromIWICBitmap(HDC hDC, IWICBitmapSource *pWICBitmapSource);

    /*!
    * @brief The default constructor function.
    */
    SdkWICImageHelper();

    /*!
    * @brief The default destructor function.
    */
    ~SdkWICImageHelper();

    /*!
    * @brief Load a image from a specified file name.
    *
    * @param lpfile         [I/ ] The pointer to file name.
    * @param uDestWidth     [I/ ] The destination width.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    BOOL LoadFromFile(LPCWSTR lpfile, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Load a image from HBITMAP.
    *
    * @param hBitmap        [I/ ] The handle to HBITMAP.
    * @param uDestWidth     [I/ ] The destination width.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    BOOL LoadFromHBITMAP(HBITMAP hBitmap, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Load a image from a resource.
    *
    * @param uResId         [I/ ] The identifier of resource.
    * @param hModule        [I/ ] Indicates thats load image from.which module, default value is NULL.
    * @param uDestWidth     [I/ ] The destination width.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    BOOL LoadFromResource(UINT uResId, HMODULE hModule = NULL, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Get the handle of the bitmap. If you call this method to get the HBITMAP
    *        of image, you should take charge of deleting the GDI object.
    *
    * @return The handle to HBITMAP.
    */
    HBITMAP GetHBITMAP();

    /*!
    * @brief Get the format converter of the image that presents a bitmap source.
    *
    * @param ppConvertedSourceBitmap     [ /O] The pointer to pointer to IWICFormatConverter interface.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetFormatConverter(OUT IWICFormatConverter **ppConvertedSourceBitmap);

    /*!
    * @brief Set the image type, which indicates which image can be processed by current class.
    *
    * @param type           [I/ ] The image type.
    */
    void SetImageType(WIC_IMAGE_TYPE type);

    /*!
    * @brief Get the width of image.
    *
    * @return The width of image.
    */
    UINT GetWidth();

    /*!
    * @brief Get the height of image.
    *
    * @return The height of image.
    */
    UINT GetHeight();

    /*!
    * @brief Create scaled bitmap from resource bitmap.
    *
    * @param uWidth     [I/ ] Source bitmap.
    * @param uHeight    [I/ ] Destination bitmap.
    *
    * @return S_OK if succeeds.
    */
    HRESULT CreateScalerWICBitmapSource(UINT32 uWidth, UINT32 uHeight);

    /*!
    * @brief Clear the bitmap data. If you want to user the object of this class to
    *        reload bitmap from file, bitmap, or resource, you should call this method
    *        to release old image data.
    */
    void ClearImageData();

private:

    /*!
    * @brief Clone the bitmap source.
    *
    * @param pWICBitmapSource   [I/ ] Source bitmap.
    * @param ppWICBitmapDest    [I/ ] Destination bitmap.
    *
    * @return S_OK if succeeds.
    */
    HRESULT CloneWICBitmapSource(IWICBitmapSource *pWICBitmapSource, IWICBitmapSource **ppWICBitmapDest);

    /*!
    * @brief Get image information.
    */
    void GetImageInfos();

    /*!
    * @brief Get the image data from resource.
    *
    * @param uResId         [I/ ] The resource id.
    * @param hModule        [I/ ] The module handler.
    * @param pData          [ /O] The loaded output data.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    DWORD LoadResourceData(UINT uResId, HMODULE hModule, OUT PVOID* pData);

    /*!
    * @brief Create the IWICBitmapScaler interface.
    *
    * @param pWICBitmapSource   [I/ ] The bitmap source.
    * @param uDestWidth         [I/ ] he destination width.
    * @param uDestHeight        [I/ ] he destination height.
    *
    * @return IWICBitmapScaler instance if succeeds, otherwise return NULL.
    */
    IWICBitmapScaler* CreateBitmapScaler(IN IWICBitmapSource* pWICBitmapSource, UINT32 uDestWidth, UINT32 uDestHeight);

private:

    UINT                         m_uImageWidth;                 // The image width.
    UINT                         m_uImageHeight;                // The image height.
    HBITMAP                      m_hBitmap;                     // The handle of the bitmap.
    WIC_IMAGE_TYPE               m_curImageType;                // The current image type
    IWICFormatConverter         *m_pConvertedSourceBitmap;      // The format converter.
    static IWICImagingFactory   *s_pImagingFactory;             // The WIC image factory.
};

END_NAMESPACE_UTILITIES

#endif // _SDKWICIMAGEHELPER_H_
#endif // __cplusplus

