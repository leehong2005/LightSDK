/*!
* @file SdkUserInfoUtil.h
* 
* @brief This file defines class SdkUserInfoUtil to manager system user data.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/24
*/

#ifdef __cplusplus
#ifndef _SDKUSERINFOMANAGER_H_
#define _SDKUSERINFOMANAGER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief The structure for user information.
*/
typedef struct _USERINFODATA
{
    LPWSTR  lpUserName;
    LPWSTR  lpLogonDomain;
    LPWSTR  lpOtherDomain;
    LPWSTR  lpLogonServer;

} USERINFODATA, *LPUSERINFODATA;


/*!
* @brief The SdkUserInfoUtil to manager system user data and account.
*/
class CLASS_DECLSPEC SdkUserInfoUtil
{
public:

    /*!
    * @brief Get the current login user data.
    *
    * @param ppUserInfo     [ /O] The output USERINFODATA data.
    *
    * @return TRUE if succeeds, otherwise FALSE.
    *
    * @remark If you no longer use ppUserInfo, you must call DeleteUserInfoData to 
    *         delete the memory to avoid memory leaking.
    */
    static BOOL GetCurLogonUserInfo(OUT USERINFODATA **ppUserInfo);

    /*!
    * @brief Delete the memory allocated by GetCurLogonUserInfo function.
    *
    * @param ppUserInfo     [I/ ] The USERINFODATA to be deleted.
    */
    static void DeleteUserInfoData(IN USERINFODATA *pUserInfo);
};

END_NAMESPACE_UTILITIES

#endif // _SDKUSERINFOMANAGER_H_
#endif // __cplusplus
