/*!
* @file SdkDropTarget.h
* 
* @brief The file define the class.
*
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#ifdef __cplusplus
#ifndef _SDKDROPTARGET_H_
#define _SDKDROPTARGET_H_

#include "SdkCommon.h"
#include "SdkCommonDef.h"
#include "SdkCommonMacro.h"
#include "SdkDataObject.h"
#include "IDropTargetNotify.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief CSnrKrDropTarget
*/
class CLASS_DECLSPEC SdkDropTarget : public IDropTarget
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkDropTarget();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkDropTarget();

    /*!
    * @brief Registers the specified window as one that can be the target of an OLE
    *        drag-and-drop operation and
    *        specifies the IDropTarget instance to use for drop operations.
    *        You should call OleInitialize function
    *        before call this function, otherwise may be return a OUTOFMEMORY error code.
    *
    * @param hTargetWnd         [I/ ] Handle to a window that can accept drops.
    * @param pDropTargetNotify  [I/ ] The pointer to IDropTargetNotify.
    *
    * @return TRUE if succeed, otherwise return FALSE.
    */
    virtual BOOL RegisterDropTarget(HWND hTargetWnd, IDropTargetNotify *pDropTargetNotify = NULL);

    /*!
    * @brief Revokes the registration of the specified application window as a potential
    *        target for OLE drag-and-drop operations.
    *
    * @return TRUE if succeed, otherwise return FALSE.
    */
    virtual BOOL RevokeDropTarget();

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
    virtual DROPEFFECT OnDragEnter(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt);

    /*!
    * @brief Called by the framework when the cursor is dragged over the window.
    *
    * @param hWnd           [I/ ] The handle to the target window.
    * @param grfKeyState    [I/ ] Current state of keyboard modifier keys.
    * @param pt             [I/ ] Current cursor coordinates.
    *
    * @return The allowed drop effect.
    */
    virtual DROPEFFECT OnDragOver(HWND hWnd, DWORD dwKeyState, POINT pt);

    /*!
    * @brief Called by the framework when the cursor leaves the window while a dragging
    *        operation is in effect.
    *
    * @param hWnd           [I/ ] The handle to the target window.
    */
    virtual void OnDragLeave(HWND hWnd);

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
    virtual BOOL OnDrop(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt);

    /*!
    * @brief Returns a pointer to a specified interface on an object to which a client
    *        currently holds an interface pointer.
    *
    * @param riid           [I/ ] Identifier of the interface being requested.
    * @param ppv            [ /O] Address of pointer variable that receives the interface
    *                             pointer requested in riid.
    *
    * @return S_OK if the interface is supported, E_NOINTERFACE if not.
    */
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);

    /*!
    * @brief The IUnknown::AddRef method increments the reference count for an interface on an object.
    *
    * @return Returns an integer from 1 to n, the value of the new reference count.
    */
    IFACEMETHODIMP_(ULONG) AddRef(void);

    /*!
    * @brief Decrements the reference count for the calling interface on a object.
    *
    * @return Returns the resulting value of the reference count,
    *         which is used for diagnostic/testing purposes only.
    */
    IFACEMETHODIMP_(ULONG) Release(void);

    /*!
    * @brief Indicates whether a drop can be accepted, and, if so, the effect of the drop.
    *
    * @param pDataObj       [I/ ] The pointer to IDataObject interface.
    * @param grfKeyState    [I/ ] Current state of keyboard modifier keys.
    * @param pt             [I/ ] Current cursor coordinates.
    * @param pdwEffect      [I/O] Pointer to the effect of the drag-and-drop operation.
    *
    * @return Returns S_OK if successful, otherwise other COM errors.
    */
    IFACEMETHODIMP DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

    /*!
    * @brief Provides target feedback to the user through the DoDragDrop function.
    *
    * @param grfKeyState    [I/ ] Current state of keyboard modifier keys.
    * @param pt             [I/ ] Current cursor coordinates.
    * @param pdwEffect      [I/O] Pointer to the effect of the drag-and-drop operation.
    *
    * @return Returns S_OK if successful, otherwise other COM errors.
    */
    IFACEMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

    /*!
    * @brief Removes target feedback and releases the data object.
    *
    * @return Returns S_OK if successful, otherwise other COM errors.
    */
    IFACEMETHODIMP DragLeave(void);

    /*!
    * @brief Drops the data into the target window.
    *
    * @param pDataObj       [I/ ] The pointer to the IDataObject interface.
    * @param grfKeyState    [I/ ] Current state of keyboard modifier keys.
    * @param pt             [I/ ] Current cursor coordinates.
    * @param pdwEffect      [I/O] Pointer to the effect of the drag-and-drop operation.
    *
    * @return Returns S_OK if successful, otherwise other COM errors.
    */
    IFACEMETHODIMP Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

protected:

    /*!
    * @brief Filter the drop effect by key state.
    *
    * @param grfKeyState    [I/ ] The key state.
    * @param dwEffectSrc    [I/ ] The source drop effect value.
    *
    * @return The filtered drop effect.
    */
    virtual DROPEFFECT FilterDropEffect(DWORD grfKeyState, DROPEFFECT dwEffectSrc);

protected:

    BOOL                m_isDataAvailable;      // The flag indicates the data available or not.
    HWND                m_hTargetWnd;           // The target window handle.
    volatile LONG       m_lRefCount;            // The reference count.
    IDropTargetNotify  *m_pDropTargetNotify;    // Pointer to IDropTargetNotify interface.
    IDropTargetHelper  *m_pDropTargetHelper;    // The pointer of IDropTargetHelper interface.
};

END_NAMESPACE_COMMON

#endif // _SDKDROPTARGET_H_
#endif // __cplusplus
