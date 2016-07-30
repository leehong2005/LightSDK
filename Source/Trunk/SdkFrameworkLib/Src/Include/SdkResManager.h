/*!
* @file SdkResManager.h
* 
* @brief This file defines SdkResManager class for managing resources.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/22
*/

#ifdef __cplusplus
#ifndef _SDKRESOURCEMANAGER_H_
#define _SDKRESOURCEMANAGER_H_

#include "SdkCommonInclude.h"

BEGIN_NAMESPACE_THEME


/*!
* @brief The image information.
*/
typedef struct _IMAGEDATA
{
    int         nWidth;             // The width of the bitmap.
    int         nHeight;            // The height of the bitmap.
    int         nResId;             // The resource id associated with the bitmap.
    HBITMAP     hBitmap;            // The bitmap handle.
    HMODULE     hModule;            // The module from which the bitmap is loaded.

} IMAGEDATA, *LPIMAGEDATA;


/*!
* @brief The SdkResManager class.
*/
class CLASS_DECLSPEC SdkResManager
{
public:

    /*!
    * @brief Load the resource library, this function should be called before any
    *        resource operation.
    *
    * @param lpLibFileName  [I/ ] The resource library name, default value is NULL, means use default resource library.
    *
    * @return TRUE if succeed, otherwise FALSE.
    */
    static BOOL LoadResLibrary(LPCTSTR lpLibFileName = NULL);

    /*!
    * @brief Free the library, typically it is called when resource is no longer used.
    */
    static void FreeResLibrary();

    /*!
    * @brief Get the resource library module handle.
    */
    static HMODULE GetResModule();

    /*!
    * @brief Get image from specified default HMODULE.
    *
    * @param nResId     [I/ ] The resource id.
    *
    * @return The handle to the HBITMAP.
    */
    static HBITMAP GetImage(int nResId);

    /*!
    * @brief Get image from specified HMODULE.
    *
    * @param nResId     [I/ ] The resource id.
    * @param hModule    [I/ ] The module handle. If you want to get resource from default, should be NULL.
    *
    * @return The handle to the HBITMAP.
    */
    static HBITMAP GetImage(int nResId, HMODULE hModule);

    /*!
    * @brief Get the image information from specified HMODULE and resource id.
    *
    * @param nResId     [I/ ] The resource id.
    * @param hModule    [I/ ] The module handle. If you want to get resource from default, should be NULL.
    *
    * @return The pointer to IMAGEINFO structure, should not delete the memory.
    */
    static LPIMAGEDATA GetImageInfo(int nResId, HMODULE hModule);

    /*!
    * @brief Get the string from specified HMODULE.
    *
    * @param nResId     [I/ ] The resource id.
    * @param hModule    [I/ ] The module handle. If you want to get resource from default, should be NULL.
    *
    * @return The pointer to TCHAR, you should copy a duplicate of the string and do not
    *         delete the memory pointed by the pointer.
    */
    static LPCTSTR GetString(int nResId, HMODULE hModule);

private:

    /*!
    * @brief Find image data from list according to the specified resource id and module handle.
    *
    * @param nResId     [I/ ] The resource id.
    * @param hModule    [I/ ] The module handle. If you want to get resource from default, should be NULL.
    *
    * @return The pointer to IMAGEDATA data, NULL if not found.
    */
    static LPIMAGEDATA FindImageData(int nResId, HMODULE hModule);

private:

    static HMODULE               s_hResModule;              // The resource library module.
    static vector<wstring>       s_vctStrings;              // The string list.
    static vector<LPIMAGEDATA>   s_vctImageInfos;           // The image informations.
    static SdkWICImageHelper    *s_pImageHelper;            // The image helper.
};

END_NAMESPACE_THEME

#endif // _SDKRESOURCEMANAGER_H_
#endif // __cplusplus
