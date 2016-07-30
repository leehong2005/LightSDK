/*!
* @file SdkProgressDialog.h 
* 
* @brief The class defines the progress dialog. it supplies the function to display the percent.
* 
* Copyright (C) 2008, LZT Corporation.
* 
* @author Li Hong
* @date 2009/01/18
*/

#ifdef __cplusplus
#ifndef _SDKPROGRESSDIALOG_H_
#define _SDKPROGRESSDIALOG_H_

#include "SdkCommon.h"
#include "SdkCommonDef.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief The class SdkProgressDialog.
*/
class CLASS_DECLSPEC SdkProgressDialog
{
public:

    /*!
    * @brief The default constructor function
    */
    SdkProgressDialog();

    /*!
    * @brief The default destructor.
    */
    virtual ~SdkProgressDialog();

    /*!
    * @brief Start to display the progress dialog.
    *
    * @param isShowTopMost  [I/ ] Indicate the dialog is showed on top most
    *
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL Start(BOOL isShowTopMost = FALSE);

    /*!
    * @brief Stop to display the progress dialog.
    *
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL Stop();

    /*!
    * @brief Set the percent of the progress dialog.
    *
    * @param dwCompleted    [I/ ] The completed percent.
    * @param dwTotal        [I/ ] The value of the total percent.
    *
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL SetProgress(IN DWORD dwCompleted, IN DWORD dwTotal);

    /*!
    * @brief User cancels the progress dialog or not.
    * c
    * @return TRUE is canceled, FALSE is not canceled.
    */
    virtual BOOL HasCancelled();

    /*!
    * @brief Set the string on the progress dialog.
    *
    * @param dwLineNum  [I/ ] The number of the line, which is 1 to 3.
    * @param szString   [I/ ] The string displayed on the progress dialog.
    *
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL SetLines(IN DWORD dwLineNum, IN const CryptString& szString);

    /*!
    * @brief Set the title of the progress dialog.
    *
    * @param szTitle    [I/ ] The string for the title of progress dialog.
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL SetTitle(IN const CryptString& szTitle);

    /*!
    * @brief Set the message when user canceled the progress dialog.
    *
    * @param szCancelMsg [I/ ] The message to display when user canceled the progress dialog.
    *
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL SetCancelMessage(IN const CryptString& szCancelMsg);

    /*!
    * @brief Get the handle of progress dialog.
    *
    * @return The handle of progress dialog.
    */
    virtual HWND GetProgressDialogHwnd() const;

private:

    HWND             m_hProgDlgWnd;         // The handle of the progress dialog.
    BOOL             m_isShowTopMost;       // Is show on top most or not.
    CryptString      m_szTitle;             // The string of title.
    IProgressDialog *m_pProgressDlg;        // The pointer to the IProgressDialog
};

END_NAMESPACE_UTILITIES

#endif // _SDKPROGRESSDIALOG_H_
#endif // __cplusplus
