/*!
* @file SdkCrypt.h
* 
* @brief This file defines SdkCrypt class to encrypt, decrypt stream, create key, etc.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/12
*/

#ifdef __cplusplus 
#ifndef _SDKCRPYT_H_
#define _SDKCRPYT_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "SdkCryptKey.h"
#include "SdkCryptDef.h"

BEGIN_NAMESPACE_COMMON

#define ENCRYPT_BLOCK_SIZE                      32      // The crypt block size.
#define ENCRYPT_LAST_PART_VERIFYDATA_SIZE       16      // The last part size.

/*!
* @brief This class provides functions to encrypt and decrypt streams.
*/
class CLASS_DECLSPEC SdkCrypt
{
public:

    /*!
    * @brief The default constructor function.
    */
    SdkCrypt();

    /*!
    * @brief The constructor function.
    *
    * @param pCryptKey  [I/ ] The pointer to SdkCryptKey class which presents the crypt key.
    */
    SdkCrypt(IN SdkCryptKey *pCryptKey);

    /*!
    * @brief The default destructor function.
    */
    ~SdkCrypt();

    /*!
    * @brief Call this function to initialize the crypt scene. This function should
    *        be called before any operation of crypt. If you call CreateCryptKey, it is
    *        unnecessary to call this function.
    *
    * @return The CRYPT_RESULT value.
    */
    CRYPT_RESULT InitializeScene();

    /*!
    * @brief Create a cryptographic key.
    *
    * @param ppCryptKey     [ /O] The pointer to pointer to SdkCryptKey class, you should delete the memory.
    *
    * @return The CRYPT_RESULT value.
    */
    CRYPT_RESULT CreateCryptKey(OUT SdkCryptKey **ppCryptKey);

    /*!
    * @brief Encrypt the data.
    *
    * @param pbData         [I/O] The data buffer.
    * @param dwDataSize     [I/ ] The data buffer size, in bytes.
    * @param isFinal        [I/ ] Indicates whether the buffer is the last section.
    *
    * @return The CRYPT_RESULT value.
    */
    CRYPT_RESULT EncryptStream(IN OUT PBYTE pbData, DWORD dwDataSize, BOOL isFinal = FALSE);

    /*!
    * @brief Decrypt the data.
    *
    * @param pbData         [I/O] The data buffer.
    * @param dwDataSize     [I/ ] The data buffer size, in bytes.
    * @param isFinal        [I/ ] Indicates whether the buffer is the last section.
    *
    * @return The CRYPT_RESULT value.
    */
    CRYPT_RESULT DecryptStream(IN OUT PBYTE pbData, DWORD dwDataSize, BOOL isFinal = FALSE);

    /*!
    * @brief Get the checked byte size, if you want to alloc a memory, you should call this
    *        function to determine how much memory to be allocated..
    *
    * @param pdwCbSize      [I/O] The input and output byte size.
    * @param isFinal        [I/ ] Indicates whether the buffer is the last section.
    *
    * @return The CRYPT_RESULT value.
    */
    CRYPT_RESULT GetCheckedCbSize(IN OUT DWORD *pdwCbSize, BOOL isFinal = TRUE);

private:

    HCRYPTPROV          m_hCryptProvider;       // The handle to CSP.
    HCRYPTKEY           m_hCryptKey;            // The key to encrypt or decrypt.
    CRYPTKEYBLOBINFO    m_keyBlob;              // The key blob.
    CRYPTKEYBLOBINFO    m_xchgKeyBlob;          // The exchange key blob.
};

END_NAMESPACE_COMMON

#endif // _SDKCRPYT_H_
#endif // __cplusplus
