/*!
* @file SdkCryptDef.h
* 
* @brief This file defines common definition for crypt module.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/12
*/

#ifdef __cplusplus
#ifndef _SDKCRYPTDEF_H_
#define _SDKCRYPTDEF_H_

#include "SdkCommon.h"
#include "SdkCommonDef.h"
#include "SdkCommonMacro.h"

#include <WinCrypt.h>

BEGIN_NAMESPACE_COMMON

/*!
* @brief The crypt operation type.
*/
typedef enum _CRYPT_OP_TYPE
{
    CRYPT_OP_UNKNOWN            = 0x00,             // Unknown operation.
    CRYPT_OP_ENCRYPT            = 0x01,             // Encrypt operation.
    CRYPT_OP_DECRYPT            = 0x02,             // Decrypt operation.

} CRYPT_OP_TYPE;


/*!
* @brief The crypt inquired type
*/
typedef enum _CRYPT_INQUIRE_RESULT
{
    CRYPT_INQUIRE_UNKNOWN       = 0x00,             // Unknown inquire.
    CRYPT_INQUIRE_YES           = 0x01,             // Yes.
    CRYPT_INQUIRE_NO            = 0x02,             // No.
    CRYPT_INQUIRE_ALLYES        = 0x03,             // All operation is Yes.
    CRYPT_INQUIRE_ALLNO         = 0x04,             // All operation is No.
    CRYPT_INQUIRE_CANCEL        = 0x05,             // Cancel the operation.

} CRYPT_INQUIRE_RESULT;


/*!
* @brief The crypt error codes.
*/
typedef enum _CRYPT_RESULT
{
    CRYPT_ERROR_FAIL                = 0,            // Fail.
    CRYPT_ERROR_SUCCEED             = 1,            // Succeed.
    CRYPT_ERROR_INVALID_KEY         = 7,            // Invalid key.
    CRYPT_ERROR_INVALIDHEADER       = 8,            // The header is invalid.
    CRYPT_ERROR_DECRYPT             = 9,            // Error of decryption.
    CRYPT_ERROR_READDATA            = 10,           // Error of read data.
    CRYPT_ERROR_ENCRYPT             = 11,           // Error of encryption.
    CRYPT_ERROR_WRITEDATA           = 12,           // Error of write data.
    CRYPT_ERROR_CANCEL              = 13,           // The cancel operation.
    CRYPT_ERROR_OTHER               = 14,           // Other unknown error.
    CRYPT_ERROR_OPENFILE            = 15,           // Run the file failure.
    CRYPT_ERROR_INVALIDPARAM        = 16,           // Invalid parameter.
    CRYPT_ERROR_INVALIDFILE         = 17,           // Invalid file.
    CRYPT_ERROR_INVALIDPWD          = 18,           // Invalid password.
    CRYPT_ERROR_NOFILES             = 20,           // No files

    CRYPT_ERROR_FILE_NOT_FOUND      = 2,            // File not found, error code is 2.
    CRYPT_ERROR_PATH_NOT_FOUND      = 3,            // The system can not find the path specified error code is 3.
    CRYPT_ERROR_ACCESS_DENIED       = 5,            // Access is denied, error code is 5.
    CRYPT_ERROR_INVALID_HANDLE      = 6,            // The handle is invalid, error code is 6.
    CRYPT_ERROR_WRITE_PROTECT       = 19,           // The media is write protected error code is 19.
    CRYPT_ERROR_SHARING_VIOLATION   = 32,           // The process con not access the file because it is being used by another process error code is 32.
    CRYPT_ERROR_DISK_FULL           = 112           // The disk is full, error code is 112.

} CRYPT_RESULT;


/*!
* @brief The crypt memory blob.
*/
typedef struct _CRYPTMEMBLOB 
{
    PBYTE  pbBlob;
    DWORD  dwBlobSize;

} CRYPTMEMBLOB, *LPCRYPTMEMBLOB;


/*!
* @brief encrypt key data's information
*/
typedef struct _CRYPTKEYBLOBINFO
{
    PBYTE pbKeyBlob;                            // The pointer to the memory blob.
    DWORD dwKeyBlobLen;                         // The length of the memory blob.

} CRYPTKEYBLOBINFO, *LPCRYPTKEYBLOBINFO;


END_NAMESPACE_COMMON

#endif // _SDKCRYPTDEF_H_
#endif // __cplusplus
