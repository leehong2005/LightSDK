/*!
* @file IDropTargetNotify.h
* 
* @brief This file defines the interface for drag target to notify client application.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/12
*/

#ifdef __cplusplus
#ifndef _IDROPTARGETNOTIFY_H_
#define _IDROPTARGETNOTIFY_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief IDropTargetNotify interface.
*/
class IDropTargetNotify
{
public:

    /*!
    * @brief The default destructor function.
    */
    virtual ~IDropTargetNotify() {}

        /*!
    * @brief Called by the framework when the cursor is first dragged into the window.
    *
    * @param hWnd           [I/ ] The handle to the target window.
    * @param pDataObj       [I/ ] The pointer to IDataObject object.
    * @param dwKeyState     [I/ ] Current state of keyboard modifier keys.
    * @param pt             [I/ ] Current cursor coordinates.
    *
    * @return The allowed drop effect.
    */
    virtual DROPEFFECT OnDragEnter(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt) = 0;

    /*!
    * @brief Called by the framework when the cursor is dragged over the window.
    *
    * @param hWnd           [I/ ] The handle to the target window.
    * @param grfKeyState    [I/ ] Current state of keyboard modifier keys.
    * @param pt             [I/ ] Current cursor coordinates.
    *
    * @return The allowed drop effect.
    */
    virtual DROPEFFECT OnDragOver(HWND hWnd, DWORD dwKeyState, POINT pt) = 0;

    /*!
    * @brief Called by the framework when the cursor leaves the window while a dragging
    *        operation is in effect.
    *
    * @param hWnd           [I/ ] The handle to the target window.
    */
    virtual void OnDragLeave(HWND hWnd) = 0;

    /*!
    * @brief Called by the framework when a drop operation is to occur..
    *
    * @param hWnd           [I/ ] The handle to the target window.
    * @param pDataObj       [I/ ] The pointer to IDataObject object.
    * @param grfKeyState    [I/ ] Current state of keyboard modifier keys.
    * @param pt             [I/ ] Current cursor coordinates.
    *
    * @return The allowed drop effect.
    */
    virtual BOOL OnDrop(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt) = 0;
};

END_NAMESPACE_COMMON

#endif // _IDROPTARGETNOTIFY_H_
#endif // __cplusplus
