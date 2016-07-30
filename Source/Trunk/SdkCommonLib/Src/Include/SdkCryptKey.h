/*!
* @file SdkCryptKey.h
* 
* @brief This file defines SdkCryptKey class to create, save, load crypt key.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/12
*/

#ifdef __cplusplus
#ifndef _SDKCRYPTKEY_H_
#define _SDKCRYPTKEY_H_

#include "SdkCryptDef.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief SdkCryptKey class supplies 
*/
class CLASS_DECLSPEC SdkCryptKey
{
public:

    /*
    * The default constructor function.
    */
    SdkCryptKey();

    /*
    * The default destructor function.
    */
    ~SdkCryptKey();

    /*!
    * @brief Call this function to fill buffer of exchange key and cryptographic key.
    *
    * @param hCryptProvider     [I/ ] The CSP handle.
    * @param hCryptKey          [I/ ] The cryptographic key.
    * @param isUseDefaultXchKey [I/ ] Indicates whether use default exchange key.
    */
    BOOL Initialize(HCRYPTPROV hCryptProvider, HCRYPTKEY hKey, IN BOOL isUseDefaultXchKey);

    /*! 
    * @brief Constructor function.
    *
    * @param pbKey              [I/ ] The pointer to key.
    * @param dwbKeySize         [I/ ] The length of key.
    * @param pbExchangeKey      [I/ ] The pointer to exchange key.
    * @param dwExchangeKeySize  [I/ ] THe length of exchange key.
    */
    BOOL Initialize(IN const PBYTE pbKey, IN DWORD dwbKeySize, IN const PBYTE pbExchangeKey, IN DWORD dwExchangeKeySize);

    /*! 
    * @brief Save key to file.
    *
    * @param lpKeyFilePath      [I/ ] The wstring string to specify the key file path.
    * @param isSaveExchangeKey  [I/ ] Indicates whether save exchange key or not, default value is FALSE.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL SaveToFile(IN LPCTSTR lpKeyFilePath, IN BOOL isSaveExchangeKey = FALSE) const;

    /*! 
    * @brief Load key from file.
    *
    * @param lpKeyFilePath      [I/ ] The wstring string to specify the key file path.
    * @param isSaveExchangeKey  [I/ ] Indicates whether load exchange key or not, default value is FALSE.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL LoadFromFile(IN LPCTSTR lpKeyFilePath, IN BOOL isLoadExchangeKey = FALSE);

    /*!
    * @brief Get the key blob information.
    *
    * @return CRYPTKEYBLOBINFO object.
    */
    CRYPTKEYBLOBINFO GetKeyBlob() const;

    /*!
    * @brief Get the exchange key blob information.
    *
    * @return CRYPTKEYBLOBINFO object.
    */
    CRYPTKEYBLOBINFO GetExchKeyBlob() const;

private:

    /*
    * The copy constructor function.
    *
    * @param srcKey     [I/ ] The source object of SdkCryptKey class.
    */
    SdkCryptKey(IN const SdkCryptKey& srcKey);

    /*! 
    * @brief [=] override
    */
    SdkCryptKey& operator = (const SdkCryptKey& rightVal);

    /*! 
    * @brief Release the memory.
    */
    void ReleaseMemory();

    /*! 
    * @brief Set the file pointer to start or end. 
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL ClearFileContent(IN LPCTSTR lpFilePath) const;

private:

    CRYPTKEYBLOBINFO    m_keyBlobInfo;              // The key blob information.
    CRYPTKEYBLOBINFO    m_xchKeyBlobInfo;           // The exchange key blob information.
};

END_NAMESPACE_COMMON

#endif // _SDKCRYPTKEY_H_
#endif // __cplusplus
