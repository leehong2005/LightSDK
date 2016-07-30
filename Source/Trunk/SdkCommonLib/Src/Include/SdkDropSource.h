/*!
* @file SdkDropSource.h
* 
* @brief The file define the class.
*
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#ifdef __cplusplus
#ifndef _SDKDROPSOURCE_H_
#define _SDKDROPSOURCE_H_

#include <shlobj.h>
#include <shlwapi.h>
#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief CSnrKrDropSource
*/
class CLASS_DECLSPEC SdkDropSource : public IDropSource
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkDropSource();

    /*!
    * @brief The destructor function.
    */
    ~SdkDropSource();

    /*!
    * @brief Returns a pointer to a specified interface on an object to which a client 
    *        currently holds an interface pointer.
    *
    * @param riid           [I/ ] Identifier of the interface being requested.
    * @param ppv            [ /O] Address of pointer variable that receives the 
    *                             interface pointer requested in riid.
    *
    * @return S_OK if the interface is supported, E_NOINTERFACE if not.
    */
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);

    /*!
    * @brief The IUnknown::AddRef method increments the reference count for an interface on an object.
    *
    * @return Returns an integer from 1 to n, the value of the new reference count.
    */
    IFACEMETHODIMP_(ULONG) AddRef();

    /*!
    * @brief Decrements the reference count for the calling interface on a object.
    *
    * @return Returns the resulting value of the reference count, which is used 
    *         for diagnostic/testing purposes only.
    */
    IFACEMETHODIMP_(ULONG) Release();

    /*!
    * @brief Determines whether a drag-and-drop operation should continue.
    *
    * @param fEscapePressed     [I/ ] Status of escape key since previous call.
    * @param grfKeyState        [I/ ] Current state of keyboard modifier keys.
    *
    * @return Returns S_OK if successful, otherwise other COM errors.
    */
    IFACEMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);

    /*!
    * @brief Gives visual feedback to an end user during a drag-and-drop operation.
    *
    * @param dwEffect           [I/ ] Effect of a drop operation.
    *
    * @return Returns S_OK if successful, otherwise other COM errors.
    */
    IFACEMETHODIMP GiveFeedback(DWORD dwEffect);

private:

    volatile LONG   m_lRefCount;        // The reference count
};

END_NAMESPACE_COMMON

#endif // _SDKDROPSOURCE_H_
#endif // __cplusplus
