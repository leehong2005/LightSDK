/*!
* @file SdkApplication.h
* 
* @brief This file defines functions to run an application, wrap message loop.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/19
*/

#ifdef __cplusplus
#ifndef _SDKAPPLICATION_H_
#define _SDKAPPLICATION_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_UILIB

/*!
* @brief The SdkApplication class.
*/
class CLASS_DECLSPEC SdkApplication
{
public:

    /*!
    * @brief The default constructor function.
    */
    SdkApplication();

    /*!
    * @brief The constructor function.
    *
    * @param hInst      [I/ ] The handle to the current instance of the application.
    * @param nCmdShow   [I/ ] Specifies how the window is to be shown.
    */
    SdkApplication(HINSTANCE hInst, int nCmdShow);

    /*!
    * @brief The destructor function.
    */
    ~SdkApplication();

    /*!
    * @brief Call this method to enter message loop.
    *
    * @return The result of the exit code.
    */
    int Run();

    /*!
    * @brief Call this method to create a window and show.
    *
    * @param pWindowForm [I/ ] The pointer to the SdkWindowForm.
    *
    * @return The result of the exit code.
    */
    int Run(SdkWindowForm *pWindowForm);

private:

    int             m_nCmdShow;         // The shown type.
    HINSTANCE       m_hInstance;        // Instance of current application.
};

END_NAMESPACE_UILIB

#endif // _SDKAPPLICATION_H_
#endif // __cplusplus
