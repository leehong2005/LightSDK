/*!
* @file SnrKrDragSourceHelper.h
* 
* @brief SnrKrDragSourceHelper
*
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#ifdef __cplusplus
#ifndef _SDKDRAGSOURCEHELPER_H_
#define _SDKDRAGSOURCEHELPER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief CSnrKrDragSourceHelper
*/
class CLASS_DECLSPEC SdkDragSourceHelper
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkDragSourceHelper();

    /*!
    * @brief The destructor function.
    */
    ~SdkDragSourceHelper();

    /*!
    * @brief Set flag to indicate drop tips enables.
    *
    * @return Returns S_OK if successful, or a COM error value otherwise.
    */
    HRESULT SetDropDescEnable();

    /*!
    * @brief Initializes the drag-image manager for a windowless control.
    *
    * @param hWnd           [I/ ] The handle to the window.
    * @param hBitmap        [I/ ] The handle to HBITMAP.
    * @param pDataObject    [I/ ] A pointer to the data object's IDataObject interface.
    * @param crColorKey     [I/ ] The key color.
    *
    * @return Returns S_OK if successful, or a COM error value otherwise.
    */
    HRESULT InitializeFromBitmap(HWND hWnd, HBITMAP hBitmap, IDataObject *pDataObject);

    /*!
    * @brief Initializes the drag-image manager for a control with a window.
    *
    * @param hWnd           [I/ ] A handle to the window that will receive the DI_GETDRAGIMAGE message.
    * @param pt             [I/ ] A pointer to a POINT structure that specifies the location of the cursor within the drag image.
    * @param pDataObject    [I/ ] A pointer to the data object's IDataObject interface.
    *
    * @return Returns S_OK if successful, or a COM error value otherwise.
    */
    HRESULT InitializeFromWindow(HWND hWnd, POINT& pt, IDataObject *pDataObject);

private:

    IDragSourceHelper2 *m_pDragSourceHelper2;       // The IDragSourceHelper2 object.
};

END_NAMESPACE_COMMON

#endif // _SDKDRAGSOURCEHELPER_H_
#endif // __cplusplus
