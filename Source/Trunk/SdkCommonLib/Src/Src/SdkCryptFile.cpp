/*!
* @file SdkCryptFile.cpp
* 
* @brief This file defines SdkCryptFile class to encrypt, decrypt files.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/22
*/

#include "stdafx.h"
#include "Resource.h"
#include "SdkCryptFile.h"
#include "SdkLogger.h"
#include "SdkUserInfoUtil.h"
#include "SdkCommonHelper.h"
#include "SdkBase64Util.h"

USING_NAMESPACE_COMMON


/*!
* @brief The crypt header of the encrypted files.
*/
struct NAMESPACE_COMMONLIB:: _CRYPTHEADER
{
    DWORD   dwIdentity;                             // The flag of the encrypted file.
    BYTE    cbPassword[MAX_PATH * sizeof(TCHAR)];   // The password of this file.
    BYTE    cbUserName[MAX_PATH * sizeof(TCHAR)];   // The current username.
    BYTE    cbFileName[MAX_PATH * sizeof(TCHAR)];   // The file name.
    DWORD   dwFlagsAndAttributes;                   // The flag and attributes.
    UINT64  nDataBegin;                             // The beginning of the data.
    UINT64  nDataLength;                            // The data length.
};


/*!
* @brief The DESTFILEINFO structure.
*/
struct NAMESPACE_COMMONLIB::_DESTFILEINFO 
{
    TCHAR           szDestFilePath[MAX_PATH]; // The destination file full path
    HANDLE          hDestFile;                // The handle of the destination file
    HANDLE          hDestMapFile;             // The map handle of destination file
    BOOL            isSuccess;                // Create or open the destination file success or failure
    DWORD           dwFlagsAndAttributes;     // The flag and attributes
    CRYPTHEADER     destFileHeader;           // The struct of the CRYPTHEADER
};


/*!
* @brief The files information.
*/
struct NAMESPACE_COMMONLIB::_FILEINFOS
{
    TCHAR           szFilePath[MAX_PATH];     // File name.
    HANDLE          hFile;                    // The handle of the file
    HANDLE          hMapFile;                 // The handle of the file mapping
    UINT64          nFileSize;                // File size
    DWORD           dwFlagsAndAttributes;     // File attributes
    FLOAT           fPencent;                 // Finished percent
    BOOL            isEncrypted;              // This member determines the file is encrypted or not
    BOOL            isSuccess;                // This member determines encrypt or open file success or not
    BOOL            isBigFile;                // This member determines this file is big or small file
    BOOL            isReplaced;               // Replace the old file or not
    DESTFILEINFO    sDestFileInfo;            // The struct of the destination file information
};


#define ENCRYPTED_FILE_EXTENDNAME    _T(".tofp")             // the extension of the encrypted files
#define MAX_WRITE_RATE               16                      // this value must be multiples of allocation granularity
#define ALLOCATION_GRANULARITY       64 * 1024               // the system allocation granularity, this min value is 64KB
#define CRITICAL_VALUE               10 * 1048576            // the critical value is 10MB, below the value,use normal I/O
                                                             // when over the value, use file mapping
#define ENCRYPTED_FILE_IDENTIFIER    0x55504654              // 0x55504654 is the DWORD value of "TFPU"
#define MAX_BUFFER_SIZE              64 * 1024               // the buffer size, this value must be multiple of 16
#define HEADER_SIZE                  sizeof(CRYPTHEADER)     // the header size


//////////////////////////////////////////////////////////////////////////

CryptString SdkCryptFile::EncryptFileName(IN const CryptString& strFileName)
{
    CryptString strRetFileName = strFileName;
    DWORD dwOriSize  = (strFileName.size() + 1) * sizeof(TCHAR);
    DWORD dwDataSize = dwOriSize;
    SdkCrypt cryptObj;
    CRYPT_RESULT nResult = CRYPT_ERROR_FAIL;

    // Get the appropriate size of data buffer.
    nResult = cryptObj.GetCheckedCbSize(&dwDataSize, TRUE);

    // Initialize the crypt scene.
    nResult = cryptObj.InitializeScene();
    if (CRYPT_ERROR_SUCCEED == nResult)
    {
        BYTE *pData = new BYTE[dwDataSize];
        ZeroMemory(pData, dwDataSize);
        memcpy_s(pData, dwDataSize, (void*)strFileName.c_str(), dwOriSize);

        // Encrypt the data.
        nResult = cryptObj.EncryptStream(pData, dwDataSize, TRUE);
        if (CRYPT_ERROR_SUCCEED == nResult)
        {
            CHAR szConvertBuffer[512] = { 0 };
            CHAR *pEncryptData = new CHAR[dwDataSize + 2];
            ZeroMemory(pEncryptData, dwDataSize+ 2);
            memcpy_s(pEncryptData, dwDataSize, pData, dwDataSize);
            SdkBase64Util::Encode(pEncryptData, dwDataSize, szConvertBuffer, 512);

#ifdef UNICODE
            strRetFileName = SdkCommonHelper::AnsiToUnicode(szConvertBuffer);
#else
            strRetFileName = CryptString(szConvertBuffer);
#endif // UNICODE

            SAFE_DELETE_ARRAY(pEncryptData);
        }

        SAFE_DELETE_ARRAY(pData);
    }

    return strRetFileName;
}

//////////////////////////////////////////////////////////////////////////

SdkCryptFile::SdkCryptFile(BOOL isDefProgressBar) : m_nFileIndex(0),
                                                    m_nFileNumbers(0),
                                                    m_hasCancelCrypt(FALSE),
                                                    m_bDelOriginalFiles(FALSE),
                                                    m_pCryptFileSink(NULL),
                                                    m_pCrypt(NULL),
                                                    m_pProgressDialog(NULL)
{
    // Get the allocation granularity, typical value is 65536.
    SYSTEM_INFO sinf;
    GetSystemInfo(&sinf);
    m_dwAllocationGranularity = sinf.dwAllocationGranularity;

    // Get the user name and domain name.
    USERINFODATA *pUserInfo = NULL;
    SdkUserInfoUtil::GetCurLogonUserInfo(&pUserInfo);
    if (NULL != pUserInfo)
    {
        m_strUserName.append(pUserInfo->lpUserName);
        m_strDomainName.append(pUserInfo->lpLogonDomain);
        m_strDomainUserName.append(pUserInfo->lpLogonServer);
    }
    SdkUserInfoUtil::DeleteUserInfoData(pUserInfo);
    pUserInfo = NULL;

    // Allocate the memory for header block.
    m_HeaderBlock.dwBlobSize = 
        HEADER_SIZE + ENCRYPT_BLOCK_SIZE + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
    m_HeaderBlock.pbBlob = new BYTE[m_HeaderBlock.dwBlobSize];

    // Allocate the memory for small file block.
    m_SmallFileBlock.dwBlobSize = 
        MAX_BUFFER_SIZE + ENCRYPT_BLOCK_SIZE + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
    m_SmallFileBlock.pbBlob = new BYTE[m_SmallFileBlock.dwBlobSize];

    if (isDefProgressBar)
    {
        m_pProgressDialog = new SdkProgressDialog();
    }
}

//////////////////////////////////////////////////////////////////////////

