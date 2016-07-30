/*!
* @file WindowDialog.h
* 
* @brief This file defines class to show modal dialog.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/28
*/

#ifdef __cplusplus
#ifndef _WINDOWDIALOG_H_
#define _WINDOWDIALOG_H_

#include "CommonInclude.h"
#include "WindowForm.h"

/*!
* @brief The base class used for displaying dialog boxes on the screen.
*/
class CLASS_DECLSPEC WindowDialog : public WindowForm
{
public:

    /*!
    * @brief The constructor function.
    */
    WindowDialog();

    /*
    * @brief The destructor function.
    */
    virtual ~WindowDialog();

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

protected:

    /*!
    * @brief Call this function to show main window.
    *
    * @param hInstance      [I/ ] The instance of current process to run main window.
    * @param hWndParent     [I/ ] The parent of the window, usually set NULL.
    * @param nCmdShow       [I/ ] Specifies the window show state.
    *
    * @return TRUE if success to initialize and show window, otherwise return FALSE.
    */
    virtual BOOL CreateWindowForm(HINSTANCE hInstance, HWND hWndParent, INT32 nCmdShow = SW_SHOW);

    /*!
    * @brief The framework calls this function when an application requests that the
    *        the Windows window be created by calling the CreateWindowForm() member function.
    *
    * @remark If derived class wants to create custom window, should override this function
    *         to specified custom parameters.
    */
    virtual void OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams);

    /*!
    * @brief Save or restore parent window's state.
    *
    * @param hWndParent     [I/ ] Parent window's handle.
    * @param isSaved        [I/ ] Save or restore parent's state.
    */
    virtual void SaveRestoreParentState(HWND hWndParent, BOOL isSaved = TRUE);

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

protected:

    BOOL        m_isEnableParent;           // Indicates enable parent window.
};

#endif // _WINDOWDIALOG_H_
#endif // __cplusplus
