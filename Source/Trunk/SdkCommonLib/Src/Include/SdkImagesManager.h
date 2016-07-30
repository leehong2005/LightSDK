/*!
* @file SdkImageManager.h
* 
* @brief This file defines the Image helper
* 
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2009/06/16 12:00
*/

#ifdef __cplusplus
#ifndef _SDKIMAGEMANAGER_H_
#define _SDKIMAGEMANAGER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "SdkCommonHelper.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief Save types for images
*/
typedef enum _IMAGE_SAVETYPE
{
    SAVETYPE_BMP        = 0,        // image/bmp
    SAVETYPE_JPEG       = 1,        // image/jpeg
    SAVETYPE_GIF        = 2,        // image/gif
    SAVETYPE_EMF        = 3,        // image/x-emf
    SAVETYPE_WMF        = 4,        // image/x-wmf
    SAVETYPE_TIFF       = 5,        // image/tiff
    SAVETYPE_PNG        = 6,        // image/png
    SAVETYPE_ICON       = 7,        // image/x-icon

} IMAGE_SAVETYPE;

/*!
* @brief Wrap modes for brushes
*/
typedef enum _IMAGE_WRAPMODE
{
    MODE_TILE           = 0,
    MODE_CENTER         = 1,
    MODE_STRETCH        = 2,
    MODE_CLAMP          = 3,
    MODE_TILEFLIPX      = 4,
    MODE_TILEFLIPY      = 5,
    MODE_TILEFLIPXY     = 6,

} IMAGE_WRAPMODE;

/*!
* @brief The SdkImagesManager class is implemented with Gdiplus.
*/
class CLASS_DECLSPEC SdkImagesManager
{
public:

    /*!
    * @brief initializes Microsoft Windows GDI+, please call this static function before making any other
    *        GDI+ calls. This function is a wrapper of GdiplusStartup function.
    */
    static void GdiplusInitialize();

    /*!
    * @brief The UnGdiplusInitialize function cleans up resources used by Microsoft Windows GDI+.
    *        Please make sure 
    *        calling this static function after you finished using GDI+ calls.
    *        This function is a wrapper of GdiplusShutdown function.
    */
    static void UnGdiplusInitialize();

