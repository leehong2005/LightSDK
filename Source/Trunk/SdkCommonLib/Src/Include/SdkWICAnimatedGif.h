/*!
* @file SdkWICAnimatedGif.h 
* 
* @brief This file defines functions to operate animated GIF file.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/03
*/

#ifdef __cplusplus
#ifndef _SDKWICANIMATEDGIF_H_
#define _SDKWICANIMATEDGIF_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief The GIF_RECT_VALUETYPE enumeration.
*/
typedef enum _GIF_RECT_VALUETYPE
{
    GIF_RECT_VALUETYPE_UNKNOWN  = 0,
    GIF_RECT_VALUETYPE_LEFT     = 1,
    GIF_RECT_VALUETYPE_TOP      = 2,
    GIF_RECT_VALUETYPE_RIGHT    = 3,
    GIF_RECT_VALUETYPE_BOTTOM   = 4,

} GIF_RECT_VALUETYPE;

/*!
* @brief The DISPOSAL_METHODS enumeration.
*/
typedef enum _DISPOSAL_METHODS
{
    DM_UNDEFINED            = 0,
    DM_NONE                 = 1,
    DM_BACKGROUND           = 2,
    DM_PREVIOUS             = 3,

} DISPOSAL_METHODS;

/*!
* @brief The WIC_GIF_TYPE enumeration.
*/
typedef enum _WIC_GIF_TYPE
{
    WIC_GIF_TYPE_UNKNOWN    = 0,
    WIC_GIF_TYPE_GIF        = 1,
    WIC_GIF_TYPE_IMAGE      = 2,
    WIC_GIF_TYPE_BITMAP     = 3,
    WIC_GIF_TYPE_PNG        = 4,

} WIC_GIF_TYPE;

/*!
* @brief The class is used to operate GIF files.
*/
class CLASS_DECLSPEC SdkWICAnimatedGif
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
    * @brief The default constructor function.
    */
    SdkWICAnimatedGif();

    /*!
    * @brief The default destructor function.
    */
    ~SdkWICAnimatedGif();

    /*!
    * @brief Load GIF from a specified file name.
    *
    * @param lpfile     [I/ ] The pointer to file name.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL LoadFromFile(IN LPCWSTR lpfile);

    /*!
    * @brief Load GIF from a resource.
    *
    * @param uResId     [I/ ] The identifier of resource.
    * @param hModule    [I/ ] Indicates thats load GIF from.which module, default value is NULL.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL LoadFromResource(UINT uResId, HMODULE hModule = NULL);

    /*!
    * @brief Judge loaded file is gif or image.
    * 
    * @param pWicBitmapDecoder      [I/ ] Loaded file bitmap decoder.
    */
    BOOL IsGif(IWICBitmapDecoder *pWicBitmapDecoder);

    /*!
    * @brief Set the image type, which indicates which image can be processed by current class.
    *
    * @param type           [I/ ] The image type.
    */
    void SetImageType(WIC_GIF_TYPE type);

    /*!
    * @brief Get the resource type.
    *
    * @return WIC_GIF_TYPE value.
    */
    WIC_GIF_TYPE GetImageType();

    /*!
    * @brief Get the format converter of the image that presents a bitmap source.
    *
    * @param ppConvertedSourceBitmap     [ /O] The pointer to pointer to IWICFormatConverter interface.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetFormatConverter(OUT IWICFormatConverter **ppConvertedSourceBitmap);

    /*!
    * @brief Get next frame format convert, presents a frame bitmap source.
    *
    * @param ppConvertedSourceBitmap     [ /O] The pointer to pointer to IWICFormatConverter interface.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetNextFrame(OUT IWICFormatConverter **ppConvertedSourceBitmap);

    /*!
    * @brief Get frame format convert, presents a frame bitmap source at specified frame index.
    *
    * @param uFrameIndex                 [I/ ] The frame index.
    * @param ppConvertedSourceBitmap     [ /O] The pointer to pointer to IWICFormatConverter interface.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetFrameAt(UINT uFrameIndex, OUT IWICFormatConverter **ppConvertedSourceBitmap);

    /*!
    * @brief Get current frame bitmap bounds.
    *
    * @param rc     [ /O] The reference to RECT structure.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetFrameRect(OUT RECT& rc);

    /*!
    * @brief Get the delay time of the GIF object.
    *
    * @return The delay time to play the animation.
    */
    UINT GetFrameDelay();

    /*!
    * @brief Get the frames count of GIF object.
    *
    * @return The frame count.
    */
    UINT GetFrameCount();

    /*!
    * @brief Get the background of the GIF.
    *
    * @param rgb     [ /O] The RGB color of background color.
    * @param alp     [ /O] The alpha of background color.
    */
    void GetBackgroundColor(OUT UINT &rgb, OUT FLOAT &alpha);

    /*!
    * @brief Get the size of the GIF object.
    *
    * @param uWidth  [ /O] The width of GIF.
    * @param uHeight [ /O] The height of GIF.
    */
    void GetSize(OUT UINT &uWidth, OUT UINT &uHeight);

    /*!
    * @brief Get the current frame index of GIF.
    *
    * @return The frame index.
    */
    UINT GetFrameIndex();

    /*!
    * @brief Set the next frame index.
    *
    * @param uNextFrameIndex    [I/ ] The next frame index.
    */
    void SetFrameIndex(UINT uNextFrameIndex);

    /*!
    * @brief Get the frame disposal.
    *
    * @return The frame disposal.
    */
    UINT GetFrameDisposal();

    /*!
    * @brief Create scaled bitmap from resource bitmap.
    *
    * @param uWidth     [I/ ] Source bitmap.
    * @param uHeight    [I/ ] Destination bitmap.
    *
    * @return S_OK if succeeds.
    */
    HRESULT CreateScalerWICAnimatedGifSource(UINT32 uWidth, UINT32 uHeight);

    /*!
    * @brief Clear the bitmap data.
    */
    void ClearData();

    /*!
    * @brief Indicates whether end animation or not.
    *
    * @return TRUE if should end animation, otherwise return FALSE.
    */
    BOOL IsEndAnimation();

