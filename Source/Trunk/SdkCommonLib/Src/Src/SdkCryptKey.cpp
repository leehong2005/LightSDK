/*!
* @file SdkCryptKey.cpp
* 
* @brief This file defines SdkCryptKey class to create, save, load crypt key.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/12
*/

#include "stdafx.h"
#include "SdkCryptKey.h"

USING_NAMESPACE_COMMON

// The default exchange key data.
static const BYTE g_szDefaultXchgCryptKeyData[] = 
{
    0x07, 0x02, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00,
    0x52, 0x53, 0x41, 0x32, 0x00, 0x04, 0x00, 0x00,
    0x01, 0x00, 0x01, 0x00, 0xEF, 0x4E, 0x67, 0xB8,
    0x5A, 0x25, 0x9F, 0xB8, 0x56, 0x0C, 0x0E, 0x21,
    0xA4, 0xEA, 0xF1, 0x27, 0xE5, 0x98, 0x77, 0xD5,
    0x65, 0xE7, 0x1B, 0x4A, 0x6D, 0x72, 0x23, 0xF9,
    0xFB, 0x77, 0xE7, 0xF3, 0xB0, 0xD3, 0xAA, 0xF9,
    0x70, 0xE4, 0xB2, 0x68, 0x5F, 0xAD, 0xEF, 0xA7,
    0x7A, 0xCB, 0xEF, 0xB7, 0x85, 0x74, 0x94, 0x6E,
    0xCF, 0xD4, 0x61, 0x7C, 0xF4, 0x09, 0x13, 0xED,
    0x53, 0xCF, 0x97, 0x50, 0x1B, 0xFE, 0xDD, 0x8D,
    0x0F, 0xF3, 0xEA, 0xD6, 0xFA, 0xC7, 0x2E, 0x40,
    0x36, 0x7E, 0x19, 0xF0, 0x2B, 0xB8, 0x18, 0x9E,
    0x8D, 0x14, 0x0A, 0x73, 0x77, 0x5C, 0xE8, 0xEA,
    0x30, 0xD8, 0x8C, 0x78, 0x6F, 0xA6, 0x6D, 0xE0,
    0x31, 0x9D, 0x0A, 0xD3, 0x17, 0x0D, 0xD4, 0x32,
    0x77, 0x3F, 0x71, 0x66, 0x9E, 0x28, 0x6F, 0x26,
    0x97, 0x07, 0x57, 0xF7, 0x6B, 0x53, 0x26, 0x9B,
    0x90, 0x4E, 0x10, 0xC4, 0x0B, 0x1D, 0x45, 0xE3,
    0x8C, 0x87, 0x96, 0x4E, 0x01, 0x0F, 0xDE, 0x6F,
    0x4E, 0xC1, 0x7C, 0xB0, 0x48, 0x96, 0xAC, 0x13,
    0x5C, 0x5B, 0xD8, 0x8C, 0x13, 0x6F, 0xE1, 0x44,
    0x29, 0x97, 0x9E, 0x10, 0xF0, 0x4D, 0x67, 0x9E,
    0xE9, 0x97, 0xE7, 0x62, 0xCC, 0x83, 0xAC, 0xE5,
    0xA0, 0x6D, 0x19, 0xAB, 0x06, 0x85, 0xBF, 0xD8,
    0x89, 0xD7, 0x87, 0xB8, 0x93, 0xB0, 0x33, 0x05,
    0xEE, 0xD8, 0x28, 0xE6, 0x2D, 0x1C, 0xBC, 0xE6,
    0xE6, 0xB6, 0x73, 0xB9, 0x5D, 0xB5, 0xBB, 0xE9,
    0x55, 0x9E, 0x47, 0x0E, 0xE3, 0x30, 0x52, 0xA7,
    0xB1, 0x34, 0x8B, 0x77, 0xE6, 0x6F, 0x67, 0xD6,
    0x34, 0xC8, 0x87, 0xD6, 0xA3, 0x99, 0x9B, 0x46,
    0x38, 0x9A, 0x13, 0x5A, 0x1E, 0x1C, 0xC2, 0x2D,
    0x24, 0xCC, 0x14, 0x22, 0x77, 0xD0, 0x9E, 0x75,
    0x0A, 0x34, 0xEF, 0xA2, 0x35, 0x9A, 0x88, 0xFB,
    0x77, 0x7D, 0x13, 0xDA, 0x15, 0x80, 0xC5, 0xB8,
    0x2D, 0xA2, 0x7A, 0x5C, 0xF9, 0x71, 0xAC, 0x87,
    0xEE, 0x57, 0xFB, 0xD2, 0xA7, 0xAE, 0xF1, 0x7B,
    0x26, 0xC9, 0xB7, 0x68, 0xB5, 0xC4, 0xA0, 0xA6,
    0x04, 0x4B, 0xEF, 0x77, 0xB4, 0xBC, 0x33, 0xFA,
    0x27, 0x68, 0xC3, 0x7E, 0xFD, 0x50, 0xDC, 0x73,
    0x83, 0x40, 0x7F, 0x7E, 0xC8, 0x0F, 0xDD, 0xA0,
    0x83, 0xBF, 0x35, 0x54, 0x41, 0xF3, 0x43, 0x59,
    0x56, 0xDD, 0x60, 0x79, 0xF5, 0x57, 0x27, 0x0B,
    0xE1, 0xD1, 0xB2, 0x48, 0x18, 0x8E, 0xA1, 0xC1,
    0xF3, 0x4C, 0x8D, 0x87, 0x1E, 0xFD, 0x46, 0xAD,
    0x79, 0xBA, 0x50, 0xA4, 0x1F, 0x78, 0x55, 0xAF,
    0xAA, 0xB5, 0x95, 0x40, 0x7F, 0x59, 0x84, 0xB6,
    0x37, 0x81, 0x13, 0x6F, 0x0F, 0x2D, 0x8B, 0xBF,
    0x07, 0xAF, 0x45, 0x40, 0xB8, 0xDB, 0xB2, 0x30,
    0x52, 0x79, 0xE4, 0xBA, 0xF2, 0x37, 0xD9, 0xC5,
    0x17, 0x9C, 0x7B, 0xBB, 0xF4, 0x4D, 0xBB, 0xBA,
    0x67, 0xE4, 0xE0, 0xB8, 0x8E, 0x35, 0x07, 0x35,
    0x85, 0x99, 0xE1, 0x3A, 0x47, 0xE3, 0xCB, 0xF9,
    0xAD, 0x5F, 0x4C, 0x46, 0xBE, 0x35, 0x09, 0xD2,
    0xC2, 0x29, 0x70, 0xF7, 0x43, 0x49, 0xC6, 0xA2,
    0x05, 0x1E, 0xAD, 0x47, 0x11, 0xD3, 0xAA, 0x2D,
    0x90, 0x8F, 0xAA, 0xFB, 0xE8, 0x7F, 0x7C, 0xC3,
    0x9E, 0xDB, 0x6A, 0x7A, 0x72, 0x24, 0x66, 0x66,
    0xC0, 0xAE, 0xC5, 0x41, 0xE1, 0x07, 0x1B, 0xB4,
    0xFA, 0xDC, 0x89, 0xFA, 0x48, 0x1E, 0x92, 0x78,
    0x37, 0x31, 0xF6, 0xE0, 0x70, 0x71, 0x53, 0xD0,
    0x10, 0xFA, 0xA7, 0x02, 0xF3, 0xA7, 0x6B, 0x6B,
    0xA3, 0x78, 0x25, 0x0C, 0x66, 0xB7, 0x31, 0xB2,
    0xA9, 0xAE, 0x34, 0xC2, 0x00, 0x69, 0x07, 0x7C,
    0x06, 0x85, 0x06, 0x01, 0x3F, 0x65, 0x9F, 0xD9,
    0x69, 0xF0, 0xBE, 0x9D, 0x77, 0x96, 0xCA, 0x15,
    0x48, 0xBE, 0xEE, 0xD9, 0xAC, 0xD5, 0xE4, 0x0E,
    0x4D, 0xC6, 0x2A, 0xF9, 0x85, 0x07, 0x71, 0x46,
    0x7C, 0xC4, 0x24, 0x68, 0x6C, 0x0D, 0x62, 0x9F,
    0xD9, 0xDF, 0x7D, 0xA5, 0x18, 0x90, 0x79, 0x6E,
    0x67, 0x8A, 0x16, 0x8D, 0xB5, 0xBC, 0x11, 0x1B,
    0x5C, 0xE6, 0xA7, 0x71, 0x84, 0x8C, 0x37, 0x48,
    0x85, 0xF2, 0x84, 0x6A, 0x2E, 0x0C, 0x41, 0x0D,
    0x7C, 0xA2, 0x97, 0x01, 0x5B, 0x3D, 0xF6, 0xFE,
    0x28, 0xF7, 0x9F, 0x77, 
};