    /*!
    * @brief Create a mask bitmap associated with a color image specified by hbmColour.
    *
    * @param hbmColour      [I/ ] The handle to a color image to be as source of mask bitmap.
    * @param crTransparent  [I/ ] The background color. The default value is RGB(0, 0, 0)(black).
    *
    * @return The handle of mask bitmap.
    */
    static HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent = RGB(0, 0, 0));

    /*!
    * @brief Constructor function
    * 
    * @param hInst          [I/ ] The handler of current instance
    */
    SdkImagesManager(IN HINSTANCE hInst = NULL);

    /*!
    * @brief The copy constructor function
    * 
    * @param imagesManagerSrc [I/ ] The reference to object of CImagesManager class
    *                               which is used to initialize a new object of CImagesManager.
    */
    SdkImagesManager(IN const SdkImagesManager& imagesManagerSrc);

    /*!
    * @brief Default destructor function
    */
    ~SdkImagesManager();

    /*!
    * @brief Load image from resource id.
    * 
    * @param resId      [I/ ] The resource id of image which format is "png"
    * 
    * @return TRUE if success, otherwise is FALSE.
    */
    BOOL LoadImageFromResId(UINT resId);

    /*!
    * @brief Load image from specified file.
    * 
    * @param pszfilePath [I/ ] The path of file which extract icon from
    * 
    * @return TRUE if success, otherwise is FALSE.
    */
    BOOL LoadImageFromFile(IN LPCTSTR pszfilePath);

    /*!
    * @brief Create an object of Gdiplus::Bitmap class from a handle to HBITMAP.
    *
    * @param hbitmap    [I/ ] The handle to HBITMAP to be as source of Gdiplus::Bitmap object.
    *
    * @return RUE is successfully to load image from HBITMAP, otherwise is FALSE.
    *
    * @remark
    * The function is different Gdiplus::Bitmap::LoadFromHBITMA, this function
    * will reserve alpha
    * information of HBITMAP, if you draw Bitmap* on a DC using Graphics,
    * you will see the transparency effect
    * if the HBITMA has. but, the object of Bitmap* generated from 
    * Gdiplus::Bitmap::LoadFromHBITMA will discard
    * the alpha information of HBITMAP. If you want draw a transparent image, 
    * you can use this function.
    * The Bitmap* returned by this function is a copy of HBITMAP, the object
    * is not associated with HBITMAP once it is created.
    */
    BOOL LoadImageFromHBITMAP(IN HBITMAP hbitmap);

    /*!
    * @brief Load image from HICON handler.
    * 
    * @param hIcon  [I/ ] The handler of HICON
    * 
    * @return TRUE if success, otherwise is FALSE.
    */
    BOOL LoadImageFromHICON(IN HICON hIcon);

    /*!
    * @brief Save the image to a local file specified by lpfileName argument.
    * 
    * @param lpfileName [I/ ] The path of the image file.
    * 
    * @return TRUE is success, otherwise is FALSE.
    */
    BOOL SaveImageToFile(IN LPCWSTR lpfileName);

    /*!
    * @brief Save the image to a local file specified by lpfileName argument.
    * 
    * @param lpfileName     [I/ ] The path of the image file.
    * 
    * @param imageSaveType  [I/ ] The type of save image.
    * 
    * @return TRUE is success, otherwise is FALSE.
    */
    BOOL SaveImageToFile(IN LPCWSTR lpfileName, IMAGE_SAVETYPE imageSaveType);

    /*!
    * @brief Change the size of current image according to the specified width and height.
    * 
    * @param fWidth         [I/ ] The width of image
    * @param fHeight        [I/ ] The height of image
    * 
    * @return TRUE is success, otherwise is FALSE.
    */
    BOOL ResizeTo(FLOAT fWidth, FLOAT fHeight);

    /*!
    * @brief Change the size of current image according to the specified ratio.
    * 
    * @param fRatio         [I/ ] The ratio of image
    * 
    * @return TRUE is success, otherwise is FALSE.
    */
    BOOL ResizeTo(FLOAT fRatio);

    /*!
    * @brief Draw the image on the Device Context specified by hDC.
    * 
    * @param hDC            [I/ ] The Device Context to be drawn.
    * @param imageMode      [I/ ] Element of the IMAGE_WRAPMODE enumeration that
    *                             specifies how repeated copies of an image
    *                             are used to tile an area when it is 
    *                             painted with this texture brush.
    */
    void DrawImage(IN HDC hDC, IMAGE_WRAPMODE imageMode = MODE_TILE);

    /*!
    * @brief Draw the image on the window specified by hWnd.
    * 
    * @param hDC            [I/ ] The Device Context to be drawn.
    * @param hWnd           [I/ ] The handle to the window.
    * @param imageMode      [I/ ] Element of the IMAGE_WRAPMODE enumeration 
    *                             that specifies how repeated copies of an image
    *                             are used to tile an area when it is painted with this texture brush.
    */
    void DrawImage(HDC hDC, IN HWND hWnd, IMAGE_WRAPMODE imageMode = MODE_TILE);

    /*!
    * @brief Delete the image object.
    */
    void DeleteImage();

    /*!
    * @brief Set the save type of current image.
    * 
    * @param imageSaveType  [I/ ] The type of save image, the default value is SAVETYPE_PNG(PNG).
    */
    void SetImageSaveType(IMAGE_SAVETYPE imageSaveType = SAVETYPE_PNG);

    /*!
    * @brief Set the save type of current image.
    * 
    * @return imageSaveType [I/ ] The type of save image.
    */
    IMAGE_SAVETYPE GetImageSaveType();

    /*!
    * @brief Get the flag that indicates current object has image object or not.
    *
    * @return BOOL
    * return value is the m_bLoadImage.
    */
    BOOL HaveLoadedImage();

    /*!
    * @brief Get the width of the image.
    *
    * @return The width of image, if there is no image, return value is 0, otherwise is the width of image.
    */
    DWORD GetWidth();

    /*!
    * @brief Get the height of the image.
    *
    * @return The height of image, if there is no image, return value is 0, otherwise is the height of image.
    */
    DWORD GetHeight();

    /*!
    * @brief Get the image object of current instance.
    *
    * @return return value is the m_pGdiImage
    */
    Bitmap* GetImage() const;

    /*!
    * @brief Overload the operator =.
    *
    * @param IN const CImagesManager& pImageManagerSrc.
    * The source of right value.
    *
    * @return CImagesManager&
    * The reference to the new object of CImagesManager.
    */
    SdkImagesManager& operator =(IN const SdkImagesManager& pImageManagerSrc);

    /*!
    * @brief Overload the operator * to get the pointer to Bitmap.
    *
    * @remark Bitmap* p = *pImagesManager; pImagesManager is the pointer to CImagesManager class
    */
    operator Bitmap*() const;

    /*!
    * @brief Get the the handle of the image.
    *
    * @return return value is the handle of image instance
    */
    HBITMAP GetHBITMAP();

