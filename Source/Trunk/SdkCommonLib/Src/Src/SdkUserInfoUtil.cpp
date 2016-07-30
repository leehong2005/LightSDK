/*!
* @file SdkUserInfoUtil.cpp
* 
* @brief This file defines class SdkUserInfoUtil to manager system user data.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/24
*/

#include "stdafx.h"
#include "SdkUserInfoUtil.h"
#include "SdkCommonMacro.h"
#include <lm.h>
#include <Lmcons.h>

USING_NAMESPACE_UTILITIES

//////////////////////////////////////////////////////////////////////////

BOOL SdkUserInfoUtil::GetCurLogonUserInfo(OUT USERINFODATA **ppUserInfo)
{
    if (NULL == ppUserInfo)
    {
        return FALSE;
    }

    (*ppUserInfo) = NULL;
    // Call the NetWkstaUserGetInfo function and specify level 1.
    DWORD dwLevel = 1;
    LPWKSTA_USER_INFO_1 pUserInfo = NULL;
    NET_API_STATUS nStatus = NERR_Success;
    nStatus = NetWkstaUserGetInfo(NULL, dwLevel, (LPBYTE*)&pUserInfo);

    // If succeed to get user information.
    if (NERR_Success == nStatus && NULL != pUserInfo)
    {
        USERINFODATA *pTempUserInfo = new USERINFODATA();

        // Get the size of the buffer.
        size_t nUserNameLen     = wcslen(pUserInfo->wkui1_username) + 1;
        size_t nLogonDomainLen  = wcslen(pUserInfo->wkui1_logon_domain) + 1;
        size_t nOtherDomainLen  = wcslen(pUserInfo->wkui1_oth_domains) + 1;
        size_t nLogonServerLen  = wcslen(pUserInfo->wkui1_logon_server) + 1;

        // Allocate the memory.
        pTempUserInfo->lpUserName    = new WCHAR[nUserNameLen];
        pTempUserInfo->lpLogonDomain = new WCHAR[nLogonDomainLen];
        pTempUserInfo->lpOtherDomain = new WCHAR[nOtherDomainLen];
        pTempUserInfo->lpLogonServer = new WCHAR[nLogonServerLen];

        // Copy the memory.
        wcscpy_s(
            pTempUserInfo->lpUserName,
            nUserNameLen,
            pUserInfo->wkui1_username);

        wcscpy_s(
            pTempUserInfo->lpLogonDomain,
            nLogonDomainLen,
            pUserInfo->wkui1_logon_domain);

        wcscpy_s(
            pTempUserInfo->lpOtherDomain,
            nOtherDomainLen,
            pUserInfo->wkui1_oth_domains);

        wcscpy_s(
            pTempUserInfo->lpLogonServer,
            nLogonServerLen,
            pUserInfo->wkui1_logon_server);

        (*ppUserInfo) = pTempUserInfo;
    }

    return (NERR_Success == nStatus && NULL != (*ppUserInfo));
}

//////////////////////////////////////////////////////////////////////////

void SdkUserInfoUtil::DeleteUserInfoData(IN USERINFODATA *pUserInfo)
{
    if (NULL != pUserInfo)
    {
        // Delete the memory pointed by member of USERINFODATA.
        SAFE_DELETE_ARRAY(pUserInfo->lpUserName);
        SAFE_DELETE_ARRAY(pUserInfo->lpLogonDomain);
        SAFE_DELETE_ARRAY(pUserInfo->lpOtherDomain);
        SAFE_DELETE_ARRAY(pUserInfo->lpLogonServer);

        // Delete the memory of USERINFODATA structure.
        SAFE_DELETE(pUserInfo);
    }
}