SdkCryptFile::~SdkCryptFile()
{
    int nSize = (int)m_vFileInfoList.size();
    for (int i = 0; i < nSize; ++i)
    {
        CloseHandles(m_vFileInfoList[i]);
        SAFE_DELETE(m_vFileInfoList[i]);
    }

    SAFE_DELETE(m_pCrypt);
    SAFE_DELETE(m_pProgressDialog);
    SAFE_DELETE_ARRAY(m_SmallFileBlock.pbBlob);
    SAFE_DELETE_ARRAY(m_HeaderBlock.pbBlob);
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::BeginCrypt(IN CRYPT_OP_TYPE operationType)
{
    CRYPT_RESULT nResult = CRYPT_ERROR_SUCCEED;

    // The operation type is invalid.
    if ( CRYPT_OP_ENCRYPT != operationType && CRYPT_OP_DECRYPT != operationType )
    {
        nResult = CRYPT_ERROR_INVALIDPARAM;
        OnCryptError(FormatErrorMessage(nResult, _T("")).c_str());
        return nResult;
    }

    // Initialize the data.
    if (CRYPT_ERROR_SUCCEED != (nResult = InitCryptData(operationType)))
    {
        OnCryptError(FormatErrorMessage(nResult, _T("")).c_str());
        return nResult;
    }

    // initialize the crypt scene.
    if (CRYPT_ERROR_SUCCEED != (nResult = InitCryptScene()))
    {
        OnCryptError(FormatErrorMessage(nResult, _T("")).c_str());
        return nResult;
    }

    OnCryptBegin(operationType);

    int nSize = (int)m_vFileInfoList.size();
    for (int i = 0; i < nSize; ++i)
    {
        // The user cancel the operation.
        if ( HasCancelled() )
        {
            break;
        }

        LPFILEINFOS lpFileInfo = m_vFileInfoList[i];
        OnCryptNext(i, lpFileInfo->szFilePath);

        // Do encrypt or decrypt operation.
        if (CRYPT_OP_ENCRYPT == operationType)
        {
            nResult = EncryptFile(lpFileInfo);
        }
        else
        {
            nResult = DecryptFile(lpFileInfo);
        }

        CloseHandles(lpFileInfo);

        // Show the error.
        if ( (CRYPT_ERROR_SUCCEED != nResult) && (CRYPT_ERROR_CANCEL != nResult) )
        {
            // Show the error message.
            CryptString strErrorMsg(FormatErrorMessage(nResult, lpFileInfo->szFilePath));
            OnCryptError(strErrorMsg.c_str());
        }
    }

    OnCryptFinish();

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::SetCryptPassword(IN LPCTSTR lpPassword)
{
    if (NULL != lpPassword)
    {
        m_strPassword = lpPassword;
    }

    return (NULL != lpPassword) ? CRYPT_ERROR_SUCCEED : CRYPT_ERROR_INVALIDPARAM;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::SetCryptKeyPath(IN LPCTSTR lpKeyPath)
{
    if ( NULL != lpKeyPath )
    {
        m_strCryptKeyPath = lpKeyPath;
    }

    return (NULL != lpKeyPath) ? CRYPT_ERROR_SUCCEED : CRYPT_ERROR_INVALIDPARAM;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::SetCryptFiles(IN const vector<CryptString>& vctFilePathList, IN BOOL isDelOriginalFile)
{
    m_bDelOriginalFiles = isDelOriginalFile;
    m_vFilePathList.clear();
    m_vFilePathList.assign(vctFilePathList.begin(), vctFilePathList.end());

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::SetCryptFileSink(IN ICryptFileNotify *pCryptFileSink)
{
    m_pCryptFileSink = pCryptFileSink;

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::CancelCrypt()
{
    m_hasCancelCrypt = TRUE;

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CryptString  SdkCryptFile::GetErrorString(CRYPT_RESULT nResultCode)
{
    CryptString strError = FormatErrorMessage(nResultCode, L"");
    return strError;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::InitCryptData(IN CRYPT_OP_TYPE operationType)
{
    BOOL isEncrypt = (CRYPT_OP_ENCRYPT == operationType) ? TRUE : FALSE;

    for each (CryptString strPath in m_vFilePathList)
    {
        if ( !::PathFileExists(strPath.c_str()) )
        {
            continue;
        }

        LPTSTR lpFileExt = PathFindExtension(strPath.c_str());
        // Check the file whether is encrypted file.
        BOOL isEncrypted = (0 == _tcsicmp(lpFileExt, ENCRYPTED_FILE_EXTENDNAME));
        if (isEncrypt != isEncrypted)
        {
            LPFILEINFOS pFileInfo = new FILEINFOS();
            ZeroMemory(pFileInfo, sizeof(FILEINFOS));

            _tcscpy_s(pFileInfo->szFilePath, MAX_PATH, strPath.c_str());
            _tcscpy_s(pFileInfo->sDestFileInfo.szDestFilePath,
                MAX_PATH,
                strPath.c_str());

            pFileInfo->isReplaced  = TRUE;
            pFileInfo->isEncrypted = isEncrypted;

            m_vFileInfoList.push_back(pFileInfo);
        }
    }

    m_nFileNumbers = (UINT32)m_vFileInfoList.size();

    return (0 == m_nFileNumbers) ? CRYPT_ERROR_NOFILES : CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::InitCryptScene()
{
    CRYPT_RESULT nResult = CRYPT_ERROR_SUCCEED;

    // First load key from user's folder.
    SdkCryptKey cryptKey;
    BOOL isSucceed = cryptKey.LoadFromFile(m_strCryptKeyPath.c_str());

    // Fail to load key from specified file.
    if (!isSucceed)
    {
        // Create a new key.
        SdkCryptKey *pCryptKey = NULL;
        m_pCrypt = new SdkCrypt();
        nResult = m_pCrypt->CreateCryptKey(&pCryptKey);
        if (CRYPT_ERROR_SUCCEED == nResult)
        {
            isSucceed = pCryptKey->SaveToFile(m_strCryptKeyPath.c_str());
        }

        if (!isSucceed)
        {
            nResult = CRYPT_ERROR_FAIL;
        }

        SAFE_DELETE(pCryptKey);
    }
    else
    {
        m_pCrypt = new SdkCrypt(&cryptKey);
        nResult = m_pCrypt->InitializeScene();
    }

    if (CRYPT_ERROR_SUCCEED != nResult)
    {
        SAFE_DELETE(m_pCrypt);
    }

    return nResult;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::InitCryptHeader(OUT LPFILEINFOS lpDestFileInfos)
{
    if ( NULL == lpDestFileInfos )
    {
        return CRYPT_ERROR_FAIL;
    }

    // set the identity value, is ENCRYPTED_FILE_IDENTIFIER
    lpDestFileInfos->sDestFileInfo.destFileHeader.dwIdentity = ENCRYPTED_FILE_IDENTIFIER;

    // Set current user name
    memcpy(lpDestFileInfos->sDestFileInfo.destFileHeader.cbUserName,
        m_strDomainUserName.c_str(),
        (m_strDomainUserName.length() + 1) * sizeof(TCHAR));

    // Get the file name from the file full path
    CryptString strFileName = GetFileName(lpDestFileInfos->szFilePath);

    memcpy(lpDestFileInfos->sDestFileInfo.destFileHeader.cbFileName,
        strFileName.c_str(),
        (strFileName.length() + 1) * sizeof(TCHAR));    // Should includes '\0'.

    memcpy(lpDestFileInfos->sDestFileInfo.destFileHeader.cbPassword,
        m_strPassword.c_str(),
        (m_strPassword.length() + 1) * sizeof(TCHAR));

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::ReadCryptHeader(OUT LPFILEINFOS lpFileInfos)
{
    if (NULL == lpFileInfos)
    {
        return CRYPT_ERROR_FAIL;
    }

    CRYPTHEADER cryptHeader = { 0 };    // CRYPTHEADER structure.
    DWORD dwBytesRead = 0;              // Bytes to read.
    DWORD dwHeaderSize = HEADER_SIZE;   // CRYPTHEADER size.
    BOOL isLastBlock = FALSE;           // Last block.

    if ( (!ISVALIDHANDLE(lpFileInfos->hFile)) ||      // Handle is invalid.
         (lpFileInfos->nFileSize < dwHeaderSize) ||   // Size is invalid.
         (!lpFileInfos->isEncrypted) )                // File is not encrypted.
    {
        return CRYPT_ERROR_INVALIDFILE;
    }

    DWORD dwMod = dwHeaderSize % ENCRYPT_BLOCK_SIZE;
    if (0 != dwMod)
    {
        // add the additional bytes to the multiple of ENCRYPT_BLOCK_SIZE
        dwHeaderSize += ENCRYPT_BLOCK_SIZE - dwMod;
    }

    if (NULL == m_HeaderBlock.pbBlob)
    {
        return CRYPT_ERROR_FAIL;
    }
    ZeroMemory(m_HeaderBlock.pbBlob, m_HeaderBlock.dwBlobSize);

    // Read the data from the original file
    BOOL isSucceed = ReadFile(
        lpFileInfos->hFile,
        m_HeaderBlock.pbBlob,
        dwHeaderSize,
        &dwBytesRead,
        NULL);

    if (!isSucceed)
    {
        return CRYPT_ERROR_READDATA;
    }

    // Decrypt the data buffer
    CRYPT_RESULT nResult = m_pCrypt->DecryptStream(
        m_HeaderBlock.pbBlob,
        dwHeaderSize,
        isLastBlock);

    if (CRYPT_ERROR_SUCCEED != nResult)
    {
        return CRYPT_ERROR_DECRYPT;
    }

    // Copy data in buffer to the struct
    memcpy(&cryptHeader, m_HeaderBlock.pbBlob, HEADER_SIZE);

    // Set the header
    lpFileInfos->sDestFileInfo.destFileHeader = cryptHeader;
    lpFileInfos->sDestFileInfo.dwFlagsAndAttributes = cryptHeader.dwFlagsAndAttributes;

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::CheckCryptHeader(IN const CRYPTHEADER& cryptHeader)
{
    // Invalid identity.
    if (ENCRYPTED_FILE_IDENTIFIER != cryptHeader.dwIdentity)
    {
        return CRYPT_ERROR_INVALIDFILE;
    }

    CryptString strPassword((TCHAR*)cryptHeader.cbPassword);
    CryptString strDomainUserName((TCHAR*)cryptHeader.cbUserName);

    BOOL isPWDValid  = (0 == m_strPassword.compare(strPassword));
    BOOL isUserValid = (0 == _tcsicmp(m_strDomainUserName.c_str(), strDomainUserName.c_str()));

    if ( !isPWDValid )
    {
        return CRYPT_ERROR_INVALIDPWD;
    }

    if ( !isUserValid )
    {
        return CRYPT_ERROR_INVALIDFILE;
    }

    // Valid username and password.
    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::WriteCryptHeader(IN LPFILEINFOS lpDestFileInfos)
{
    if (NULL == lpDestFileInfos)
    {
        return CRYPT_ERROR_FAIL;
    }

    CRYPTHEADER cryptHeader = { 0 };    // The header structure.
    BOOL isLastBlock = FALSE;           // The last block.
    DWORD dwBytesWritten = 0;           // The written bytes.
    DWORD dwHeaderSize = HEADER_SIZE;   // The size of CRYPTHEADER structure.
    HANDLE hOutFile = lpDestFileInfos->sDestFileInfo.hDestFile;

    DWORD dwMod = dwHeaderSize % ENCRYPT_BLOCK_SIZE;
    if ( 0 != dwMod )
    {
        // Add the bytes size to multiple of ENCRYPT_BLOCK_SIZE
        dwHeaderSize += ENCRYPT_BLOCK_SIZE - dwMod;
    }

    // The file is big file, uses file mapping
    if (lpDestFileInfos->isBigFile)
    {
        lpDestFileInfos->sDestFileInfo.destFileHeader.nDataBegin  = m_dwAllocationGranularity;
        lpDestFileInfos->sDestFileInfo.destFileHeader.nDataLength = lpDestFileInfos->nFileSize;
        cryptHeader = lpDestFileInfos->sDestFileInfo.destFileHeader;

        PBYTE pDestMapBuffer = (PBYTE)MapViewOfFile(
            lpDestFileInfos->sDestFileInfo.hDestMapFile,
            FILE_MAP_READ | FILE_MAP_WRITE,
            0,
            0,
            dwHeaderSize);

        if (NULL == pDestMapBuffer)
        {
            return CRYPT_ERROR_ENCRYPT;
        }

        // Copy the header memory to buffer.
        memcpy_s(pDestMapBuffer, dwHeaderSize, &cryptHeader, dwHeaderSize);

        // Encrypt the data buffer
        CRYPT_RESULT nResult = m_pCrypt->EncryptStream(pDestMapBuffer, dwHeaderSize, FALSE);
        // Fail to encrypt data buffer.
        if (CRYPT_ERROR_SUCCEED != nResult)
        {
            UnmapViewOfFile(pDestMapBuffer);

            return CRYPT_ERROR_ENCRYPT;
        }

        UnmapViewOfFile(pDestMapBuffer);
    }
    else
    {
        lpDestFileInfos->sDestFileInfo.destFileHeader.nDataBegin  = dwHeaderSize;
        lpDestFileInfos->sDestFileInfo.destFileHeader.nDataLength = lpDestFileInfos->nFileSize;
        cryptHeader = lpDestFileInfos->sDestFileInfo.destFileHeader;

        if (NULL == m_HeaderBlock.pbBlob)
        {
            return CRYPT_ERROR_ENCRYPT;
        }
        ZeroMemory(m_HeaderBlock.pbBlob, m_HeaderBlock.dwBlobSize);

        // Copy the header to blob.
        memcpy(m_HeaderBlock.pbBlob, &cryptHeader, dwHeaderSize);

        // Encrypt the data buffer
        CRYPT_RESULT nResult = m_pCrypt->EncryptStream(
            m_HeaderBlock.pbBlob,
            dwHeaderSize,
            isLastBlock);

        if (CRYPT_ERROR_SUCCEED != nResult)
        {
            return CRYPT_ERROR_ENCRYPT;
        }

        // Use normal I/O to operate the file
        BOOL isSucceed = WriteFile(
            hOutFile,
            m_HeaderBlock.pbBlob,
            dwHeaderSize,
            &dwBytesWritten,
            NULL);

        if ( !isSucceed || (dwBytesWritten != dwHeaderSize) )
        {
            return CRYPT_ERROR_WRITEDATA;
        }
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::EncryptFile(IN LPFILEINFOS lpFileInfos)
{
    CRYPT_RESULT nResult = CRYPT_ERROR_SUCCEED;

    if (CRYPT_ERROR_SUCCEED != (nResult = OpenFile(lpFileInfos)))
    {
        return nResult;
    }

    // Get the path of destination files.
    GetDestFilePath(lpFileInfos, TRUE);

    // Create the files.
    if (CRYPT_ERROR_SUCCEED != (nResult = CreateFile(lpFileInfos)))
    {
        return nResult;
    }

    if ( !lpFileInfos->isSuccess || !lpFileInfos->sDestFileInfo.isSuccess )
    {
        DeleteFiles(lpFileInfos, FALSE);
        return CRYPT_ERROR_ENCRYPT;
    }

    // initialize the crypt header
    if ( CRYPT_ERROR_SUCCEED != (nResult = InitCryptHeader(lpFileInfos)) )
    {
        return nResult;
    }

    // write the crypt header to the destination file
    if ( CRYPT_ERROR_SUCCEED != (nResult = WriteCryptHeader(lpFileInfos)) )
    {
        DeleteFiles(lpFileInfos, FALSE);
        return nResult;
    }

    nResult = lpFileInfos->isBigFile ? EncryptBigFile(lpFileInfos) : EncryptSmallFile(lpFileInfos);

    if ( CRYPT_ERROR_SUCCEED != nResult )
    {
        DeleteFiles(lpFileInfos, FALSE);
        return nResult;
    }

    // Delete the original file
    if ( this->m_bDelOriginalFiles )
    {
        DeleteFiles(lpFileInfos);
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::EncryptBigFile(IN LPFILEINFOS lpFileInfos)
{
    if ( NULL == lpFileInfos )
    {
        return CRYPT_ERROR_FAIL;
    }

    if ( (NULL == lpFileInfos->hMapFile) || (NULL == lpFileInfos->sDestFileInfo.hDestMapFile) )
    {
        return CRYPT_ERROR_INVALID_HANDLE;
    }

    DWORD dwBlockSize = MAX_WRITE_RATE * m_dwAllocationGranularity;

    BOOL   isLastBlock      = FALSE;                        // last block or not
    UINT64 dwSrcFileOffset  = 0;                            // the offset of source file
    UINT64 dwDestFileOffset = m_dwAllocationGranularity;    // the offset of destination file
    UINT64 nFileLeftSize    = lpFileInfos->nFileSize;       // the bytes not map
    DWORD  dwBytesToMap     = dwBlockSize;                  // the number of bytes to map

    // cycle to map the file
    while (nFileLeftSize > 0)
    {
        // User cancels the operation
        if ( HasCancelled() )
        {
            return CRYPT_ERROR_CANCEL;
        }

        if (nFileLeftSize <= dwBlockSize)
        {
            isLastBlock = TRUE;

            // Update the variable
            dwBytesToMap = (DWORD)nFileLeftSize;
            dwBlockSize  = dwBytesToMap;
            DWORD dwMod = dwBytesToMap % ENCRYPT_BLOCK_SIZE;
            if ( 0 != dwMod )
            {
                // The last block, append ENCRYPT_LAST_PART_VERIFYDATA_SIZE.
                dwBlockSize += ENCRYPT_BLOCK_SIZE - dwMod + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
            }
        }

        PBYTE pSrcMapBuffer = (PBYTE)MapViewOfFile(
            lpFileInfos->hMapFile,
            FILE_MAP_READ,
            (DWORD)(dwSrcFileOffset >> 32),
            (DWORD)(dwSrcFileOffset & 0xFFFFFFFF),
            dwBytesToMap);

        if (NULL == pSrcMapBuffer)
        {
            return CRYPT_ERROR_INVALID_HANDLE;
        }

        // the map of the destination file
        PBYTE pDestMapBuffer = (PBYTE)MapViewOfFile(
            lpFileInfos->sDestFileInfo.hDestMapFile,
            FILE_MAP_READ | FILE_MAP_WRITE,
            (DWORD)(dwDestFileOffset >> 32),
            (DWORD)(dwDestFileOffset & 0xFFFFFFFF),
            dwBlockSize);

        if (NULL == pDestMapBuffer)
        {
            UnmapViewOfFile(pSrcMapBuffer);
            return CRYPT_ERROR_INVALID_HANDLE;
        }

        memcpy_s(pDestMapBuffer, dwBlockSize, pSrcMapBuffer, dwBytesToMap);

        // Encrypt the data buffer.
        CRYPT_RESULT nResult = m_pCrypt->EncryptStream(
            pDestMapBuffer,
            dwBlockSize,
            isLastBlock);

        if (CRYPT_ERROR_SUCCEED != nResult)
        {
            UnmapViewOfFile(pSrcMapBuffer);
            UnmapViewOfFile(pDestMapBuffer);

            return CRYPT_ERROR_ENCRYPT;
        }

        // Update the variables
        dwSrcFileOffset  += dwBytesToMap;
        dwDestFileOffset += dwBlockSize;
        nFileLeftSize    -= dwBytesToMap;

        // Update the percent
        lpFileInfos->fPencent = ((FLOAT)dwSrcFileOffset / (FLOAT)lpFileInfos->nFileSize) * 100;

        SetPercent();

        // clear up the data buffer
        UnmapViewOfFile(pSrcMapBuffer);
        UnmapViewOfFile(pDestMapBuffer);
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::EncryptSmallFile(IN LPFILEINFOS lpFileInfos)
{
    if (NULL == lpFileInfos)
    {
        return CRYPT_ERROR_FAIL;
    }

    HANDLE hFile    = lpFileInfos->hFile;
    HANDLE hOutFile = lpFileInfos->sDestFileInfo.hDestFile;

    if ( !ISVALIDHANDLE(hFile) || !ISVALIDHANDLE(hOutFile) )
    {
        return CRYPT_ERROR_INVALID_HANDLE;
    }

    DWORD dwDestOffset = (DWORD)lpFileInfos->sDestFileInfo.destFileHeader.nDataBegin;
    DWORD dwDataLength = (DWORD)lpFileInfos->sDestFileInfo.destFileHeader.nDataLength;
    DWORD dwBufferSize = (MAX_BUFFER_SIZE >= dwDataLength) ? dwDataLength : MAX_BUFFER_SIZE;

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    SetFilePointer(hOutFile, dwDestOffset, NULL, FILE_BEGIN);

    if ( NULL == m_SmallFileBlock.pbBlob )
    {
        return CRYPT_ERROR_FAIL;
    }

    BOOL isLastBlock     = FALSE;                   // last block or not
    DWORD dwLeftSize     = dwDataLength;            // the left size which not read
    DWORD dwBytesRead    = 0;                       // the number of bytes always read
    DWORD dwBytesWritten = 0;                       // the number of bytes always written

    // When the file is 0 bytes, add the additional bytes.
    if ( 0 == dwLeftSize )
    {
        dwLeftSize = ENCRYPT_BLOCK_SIZE + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
        dwBufferSize = dwLeftSize;

        ZeroMemory(m_SmallFileBlock.pbBlob, m_SmallFileBlock.dwBlobSize);
        // Encrypt the data
        CRYPT_RESULT nResult = m_pCrypt->EncryptStream(
            m_SmallFileBlock.pbBlob,
            dwBufferSize,
            TRUE);

        if ( CRYPT_ERROR_SUCCEED != nResult )
        {
            return CRYPT_ERROR_ENCRYPT;
        }

        // Write the data to destination file
        if ( !WriteFile(hOutFile, m_SmallFileBlock.pbBlob, dwBufferSize, &dwBytesWritten, NULL) )
        {
            return CRYPT_ERROR_WRITEDATA;
        }

        return CRYPT_ERROR_SUCCEED;
    }

    while ( dwLeftSize > 0 )
    {
        ZeroMemory(m_SmallFileBlock.pbBlob, m_SmallFileBlock.dwBlobSize);

        // User cancels the operation.
        if ( HasCancelled() )
        {
            return CRYPT_ERROR_CANCEL;
        }

        // The last block
        if ( dwLeftSize <= dwBufferSize )
        {
            isLastBlock = TRUE;
            dwBufferSize = dwLeftSize;
            DWORD dwMod = dwBufferSize % ENCRYPT_BLOCK_SIZE;
            if ( 0 != dwMod )
            {
                dwBufferSize += ENCRYPT_BLOCK_SIZE - dwMod + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
            }
        }

        // Read the data.
        BOOL isSucceed = ReadFile(
            hFile,
            m_SmallFileBlock.pbBlob,
            dwBufferSize,
            &dwBytesRead,
            NULL);

        if ( !isSucceed )
        {
            return CRYPT_ERROR_READDATA;
        }

        // Encrypt the data
        CRYPT_RESULT nResult = m_pCrypt->EncryptStream(
            m_SmallFileBlock.pbBlob,
            dwBufferSize,
            isLastBlock);

        if ( CRYPT_ERROR_SUCCEED != nResult )
        {
            return CRYPT_ERROR_ENCRYPT;
        }

        // Write the data to destination file.
        isSucceed = WriteFile(
            hOutFile,
            m_SmallFileBlock.pbBlob,
            dwBufferSize,
            &dwBytesWritten,
            NULL);

        if ( !isSucceed )
        {
            return CRYPT_ERROR_WRITEDATA;
        }

        dwLeftSize -= dwBytesRead;

        // update the percent
        lpFileInfos->fPencent = ( (FLOAT)(dwDataLength - dwLeftSize) / (FLOAT)dwDataLength ) * 100;

        SetPercent();
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::DecryptFile(IN LPFILEINFOS lpFileInfos)
{
    CRYPT_RESULT nResult = CRYPT_ERROR_SUCCEED;

    if ( CRYPT_ERROR_SUCCEED != (nResult = OpenFile(lpFileInfos)) )
    {
        return nResult;
    }

    // Get crypt header from encrypted file.
    if ( CRYPT_ERROR_SUCCEED != (nResult = ReadCryptHeader(lpFileInfos)) )
    {
        return nResult;
    }

    // Check the header valid or invalid.
    nResult = CheckCryptHeader(lpFileInfos->sDestFileInfo.destFileHeader);
    if ( CRYPT_ERROR_SUCCEED != nResult )
    {
        return nResult;
    }

    // Get the destination file full path.
    GetDestFilePath(lpFileInfos, FALSE);

    if ( CRYPT_ERROR_SUCCEED != (nResult = CreateFile(lpFileInfos, FALSE)) )
    {
        return nResult;
    }

    if ( !lpFileInfos->isSuccess || !lpFileInfos->sDestFileInfo.isSuccess )
    {
        if ( HasCancelled() || !lpFileInfos->isReplaced )
        {
            return CRYPT_ERROR_CANCEL;
        }

        DeleteFiles(lpFileInfos, FALSE);
        return CRYPT_ERROR_FAIL;
    }

    nResult = lpFileInfos->isBigFile ? DecryptBigFile(lpFileInfos) : DecryptSmallFile(lpFileInfos);;
    if ( CRYPT_ERROR_SUCCEED != nResult )
    {
        DeleteFiles(lpFileInfos, FALSE);
        return nResult;
    }

    // Delete the original file.
    if ( this->m_bDelOriginalFiles )
    {
        DeleteFiles(lpFileInfos);
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::DecryptBigFile(IN LPFILEINFOS lpFileInfos)
{
    if ( (NULL == lpFileInfos) ||
         (!ISVALIDHANDLE(lpFileInfos->hMapFile)) ||
         (!ISVALIDHANDLE(lpFileInfos->sDestFileInfo.hDestMapFile)) )
    {
        return CRYPT_ERROR_INVALID_HANDLE;
    }

    // get the system allocation granularity, this value is 64KB(64 * 1024 = 65536)
    DWORD dwBlockSize = MAX_WRITE_RATE * m_dwAllocationGranularity;

    UINT64 nDestFileOffset  = 0;
    UINT64 nSrcFileOffset = lpFileInfos->sDestFileInfo.destFileHeader.nDataBegin;
    UINT64 nSrcFileLength = lpFileInfos->sDestFileInfo.destFileHeader.nDataLength;
    UINT64 nFileLeftSize  = nSrcFileLength;         // the left bytes which not to be map
    DWORD  dwBytesToMap   = dwBlockSize;            // the bytes to map
    BOOL   isLastBlock    = FALSE;                  // last block or not
    CRYPT_RESULT nResult  = CRYPT_ERROR_SUCCEED;               // The crypt error code.

    while ( nFileLeftSize > 0 )
    {
        // User cancels the operation
        if ( HasCancelled() )
        {
            return CRYPT_ERROR_CANCEL;
        }

        if ( nFileLeftSize <= dwBlockSize )
        {
            dwBytesToMap = (DWORD)nFileLeftSize;
            isLastBlock = TRUE;

            dwBlockSize = dwBytesToMap;
            DWORD dwMod = dwBytesToMap % ENCRYPT_BLOCK_SIZE;
            if ( 0 != dwMod )
            {
                dwBlockSize += ENCRYPT_BLOCK_SIZE - dwMod + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
            }
        }

        PBYTE pSrcMapBuffer = (PBYTE)MapViewOfFile(
            lpFileInfos->hMapFile,
            FILE_MAP_READ,
            (DWORD)(nSrcFileOffset >> 32),
            (DWORD)(nSrcFileOffset & 0xFFFFFFFF),
            dwBlockSize );

        if ( NULL == pSrcMapBuffer )
        {
            return CRYPT_ERROR_INVALID_HANDLE;
        }

        // the map of the destination file
        PBYTE pDestMapBuffer = (PBYTE)MapViewOfFile(
            lpFileInfos->sDestFileInfo.hDestMapFile,
            FILE_MAP_READ | FILE_MAP_WRITE,
            (DWORD)(nDestFileOffset >> 32),
            (DWORD)(nDestFileOffset & 0xFFFFFFFF),
            dwBytesToMap);

        if ( NULL == pDestMapBuffer )
        {
            UnmapViewOfFile(pSrcMapBuffer);
            return CRYPT_ERROR_INVALID_HANDLE;
        }

        // The last block.
        if ( isLastBlock )
        {
            // allocate the new buffer, which size is dwBlockSize + 1. the last byte is 0
            PBYTE pBuffer = new BYTE[dwBlockSize + 1];
            ZeroMemory(pBuffer, dwBlockSize + 1);
            memmove(pBuffer, pSrcMapBuffer, dwBlockSize);

            // Decrypt the buffer. the size of the buffer must be dwBlockSize,
            // because encryption the
            // last block size is also dwBlockSize. but the m_pDestBuffer size
            // is dwBytesToMap, which
            // is below dwBlockSize, so I copy the m_pSrcBuffer to a new buffer,
            // which allocated memory
            // dwBlockSize + 1. then decrypt the new buffer, and copy the new 
            // buffer to m_pDestBuffer,
            // the destination size is must be dwBytesToMap,
            // because the map size of m_pDestBuffer is dwBytesToMap.
            nResult = m_pCrypt->DecryptStream(pBuffer, dwBlockSize, isLastBlock);
            if ( CRYPT_ERROR_SUCCEED != nResult )
            {
                SAFE_DELETE(pBuffer);
                UnmapViewOfFile(pDestMapBuffer);
                UnmapViewOfFile(pSrcMapBuffer);
                return CRYPT_ERROR_DECRYPT;
            }

            memmove(pDestMapBuffer, pBuffer, dwBytesToMap);

            // Delete the buffer memory.
            SAFE_DELETE(pBuffer);
        }
        else
        {
            //memcpy_s(m_pDestBuffer, dwBlockSize, m_pSrcBuffer, dwBytesToMap);
            memmove(pDestMapBuffer, pSrcMapBuffer, dwBytesToMap);

            // Decrypt the data buffer
            nResult = m_pCrypt->DecryptStream(pDestMapBuffer, dwBlockSize, isLastBlock);
            if ( CRYPT_ERROR_SUCCEED != nResult )
            {
                UnmapViewOfFile(pDestMapBuffer);
                UnmapViewOfFile(pSrcMapBuffer);
                return CRYPT_ERROR_DECRYPT;
            }
        }

        nSrcFileOffset  += dwBlockSize;
        nDestFileOffset += dwBytesToMap;
        nFileLeftSize   -= dwBytesToMap;

        // update the percent
        lpFileInfos->fPencent = ( (FLOAT)nSrcFileOffset / (FLOAT)nSrcFileLength ) * 100;
        SetPercent();

        UnmapViewOfFile(pDestMapBuffer);
        UnmapViewOfFile(pSrcMapBuffer);
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::DecryptSmallFile(IN LPFILEINFOS lpFileInfos)
{
    if ( (NULL == lpFileInfos) ||
         (!ISVALIDHANDLE(lpFileInfos->hFile)) ||
         (!ISVALIDHANDLE(lpFileInfos->sDestFileInfo.hDestFile)) )
    {
        return CRYPT_ERROR_INVALID_HANDLE;
    }

    HANDLE hFile       = lpFileInfos->hFile;
    HANDLE hOutFile    = lpFileInfos->sDestFileInfo.hDestFile;
    LONG  dwOffsetLow  = (LONG)(lpFileInfos->sDestFileInfo.destFileHeader.nDataBegin & 0xFFFFFFFF);
    LONG  dwOffsetHigh = (LONG)(lpFileInfos->sDestFileInfo.destFileHeader.nDataBegin >> 32);
    DWORD dwDataLength = (DWORD)lpFileInfos->sDestFileInfo.destFileHeader.nDataLength;

    SetFilePointer(hFile, dwOffsetLow, &dwOffsetHigh, FILE_BEGIN);
    SetFilePointer(hOutFile, 0, NULL, FILE_BEGIN);

    DWORD dwBufferSize = (MAX_BUFFER_SIZE >= dwDataLength) ? dwDataLength : MAX_BUFFER_SIZE;

    if ( NULL == m_SmallFileBlock.pbBlob )
    {
        return CRYPT_ERROR_FAIL;
    }

    DWORD dwLeftSize = dwDataLength;
    DWORD dwBytesToWrite = dwBufferSize;
    DWORD dwBytesRead = 0;
    DWORD dwBytesWritten = 0;
    BOOL  isLastBlock = FALSE;
    BOOL  isSucceed = FALSE;
    CRYPT_RESULT nResult = CRYPT_ERROR_SUCCEED;

    // when file size is 0 bytes, decrypt the additional bytes, and the block is last one
    if ( 0 == dwLeftSize )
    {
        dwLeftSize = ENCRYPT_BLOCK_SIZE + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
        dwBufferSize = dwLeftSize;

        isSucceed = ReadFile(
            hFile,
            m_SmallFileBlock.pbBlob,
            dwBufferSize,
            &dwBytesRead,
            NULL);

        if ( !isSucceed )
        {
            return CRYPT_ERROR_READDATA;
        }

        nResult = m_pCrypt->DecryptStream(m_SmallFileBlock.pbBlob, dwBufferSize, TRUE);
        if ( CRYPT_ERROR_SUCCEED != nResult )
        {
            return CRYPT_ERROR_DECRYPT;
        }

        lpFileInfos->fPencent = 100;
        SetPercent();

        return CRYPT_ERROR_SUCCEED;
    }

    while ( dwLeftSize > 0 )
    {
        ZeroMemory(m_SmallFileBlock.pbBlob, m_SmallFileBlock.dwBlobSize);

        if ( HasCancelled() )
        {
            return CRYPT_ERROR_CANCEL;
        }

        if ( dwLeftSize <= dwBufferSize )
        {
            isLastBlock = TRUE;
            dwBytesToWrite = dwLeftSize;
            dwBufferSize = dwLeftSize;
            DWORD dwMod = dwBufferSize % ENCRYPT_BLOCK_SIZE;
            if ( 0 != dwMod )
            {
                dwBufferSize += ENCRYPT_BLOCK_SIZE - dwMod + ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
            }
        }

        // Read data from file, the data is encrypted
        isSucceed = ReadFile(
            hFile,
            m_SmallFileBlock.pbBlob,
            dwBufferSize,
            &dwBytesRead,
            NULL);

        if ( !isSucceed )
        {
            return CRYPT_ERROR_READDATA;
        }

        // Decrypt the data.
        nResult = m_pCrypt->DecryptStream(
            m_SmallFileBlock.pbBlob,
            dwBufferSize,
            isLastBlock);

        if ( CRYPT_ERROR_SUCCEED != nResult )
        {
            return CRYPT_ERROR_DECRYPT;
        }

        // Write the data to destination file, the data has been decrypted.
        isSucceed = WriteFile(
            hOutFile,
            m_SmallFileBlock.pbBlob, 
            dwBytesToWrite,
            &dwBytesWritten,
            NULL);

        if ( !isSucceed )
        {
            return CRYPT_ERROR_WRITEDATA;
        }

        dwLeftSize -= dwBytesWritten;

        lpFileInfos->fPencent = ( (FLOAT)(dwDataLength - dwLeftSize) / (FLOAT)dwDataLength ) * 100;
        SetPercent();
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::OpenFile(IN OUT LPFILEINFOS lpFileInfos)
{
    if ( NULL == lpFileInfos )
    {
        return CRYPT_ERROR_FAIL;
    }

    HANDLE hFile = INVALID_HANDLE_VALUE;
    DWORD dwFileSizeLow  = 0;
    DWORD dwFileSizeHigh = 0;
    CryptString strFileExt;

    hFile = ::CreateFile(
        lpFileInfos->szFilePath,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if ( !ISVALIDHANDLE(hFile) )
    {
        return (CRYPT_RESULT)GetLastError();
    }

    lpFileInfos->isSuccess = TRUE;
    lpFileInfos->hFile = hFile;
    lpFileInfos->dwFlagsAndAttributes = ::GetFileAttributes( lpFileInfos->szFilePath );
    lpFileInfos->sDestFileInfo.destFileHeader.dwFlagsAndAttributes =
        lpFileInfos->dwFlagsAndAttributes;

    dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);
    lpFileInfos->nFileSize = dwFileSizeHigh;
    lpFileInfos->nFileSize <<= 32;
    lpFileInfos->nFileSize |= dwFileSizeLow;

    // Big file
    if ( lpFileInfos->nFileSize >= CRITICAL_VALUE )
    {
        lpFileInfos->isBigFile = TRUE;
        lpFileInfos->hMapFile = CreateFileMapping(hFile, NULL ,PAGE_READONLY, 0, 0, NULL);
        lpFileInfos->isSuccess = (NULL == lpFileInfos->hMapFile) ? FALSE : TRUE;

        if ( !lpFileInfos->isSuccess )
        {
            return (CRYPT_RESULT)GetLastError();
        }
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

CRYPT_RESULT SdkCryptFile::CreateFile(IN OUT LPFILEINFOS lpDestFileInfos, IN BOOL isEncrypt)
{
    if (NULL == lpDestFileInfos)
    {
        return CRYPT_ERROR_FAIL;
    }

    HANDLE hOutFile = INVALID_HANDLE_VALUE;
    DWORD dwAttributes = 0;
    if ( isEncrypt )
    {
        dwAttributes = FILE_ATTRIBUTE_READONLY;
    }
    else
    {
        dwAttributes = lpDestFileInfos->sDestFileInfo.destFileHeader.dwFlagsAndAttributes;
    }

    // Replace the destination file or not
    if ( !ReplaceDestFile(lpDestFileInfos, isEncrypt) )
    {
        return CRYPT_ERROR_SUCCEED;
    }

    hOutFile = ::CreateFile(
        lpDestFileInfos->sDestFileInfo.szDestFilePath,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        dwAttributes,
        NULL);

    if ( !ISVALIDHANDLE(hOutFile) )
    {
        return (CRYPT_RESULT)GetLastError();
    }

    lpDestFileInfos->sDestFileInfo.hDestFile = hOutFile;
    lpDestFileInfos->sDestFileInfo.isSuccess = TRUE;
    lpDestFileInfos->sDestFileInfo.dwFlagsAndAttributes = dwAttributes;

    // Create the file mapping.
    if ( lpDestFileInfos->isBigFile )
    {
        HANDLE hOutMapFile = NULL;
        LARGE_INTEGER liDistanceToMove;

        if ( isEncrypt )
        {
            liDistanceToMove.QuadPart = 
                lpDestFileInfos->nFileSize + m_dwAllocationGranularity;
            DWORD dwMod = (DWORD)(lpDestFileInfos->nFileSize % ENCRYPT_BLOCK_SIZE);
            if ( 0 != dwMod )
            {
                liDistanceToMove.QuadPart += ENCRYPT_BLOCK_SIZE - dwMod;
            }
            liDistanceToMove.QuadPart += ENCRYPT_LAST_PART_VERIFYDATA_SIZE;
        }
        else
        {
            liDistanceToMove.QuadPart =
                lpDestFileInfos->sDestFileInfo.destFileHeader.nDataLength;
        }

        SetFilePointerEx(hOutFile, liDistanceToMove, NULL, FILE_BEGIN);
        SetEndOfFile(hOutFile);

        hOutMapFile = CreateFileMapping(hOutFile,
            NULL,
            PAGE_READWRITE,
            DWORD(liDistanceToMove.QuadPart >> 32),
            DWORD(liDistanceToMove.QuadPart & 0xFFFFFFFF),
            NULL);

        if ( NULL == hOutMapFile )
        {
            return (CRYPT_RESULT)GetLastError();
        }

        lpDestFileInfos->sDestFileInfo.hDestMapFile = hOutMapFile;
    }

    return CRYPT_ERROR_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::GetDestFilePath(IN OUT LPFILEINFOS lpDestFileInfo, IN BOOL isEncrypt)
{
    if ( (NULL == lpDestFileInfo) || (lpDestFileInfo->isEncrypted && isEncrypt) )
    {
        return;
    }

    // The path is for encrypting
    if ( isEncrypt )
    {
        CryptString strFilePath(lpDestFileInfo->sDestFileInfo.szDestFilePath);
        strFilePath.append(ENCRYPTED_FILE_EXTENDNAME);
        _tcscpy_s(
            lpDestFileInfo->sDestFileInfo.szDestFilePath,
            MAX_PATH,
            strFilePath.c_str());
    }
    else
    {
        ::PathRemoveFileSpec(lpDestFileInfo->sDestFileInfo.szDestFilePath);
        ::PathCombine(
            lpDestFileInfo->sDestFileInfo.szDestFilePath,
            lpDestFileInfo->sDestFileInfo.szDestFilePath,
            (TCHAR*)lpDestFileInfo->sDestFileInfo.destFileHeader.cbFileName);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::DeleteFiles(IN LPFILEINFOS lpFileInfos, IN BOOL isDelSrcOrDestFile)
{
    if ( NULL == lpFileInfos )
    {
        return;
    }

    // Before delete operation, free the handles
    CloseHandles(lpFileInfos);

    DWORD dwFlagAttributes = 0;
    if ( isDelSrcOrDestFile )
    {
        dwFlagAttributes = lpFileInfos->dwFlagsAndAttributes;
    }
    else
    {
        dwFlagAttributes = lpFileInfos->sDestFileInfo.dwFlagsAndAttributes;
    }

    CryptString strFilePath;
    if ( isDelSrcOrDestFile )
    {
        strFilePath = lpFileInfos->szFilePath;
    }
    else
    {
        strFilePath = lpFileInfos->sDestFileInfo.szDestFilePath;
    }

    // The file is read only
    if ( FILE_ATTRIBUTE_READONLY == (FILE_ATTRIBUTE_READONLY & dwFlagAttributes) )
    {
        // Remove the FILE_ATTRIBUTE_READONLY attributes.
        SetFileAttributes(strFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);
    }

    DeleteFile(strFilePath.c_str());
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::CloseHandles(IN LPFILEINFOS lpFileInfos)
{
    if ( NULL == lpFileInfos )
    {
        return;
    }

    // close the source file handles
    SAFE_CLOSE_HANDLE(lpFileInfos->hMapFile);
    SAFE_CLOSE_HANDLE(lpFileInfos->hFile);

    // close the destination file handles
    SAFE_CLOSE_HANDLE(lpFileInfos->sDestFileInfo.hDestMapFile);
    SAFE_CLOSE_HANDLE(lpFileInfos->sDestFileInfo.hDestFile);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptFile::ReplaceDestFile(IN OUT LPFILEINFOS lpFileInfo, IN BOOL isEncrypt)
{
    UNREFERENCED_PARAMETER(isEncrypt);

    if ( NULL == lpFileInfo )
    {
        return FALSE;
    }

    if (::PathFileExists(lpFileInfo->sDestFileInfo.szDestFilePath))
    {
        CRYPT_INQUIRE_RESULT nResult = CRYPT_INQUIRE_YES;
        OnReplaceFiles(&nResult, lpFileInfo->sDestFileInfo.szDestFilePath);
        lpFileInfo->isReplaced = (CRYPT_INQUIRE_YES == nResult);

        if (HasCancelled())
        {
            return FALSE;
        }
    }

    return lpFileInfo->isReplaced;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptFile::HasCancelled()
{
    if (NULL != m_pProgressDialog)
    {
        return m_pProgressDialog->HasCancelled();
    }

    return m_hasCancelCrypt;
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::SetPercent()
{
    FLOAT fTotalPercent = 0;
    FLOAT fProgress = 0;
    int nSize = (int)m_vFileInfoList.size();
    for (int i = 0; i < nSize; ++i)
    {
        fTotalPercent += m_vFileInfoList[i]->fPencent;
    }

    if ( 0 != m_nFileNumbers )
    {
        fProgress = (fTotalPercent / m_nFileNumbers);
    }

    OnProgressChanged((DWORD)fProgress, 100);
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCryptFile::GetFileName(IN const CryptString& filePath)
{
    return CryptString(PathFindFileName(filePath.c_str()));
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCryptFile::GetErrorCodeString(DWORD dwErrorCode)
{
    static WCHAR ch[20] = { 0 };
    ZeroMemory(ch, 20 * sizeof(WCHAR));
    wsprintf(ch, L"Error code: %d", dwErrorCode);
    return CryptString(ch);
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCryptFile::FormatErrorMessage(IN CRYPT_RESULT nResultCode, IN const CryptString& strFilePath)
{
    CryptString strMsg(strFilePath);
    if (0 != strMsg.length())
    {
        strMsg.append(L": ");
    }

    switch( nResultCode )
    {
    case CRYPT_ERROR_SUCCEED:
        strMsg = LoadResString(IDS_CRYPT_SUCCESS);
        break;

    case CRYPT_ERROR_FAIL:
        strMsg.append(LoadResString(IDS_CRYPT_FAILED));
        break;

    case CRYPT_ERROR_INVALID_KEY:
        strMsg.append(LoadResString(IDS_CRYPT_INVALID_KEY));
        break;

    case CRYPT_ERROR_INVALIDFILE:
        strMsg.append(LoadResString(IDS_CRYPT_INVALID_FILE));
        break;

    case CRYPT_ERROR_ENCRYPT:
        strMsg.append(LoadResString(IDS_CRYPT_ENCRYPT_ERROR));
        break;

    case CRYPT_ERROR_DECRYPT:
        strMsg.append(LoadResString(IDS_CRYPT_DECRYPT_ERROR));
        break;

    case CRYPT_ERROR_CANCEL:
        strMsg.append(LoadResString(IDS_CRYPT_CANCEL_ERROR));
        break;

    case CRYPT_ERROR_OTHER:
        strMsg.append(LoadResString(IDS_CRYPT_OTHER_ERROR));
        break;

    case CRYPT_ERROR_ACCESS_DENIED:
        strMsg.append(LoadResString(IDS_CRYPT_ACCESS_DENIED));
        break;

    case CRYPT_ERROR_WRITE_PROTECT:
        strMsg.append(LoadResString(IDS_CRYPT_WRITE_PROTECT));
        break;

    case CRYPT_ERROR_DISK_FULL:
        strMsg.append(LoadResString(IDS_CRYPT_DISK_FULL));
        break;

    case CRYPT_ERROR_SHARING_VIOLATION:
        strMsg.append(LoadResString(IDS_CRYPT_SHARING_VIOLATION));
        break;

    case CRYPT_ERROR_PATH_NOT_FOUND:
        strMsg.append(LoadResString(IDS_CRYPT_PATH_NOT_FOUND));
        break;

    case CRYPT_ERROR_FILE_NOT_FOUND:
        strMsg.append(LoadResString(IDS_FILE_NOT_FOUND));
        break;

    case CRYPT_ERROR_OPENFILE:
        strMsg.append(LoadResString(IDS_CRYPT_NOFILES));
        break;

    case CRYPT_ERROR_NOFILES:
        strMsg = LoadResString(IDS_CRYPT_NOFILES);
        break;

    case CRYPT_ERROR_INVALIDPARAM:
        strMsg.append(LoadResString(IDS_CRYPT_IVALIDPARAM));
        break;

    case CRYPT_ERROR_INVALIDPWD:
        strMsg.append(LoadResString(IDS_CRYPT_IVALIDPWD));
        break;

    default:
        strMsg.append(LoadResString(IDS_CRYPT_OTHER_ERROR));
        break;
    }

    return strMsg;
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCryptFile::LoadResString(UINT32 uResId)
{
    static TCHAR szBuffer[1024] = { 0 };
    ZeroMemory(szBuffer, 1024 * sizeof(TCHAR));

    UINT nLength = ::LoadStringW(HINST_THISCOMPONENT, uResId, szBuffer, 1024);
    if(nLength <= 0)
    {
        ZeroMemory(szBuffer, 1024 * sizeof(TCHAR));
    }

    return szBuffer;
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::OnCryptBegin(IN CRYPT_OP_TYPE operationType)
{
    if (NULL != m_pCryptFileSink)
    {
        m_pCryptFileSink->OnCryptBegin();
    }

    // Show the default progress dialog.
    if (NULL != m_pProgressDialog)
    {
        m_pProgressDialog->Start();

        // Set the title of the dialog.
        switch (operationType)
        {
        case CRYPT_OP_ENCRYPT:
            m_pProgressDialog->SetTitle(LoadResString(IDS_ENCRYPT_PROGRESS_DLG_TITLE));
            break;

        case CRYPT_OP_DECRYPT:
            m_pProgressDialog->SetTitle(LoadResString(IDS_DECRYPT_PROGRESS_DLG_TITLE));
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::OnCryptFinish()
{
    if (NULL != m_pCryptFileSink)
    {
        m_pCryptFileSink->OnCryptFinish();
    }

    if (NULL != m_pProgressDialog)
    {
        m_pProgressDialog->Stop();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::OnCryptNext(IN DWORD dwCurIndex, IN LPCTSTR lpCurFilePath)
{
    if (NULL != m_pCryptFileSink)
    {
        m_pCryptFileSink->OnCryptNext(dwCurIndex, lpCurFilePath);
    }

    // Change the text on the dialog.
    if (NULL != m_pProgressDialog)
    {
        static CryptString strText = LoadResString(IDS_DECRYPT_PROGRESS_DLG_LINE2);
        static TCHAR szBuf[50] = { 0 };
        szBuf[0] = 0;
        _stprintf(
            szBuf,
            50,
            _T("%s  %d / %d"),
            strText.c_str(),
            (dwCurIndex + 1),
            m_nFileNumbers);

        m_pProgressDialog->SetLines(1, lpCurFilePath);
        m_pProgressDialog->SetLines(2, szBuf);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::OnCryptError(IN LPCTSTR lpErrorMsg)
{
    // Output the error message.
    SdkCommonHelper::PrintDebugString(L"Crypt error: %s\n", lpErrorMsg);

    if (NULL != m_pCryptFileSink)
    {
        m_pCryptFileSink->OnCryptError(lpErrorMsg);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::OnProgressChanged(IN DWORD dwCompleted, IN DWORD dwTotal)
{
    if (NULL != m_pCryptFileSink)
    {
        m_pCryptFileSink->OnProgressChanged(dwCompleted, dwTotal);
    }

    // Update the progress.
    if (NULL != m_pProgressDialog)
    {
        m_pProgressDialog->SetProgress(dwCompleted, dwTotal);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptFile::OnReplaceFiles(OUT CRYPT_INQUIRE_RESULT *pResult, IN LPCTSTR lpFileName)
{
    if (NULL != m_pCryptFileSink)
    {
        m_pCryptFileSink->OnReplaceFiles(pResult, lpFileName);
    }
}