protected:

    /*!
    * @brief Check the lat status of graphics plus operation.
    *        If the status is Gdiplus::Ok, m_bLoadImage will be
    *        set to TRUE, otherwise, m_bLoadImage is set to FALSE and
    *        DeleteImage will be called.
    *
    * @return Return TRUE if current object has image, otherwise return FALSE.
    */
    BOOL CheckLastStatus();

    /*!
    * @brief Get the bits data storing in a handle to HBITMAP. The GetBitmapBitsData
    *        will allocate memory for the data buffer,
    *        You must delete the memory pointed by the data buffer by using delete[]
    *        to avoid memory leak.
    *
    * @param hBitmap    [I/ ] The handle to HBITMAP to be get bits data.
    * @param lpbitmap   [ /O] The pointer to BITMAP structure.
    * @param pbitbuf    [ /O] The address of a buffer to receive the bits data, 
    *                         this function will allocate memory for data buffer.
    *
    * @return Return TRUE if current object has image, otherwise return FALSE.
    *
    * @remark
    * If hBitmap is NULL, the function returns FALSE.
    * If lpbitmap is NULL, the function returns FALSE.
    * If lpbitmap and pbitbuf are NULL, the function fails and returns FALSE.
    * If hBitmap and lpbitmap are NOT NULL, and the function succeeds, the returned value is TRUE.
    */
    BOOL GetBitmapBitsData(IN HBITMAP hBitmap, OUT LPBITMAP lpbitmap, OUT LPVOID* pbitbuf);

    /*!
    * @brief Load resource to memory by identifier, type, from specified handle to HMODULE.
    *
    * @param nResID     [I/ ] The identifier of the resource.
    * @param lpcsType   [I/ ] The type of the resource, such as _T("PNG").
    * @param hModule    [I/ ] The handle to the module whose executable file contains
    *                         the resource. If hModule is NULL, the system
    *                         will load the resource from the handle that is used
    *                         to create the current process.
    *
    * @param pData      [ /O] The pointer to pointer to VOID, which stores the data of 
    *                         the resource. You should call delete[] to free
    *                         the memory in no longer needs to prevent the memory leaks.
    *
    * @return The size of the resource. If the function fails, the value is 0,
    *         otherwise is the size of resource.
    */
    DWORD LoadResourceData(UINT nResID, IN LPCWSTR lpcsType, IN HMODULE hModule, OUT LPVOID* ppData);

    /*!
    * @brief Get the clsid of image.
    *
    * @param pszformat              [I/ ] The format of image, which must be one of follows values:
    * image/bmp
    * image/jpeg
    * image/gif
    * image/x-emf
    * image/x-wmf
    * image/tiff
    * image/png
    * image/x-icon
    *
    * @param pclsid                 [ /O] The result of clsid
    * @param szFileNameExtension    [ /O] The file extension, such as *.png.
    * @param dwSize                 [I/ ] The size of the buffer szFileNameExtension
    *
    * @return TRUE is success, FALSE is failure.
    */
    BOOL GetEncoderClsid(
        LPCTSTR pszformat,
        OUT CLSID* pclsid,
        OUT LPWSTR szFileNameExtension,
        IN DWORD dwSize);

private:
    
    BOOL             m_bLoadImage;           // Judge the image is or nor be load.
    Bitmap          *m_pGdiImage;            // Define a bitmap pointer.
    HINSTANCE        m_hInst;                // The instance of current process
    IMAGE_SAVETYPE   m_imageSaveType;        // The type for saving image.
    static ULONG_PTR m_gdiplusToken;         // Used to initialize GDI+.
};

END_NAMESPACE_UTILITIES

#endif // _SDKIMAGEMANAGER_H_
#endif // __cplusplus