protected:

    /*!
    * @brief Get the raw data at specified frame index.
    *
    * @param uFrameIndex  [I/ ] The frame index.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetRawFrame(UINT uFrameIndex);

    /*!
    * @brief Get global meta data.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetGlobalMetaData();

    /*!
    * @brief Get background color.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetGifBackgroundColor();

    /*!
    * @brief Get the looping information.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetGifLoopingInfo();

    /*!
    * @brief Get background color.
    *
    * @param pFrameMetadataQueryReader  [I/ ] The meta data query reader instance.
    * @param type                       [I/ ] The rect value type.
    * @param value                      [ /O] the output value.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetGifRectValue(IWICMetadataQueryReader *pFrameMetadataQueryReader, GIF_RECT_VALUETYPE type, OUT UINT& value);

    /*!
    * @brief Get size information.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetGifSizeInfo();

    /*!
    * @brief Indicates whether is last frame.
    *
    * @return TRUE if last frame, otherwise return FALSE.
    */
    BOOL IsLastFrame();

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

protected:

    UINT                 m_bkColorRGB;                  // The RGB of background color.
    FLOAT                m_bkColorAlpha;                // The alpha of background color.
    RECT                 m_frameRect;                   // The current frame drawing rectangle.
    UINT                 m_uFrameCount;                 // The frame count;
    UINT                 m_uNextFrameIndex;             // The next frame index.
    UINT                 m_uTotalLoopCount;             // The number of loops for which the animation will be played.
    UINT                 m_uLoopNumber;                 // The current animation loop number (e.g. 1 when the animation is first played).
    UINT                 m_uFrameDisposal;              // The current disposal for frame.
    BOOL                 m_fHasLoop;                    // Whether the GIF has a loop.
    UINT                 m_uFrameDelay;                 // The delay to play next frame of GIF.
    UINT                 m_cxGifImagePixel;             // Width of the displayed image in pixel calculated using pixel aspect ratio.
    UINT                 m_cyGifImagePixel;             // Height of the displayed image in pixel calculated using pixel aspect ratio.
    IWICFormatConverter *m_pFormatConverter;            // The format converter.
    IWICBitmapDecoder   *m_pWicBitmapDecoder;           // The bitmap decoder.
    WIC_GIF_TYPE         m_curGifType;                  // Current gif type.

    static IWICImagingFactory   *s_pImagingFactory;    // The WIC image factory.
};

END_NAMESPACE_UTILITIES

#endif // _SDKWICANIMATEDGIF_H_
#endif // __cplusplus
