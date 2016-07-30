/*!
* @file ICryptFileNotify.h
* 
* @brief This file defines the interface of crypt module to communicate with client module.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/12
*/

#ifdef __cplusplus
#ifndef _ICRYPTFILENOTIFY_H_
#define _ICRYPTFILENOTIFY_H_

#include "SdkCryptDef.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The interface between crypt module and client module.
*/
class DECLSPEC_NOVTABLE ICryptFileNotify
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ICryptFileNotify() {}

    /*!
    * @brief Called when crypt operation begins.
    */
    virtual void OnCryptBegin() = 0;

    /*!
    * @brief Called when crypt operation finishes.
    */
    virtual void OnCryptFinish() = 0;

    /*!
    * @brief Called process next file.
    *
    * @dwCurIndex       [I/ ] The current index of files.
    * @lpCurFilePath    [I/ ] The path of the file.
    */
    virtual void OnCryptNext(IN DWORD dwCurIndex, IN LPCTSTR lpCurFilePath) = 0;

    /*!
    * @brief Called when an error occurs.
    *
    * @lpErrorMsg       [I/ ] The error message of operation.
    */
    virtual void OnCryptError(IN LPCTSTR lpErrorMsg) = 0;

    /*!
    * @brief Called when should update the process of operation.
    *
    * @param dwCompleted [I/ ] The completed number.
    * @param dwTotal     [I/ ] The total number.
    */
    virtual void OnProgressChanged(IN DWORD dwCompleted, IN DWORD dwTotal) = 0;

    /*!
    * @brief Called when inquire user to replace existed files or not.
    *
    * @param pResult     [ /O] One value of CRYPT_INQUIRE_RESULT.
    * @param lpFilePath  [I/ ] Current conflicted file name.
    */
    virtual void OnReplaceFiles(OUT CRYPT_INQUIRE_RESULT *pResult, IN LPCTSTR lpFileName) = 0;
};

END_NAMESPACE_COMMON

#endif // _ICRYPTFILENOTIFY_H_
#endif // __cplusplus