//////////////////////////////////////////////////////////////////////////

SdkCryptKey::SdkCryptKey()
{
    ZeroMemory(&m_keyBlobInfo,    sizeof(m_keyBlobInfo));
    ZeroMemory(&m_xchKeyBlobInfo, sizeof(m_xchKeyBlobInfo));
}

//////////////////////////////////////////////////////////////////////////

SdkCryptKey::~SdkCryptKey()
{
    ReleaseMemory();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptKey::Initialize(HCRYPTPROV hCryptProvider, HCRYPTKEY hKey, IN BOOL isUseDefaultXchKey)
{
    BOOL bResult = FALSE;

    // Handle to receiver's exchange public key
    HCRYPTKEY hExchageKey = (HCRYPTKEY)NULL;

    // Use the default exchange key.
    if ( isUseDefaultXchKey )
    {
        SAFE_DELETE_ARRAY(m_xchKeyBlobInfo.pbKeyBlob);
        m_xchKeyBlobInfo.dwKeyBlobLen = ARRAYSIZE(g_szDefaultXchgCryptKeyData);
        m_xchKeyBlobInfo.pbKeyBlob = new BYTE[m_xchKeyBlobInfo.dwKeyBlobLen];
        memcpy_s(
            m_xchKeyBlobInfo.pbKeyBlob,         // Destination buffer.
            m_xchKeyBlobInfo.dwKeyBlobLen,      // Copy bytes number.
            g_szDefaultXchgCryptKeyData,        // Source buffer.
            m_xchKeyBlobInfo.dwKeyBlobLen);     // Copy bytes number.
    }
    else
    {
        // Get the handle to the exchange public key (public/private key pair).
        if ( !CryptGetUserKey(hCryptProvider, AT_KEYEXCHANGE, &hExchageKey) )
        {
            if( NTE_NO_KEY == GetLastError() )
            {
                // No exchange key exists. Try to create one.

                bResult = CryptGenKey(
                    hCryptProvider,
                    AT_KEYEXCHANGE,
                    CRYPT_EXPORTABLE,
                    &hExchageKey);

                if ( !bResult )
                {
                    goto Cleanup;
                }
            }
        }

        // Determine the size of the EXCHANGE KEY blob and allocate memory.
        bResult = CryptExportKey(
            hExchageKey,                        // A handle to the key to be exported.
            0,                                  // Should be set zero if dwBlobType is PRIVATEKEYBLOB.
            PRIVATEKEYBLOB,                     // Used to transport public/private key pairs. Session key.
            0,                                  // Set zero.
            NULL,                               // Output buffer.
            &m_xchKeyBlobInfo.dwKeyBlobLen);    // The buffer size.

        if ( !bResult )
        {
            // Fail to determine blob size for exported key.
            goto Cleanup;
        }

        // First release the buffer.
        SAFE_DELETE_ARRAY(m_xchKeyBlobInfo.pbKeyBlob);
        // Allocate the memory for exchange key blob.
        m_xchKeyBlobInfo.pbKeyBlob = new BYTE[m_xchKeyBlobInfo.dwKeyBlobLen];

        // Export EXCHANGE KEY into a simple key BLOB.
        bResult = CryptExportKey(
            hExchageKey,                        // A handle to the key to be exported.
            0,                                  // Should be set zero if dwBlobType is PUBLICKEYBLOB.
            PRIVATEKEYBLOB,                     // Used to transport public/private key pairs.
            0,                                  // Set zero.
            m_xchKeyBlobInfo.pbKeyBlob,         // Output buffer.
            &m_xchKeyBlobInfo.dwKeyBlobLen);    // The buffer size.

        if ( !bResult )
        {
            // Fail to export key.
            goto Cleanup;
        }
    }

    // Create a cryptographic key from a blob buffer.
    bResult = CryptImportKey(
        hCryptProvider,                         // The CSP handle.
        m_xchKeyBlobInfo.pbKeyBlob,             // The blob buffer.
        m_xchKeyBlobInfo.dwKeyBlobLen,          // The blob buffer size.
        0,                                      // Flag
        CRYPT_EXPORTABLE,                       // The exportable.
        &hExchageKey);

    if ( !bResult )
    {
        goto Cleanup;
    }

    // Determines the key buffer size.
    bResult = CryptExportKey(
        hKey,
        hExchageKey,
        SIMPLEBLOB,
        0,
        NULL,
        &m_keyBlobInfo.dwKeyBlobLen);

    if ( !bResult )
    {
        goto Cleanup;
    }

    // First release the memory of key blob.
    SAFE_DELETE_ARRAY(m_keyBlobInfo.pbKeyBlob);
    // Allocate the memory for the key blob.
    m_keyBlobInfo.pbKeyBlob = new BYTE[m_keyBlobInfo.dwKeyBlobLen];

    // Export the key into a simple key BLOB.
    bResult = CryptExportKey(
        hKey,
        hExchageKey,
        SIMPLEBLOB,
        0,
        m_keyBlobInfo.pbKeyBlob,
        &m_keyBlobInfo.dwKeyBlobLen);

Cleanup:

    // Destroy the crypt object.
    if ( NULL != hExchageKey )
    {
        CryptDestroyKey(hExchageKey);
    }

    return bResult;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptKey::Initialize(IN const PBYTE pbKey, IN DWORD dwbKeySize, IN const PBYTE pbExchangeKey, IN DWORD dwExchangeKeySize)
{
    if ( (NULL != pbKey) && (dwbKeySize > 0) )
    {
        SAFE_DELETE_ARRAY(m_keyBlobInfo.pbKeyBlob);
        m_keyBlobInfo.dwKeyBlobLen = dwbKeySize;
        m_keyBlobInfo.pbKeyBlob = new BYTE[dwbKeySize];
        memcpy_s(m_keyBlobInfo.pbKeyBlob, dwbKeySize, pbKey, dwbKeySize);
    }

    if ( (NULL != pbExchangeKey) && (dwExchangeKeySize > 0) )
    {
        SAFE_DELETE_ARRAY(m_xchKeyBlobInfo.pbKeyBlob);
        m_xchKeyBlobInfo.dwKeyBlobLen = dwExchangeKeySize;
        m_xchKeyBlobInfo.pbKeyBlob = new BYTE[dwExchangeKeySize];
        memcpy_s(
            m_xchKeyBlobInfo.pbKeyBlob,
            dwExchangeKeySize,
            pbExchangeKey, 
            dwExchangeKeySize);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptKey::SaveToFile(IN LPCTSTR lpKeyFilePath, IN BOOL isSaveExchangeKey) const
{
    if ( (m_keyBlobInfo.dwKeyBlobLen < 0) || (NULL == m_keyBlobInfo.pbKeyBlob) )
    {
        return FALSE;
    }

    if ( !ClearFileContent(lpKeyFilePath) )
    {
        return FALSE;
    }

    // Open file or create a new one if not exist.
    HANDLE hFile = CreateFile(
        lpKeyFilePath,              // File name.
        FILE_WRITE_DATA,            // Desired access.
        0,                          // The object cannot be shared and cannot be opened again until the handle is closed. 
        NULL,                       // Not security attributes.
        CREATE_ALWAYS,              // Open always.
        FILE_ATTRIBUTE_NORMAL,      // Normal attribute.
        NULL);

    // Open or create file fails.
    if ( !ISVALIDHANDLE(hFile) )
    {
        return FALSE;
    }

    BOOL lResult = FALSE;
    DWORD dwBytesWritten = 0;

    // Write data byte length to specified file.
    BOOL res = WriteFile(
        hFile,
        (LPCVOID)&(m_keyBlobInfo.dwKeyBlobLen),
        (DWORD)sizeof(m_keyBlobInfo.dwKeyBlobLen),
        &dwBytesWritten,
        NULL);

    lResult = res && (dwBytesWritten == sizeof(m_keyBlobInfo.dwKeyBlobLen));
    if ( lResult )
    {
        // Write key data to file.
        res = WriteFile(
            hFile,
            (LPCVOID)(m_keyBlobInfo.pbKeyBlob),
            (DWORD)(m_keyBlobInfo.dwKeyBlobLen),
            &dwBytesWritten,
            NULL);

        lResult = res && (dwBytesWritten == m_keyBlobInfo.dwKeyBlobLen);
    }

    // If save exchange key.
    if ( isSaveExchangeKey && lResult )
    {
        // Write exchange key data length to file.
        BOOL res = WriteFile(
            hFile,
            (LPCVOID)&m_xchKeyBlobInfo.dwKeyBlobLen,
            (DWORD)sizeof(m_xchKeyBlobInfo.dwKeyBlobLen),
            &dwBytesWritten,
            NULL);

        lResult = res && (dwBytesWritten == sizeof(m_xchKeyBlobInfo.dwKeyBlobLen));
        if ( TRUE == lResult )
        {
            // Write exchange key data to file
            BOOL res = WriteFile(
                hFile, 
                (LPCVOID)(m_xchKeyBlobInfo.pbKeyBlob),
                (DWORD)(m_xchKeyBlobInfo.dwKeyBlobLen),
                &dwBytesWritten,
                NULL);

            lResult = res && (dwBytesWritten == m_xchKeyBlobInfo.dwKeyBlobLen);
        }
    }

    // Close the handle to file.
    SAFE_CLOSE_HANDLE(hFile);

    // If fail to write file, delete the crashed file.
    if ( !lResult )
    {
        ::DeleteFile(lpKeyFilePath);
    }

    return lResult;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptKey::LoadFromFile(IN LPCTSTR lpKeyFilePath, IN BOOL isLoadExchangeKey)
{
    HANDLE hFile = CreateFile(
        lpKeyFilePath,          // File path.
        FILE_READ_DATA,         // Read data access.
        FILE_SHARE_READ,        // Share read.
        NULL,                   // No security.
        OPEN_EXISTING,          // Open existing.
        FILE_ATTRIBUTE_NORMAL,  // Normal attribute.
        NULL);

    // Fail to open file.
    if ( !ISVALIDHANDLE(hFile) )
    {
        return FALSE;
    }

    BOOL lResult = FALSE;
    DWORD dwBytesRead = 0;
    DWORD dwFileSize = GetFileSize(hFile, NULL);

    // The size of file should be bigger than zero.
    if ( dwFileSize > 0 )
    {
        // First release the memory of key and exchange key.
        ReleaseMemory();

        // Read the key data length.
        lResult = ReadFile(
            hFile,
            (LPVOID)&m_keyBlobInfo.dwKeyBlobLen,
            (DWORD)sizeof(m_keyBlobInfo.dwKeyBlobLen),
            &dwBytesRead,
            NULL);

        lResult = (lResult && (sizeof(m_keyBlobInfo.dwKeyBlobLen) == dwBytesRead));
        if ( lResult )
        {
            if ( (m_keyBlobInfo.dwKeyBlobLen + sizeof(m_keyBlobInfo.dwKeyBlobLen)) <= dwFileSize && 
                 (0 != m_keyBlobInfo.dwKeyBlobLen) )
            {
                // Allocate the memory for key buffer.
                m_keyBlobInfo.pbKeyBlob = new BYTE[m_keyBlobInfo.dwKeyBlobLen];
                // Read the key data from file.
                BOOL res = ReadFile(
                    hFile,
                    (LPVOID)m_keyBlobInfo.pbKeyBlob,
                    m_keyBlobInfo.dwKeyBlobLen,
                    &dwBytesRead,
                    NULL);

                lResult = res && (dwBytesRead == m_keyBlobInfo.dwKeyBlobLen);
            }
        }

        // Read exchange key data from file.
        if ( isLoadExchangeKey && lResult )
        {
            lResult = ReadFile(
                hFile,
                (LPVOID)&(m_xchKeyBlobInfo.dwKeyBlobLen),
                (DWORD)sizeof(m_xchKeyBlobInfo.dwKeyBlobLen),
                &dwBytesRead,
                NULL);

            lResult = ( lResult && 
                       (sizeof(m_xchKeyBlobInfo.dwKeyBlobLen) == dwBytesRead) &&
                       (dwBytesRead > 0) );

            if ( lResult )
            {
                // Allocate the memory for exchange buffer.
                m_xchKeyBlobInfo.pbKeyBlob = new BYTE[m_xchKeyBlobInfo.dwKeyBlobLen];
                // Read the exchange key data from file.
                lResult = ReadFile(
                    hFile,
                    (LPVOID)(m_xchKeyBlobInfo.pbKeyBlob),
                    (DWORD)(m_xchKeyBlobInfo.dwKeyBlobLen),
                    &dwBytesRead,
                    NULL);

                lResult = lResult && (dwBytesRead == m_xchKeyBlobInfo.dwKeyBlobLen);
            }
        }
        else
        {
            DWORD dwcbCount = ARRAYSIZE(g_szDefaultXchgCryptKeyData);
            m_xchKeyBlobInfo.dwKeyBlobLen = dwcbCount;
            // Allocate the memory for exchange buffer.
            m_xchKeyBlobInfo.pbKeyBlob = new BYTE[dwcbCount];
            memcpy_s(
                m_xchKeyBlobInfo.pbKeyBlob,
                dwcbCount,
                g_szDefaultXchgCryptKeyData,
                dwcbCount);
        }
    }

    SAFE_CLOSE_HANDLE(hFile);

    return lResult;
}

//////////////////////////////////////////////////////////////////////////

CRYPTKEYBLOBINFO SdkCryptKey::GetKeyBlob() const
{
    return m_keyBlobInfo;
}

//////////////////////////////////////////////////////////////////////////

CRYPTKEYBLOBINFO SdkCryptKey::GetExchKeyBlob() const
{
    return m_xchKeyBlobInfo;
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptKey::ReleaseMemory()
{
    SAFE_DELETE_ARRAY(m_keyBlobInfo.pbKeyBlob);
    SAFE_DELETE_ARRAY(m_xchKeyBlobInfo.pbKeyBlob);

    m_keyBlobInfo.dwKeyBlobLen    = 0;
    m_xchKeyBlobInfo.dwKeyBlobLen = 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptKey::ClearFileContent(IN LPCTSTR lpFilePath) const
{
    if ( !PathFileExists(lpFilePath) )
    {
        return TRUE;
    }

    BOOL lResult = FALSE;

    if ( SetFileAttributes(lpFilePath, FILE_ATTRIBUTE_NORMAL) )
    {
        // Open the existing file.
        HANDLE hFile = CreateFile(
            lpFilePath,             // File path.
            FILE_WRITE_DATA,        // Write access
            FILE_SHARE_READ,        // Can read.
            NULL,                   // No security.
            OPEN_EXISTING,          // Open existing always.
            FILE_ATTRIBUTE_NORMAL,  // Normal attribute.
            NULL);

        // Set the file pointer for specified file.
        if ( INVALID_SET_FILE_POINTER != SetFilePointer(hFile, 0, NULL, FILE_BEGIN) )
        {
            // Set the file size to zero.
            lResult = SetEndOfFile(hFile);
        }

        SAFE_CLOSE_HANDLE(hFile);
    }

    return lResult;
}
