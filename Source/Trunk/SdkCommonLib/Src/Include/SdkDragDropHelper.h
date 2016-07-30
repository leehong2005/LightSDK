/*!
* @file SdkDragDropHelper.h
* 
* @brief The file define the class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#ifdef __cplusplus
#ifndef _SDKDRAGDROPHELPER_H_
#define _SDKDRAGDROPHELPER_H_

#include "SdkDataObject.h"
#include "SdkDropSource.h"
#include "SdkDropTarget.h"
#include "SdkDragSourceHelper.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief CSnrKrDragHelper
*/
class CLASS_DECLSPEC SdkDragDropHelper
{
public:

    /*!
    * @brief The RegisterClipboardFormat function registers a new clipboard format.
    *
    * @param lpszFormat     [I/ ] Pointer to a null-terminated string that names the new format.
    *
    * @return If the function succeeds, the return value identifies the registered clipboard format.
    *         If the function fails, the return value is zero.
    */
    static CLIPFORMAT RegisterClipFormat(IN LPCTSTR lpszFormat);

    /*!
    * @brief Check whether the window begins drag and drop.
    *
    * @param hWndDrag       [I/ ] The handle to the window to be checked.
    * @param x              [I/ ] The x coordinate of mouse point.
    * @param y              [I/ ] The y coordinate of mouse point.
    *
    * @return return TRUE if the window should do drag and drop operation,
    *         return FALSE if the window should NOT do drag and drop operation.
    */
    static BOOL CheckForBeginDrag(IN HWND hWndDrag, IN int x, IN int y);
};

END_NAMESPACE_COMMON

#endif // _SDKDRAGDROPHELPER_H_
#endif // __cplusplus
