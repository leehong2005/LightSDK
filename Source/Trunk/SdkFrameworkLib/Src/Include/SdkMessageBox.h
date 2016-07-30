/*!
* @file SdkMessageBox.h
* 
* @brief This file defines class to show message dialog box.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/28
*/

#ifdef __cplusplus
#ifndef _MESSAGEDIALOGBOX_H_
#define _MESSAGEDIALOGBOX_H_

#include "SdkUICommon.h"
#include "SdkWindowDialog.h"
#include "IViewOnClickHandler.h"

BEGIN_NAMESPACE_WINDOW

/*!
* @brief The message dialog box.
*/
class CLASS_DECLSPEC SdkMessageBox : public SdkWindowDialog, public IViewOnClickHandler
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkMessageBox();

    /*
    * @brief The destructor function.
    */
    virtual ~SdkMessageBox();

    /*!
    * @brief Call this function to creates, displays and operates a message modal dialog box.
    *        This message box contains an application-defined message and title, along with
    *        any combination of predefined icons and push buttons.
    *
    * @param hWnd       [I/ ] Handle to the owner window of the message box.
    * @param lpText     [I/ ] Pointer to a null-terminated string that contains the message to be displayed.
    * @param lpCaption  [I/ ] Pointer to a null-terminated string that contains the title to be displayed. .
    * @param uType      [I/ ] Specifies the contents and behavior of the dialog box. The value can be one or
    *                         combination of flags from following groups of flags.
    *                         MB_OK, MB_OKCANCEL, MB_YESNO, MB_YESNOCANCEL, MB_ICONERROR,
    *                         MB_ICONWARNING, MB_ICONINFORMATION, MB_ICONQUESTION, MB_ICONHAND, etc.
    */
    virtual INT_PTR Show(HWND hWnd, IN LPCTSTR lpText, LPCTSTR lpCaption, UINT uType = MB_OK);

protected:

    /*!
    * @brief Initialize the type text map.
    */
    static void InitMessageTypeTextMap();

    /*!
    * @brief Call this member function to invoke the modal dialog box and return the dialog box result when done.
    *
    * @param hWndParent     [I/ ] The handle to parent window.
    *
    * @return The result value, one of IDOK, IDCANCEL, IDABORT, IDRETRY, IDIGNORE, IDYES, IDNO, IDCLOSE.
    */
    virtual INT_PTR DoModal(HWND hWndParent);

    /*!
    * @brief Call this member function to terminate a modal dialog box.
    *
    * @param nResult        [I/ ] Contains the value to be returned from the dialog box to the caller of DoModal.
    */
    virtual void EndDialog(INT32 nResult);

    /*!
    * @brief Call this method to create layouts.
    */
    virtual void CreateLayouts();

    /*!
    * @brief Parse message type.
    */
    virtual void ParseMessageType(UINT uType);

    /*!
    * @brief Called when the window is created. In this method, derived class
    *        can initialize device, add child view, etc.
    */
    virtual void OnCreated();

    /*!
    * @brief The framework calls this function when an application requests that the
    *        the Windows window be created by calling the CreateWindowForm() member function.
    *
    * @remark If derived class wants to create custom window, should override this function
    *         to specified custom parameters.
    */
    virtual void OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams);

    /*!
    * @brief The destructor function.
    *
    * @param view       [I/ ] The pointer SdkViewElement, you should NOT delete the pointer.
    */
    virtual void OnClick(SdkViewElement *pView);

    /*!
    * @brief Called when a view has been clicked and held.
    *
    * @param pView      [I/ ] The event source.
    *
    * @return TRUE if consumed the long click, FALSE otherwise.
    */
    virtual BOOL OnLongClick(SdkViewElement *pView);

    /*!
    * @brief The message procedure.
    *
    * @param hWnd       [I/ ] The handle to the window which occurs the message.
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return If you process the message, return 0, otherwise call DefWindowProc.
    */
    virtual BOOL OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    UINT            m_uBtnType;         // The type of message box.
    UINT            m_uIconType;        // The icon type.
    wstring         m_strText;          // The pointer to null-terminated string for message.
    wstring         m_strCaption;       // The pointer to null-terminated string for caption.
    SdkViewElement *m_pCloseView;       // The close box view.

    static map<UINT, UINT> s_mapBtnTypeToResId;      // The type to text map.
};

END_NAMESPACE_WINDOW

#endif // _MESSAGEDIALOGBOX_H_
#endif // __cplusplus
