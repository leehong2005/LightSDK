/*!
* @file SdkCryptFile.h
* 
* @brief This file defines SdkCryptFile class to encrypt, decrypt files.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/22
*/

#ifdef __cplusplus
#ifndef TFPUCRYPTFILE__H
#define TFPUCRYPTFILE__H

#include "SdkCryptDef.h"
#include "SdkCrypt.h"
#include "SdkCryptKey.h"
#include "SdkProgressDialog.h"
#include "ICryptFileNotify.h"

USING_NAMESPACE_UTILITIES

BEGIN_NAMESPACE_COMMON

typedef struct _CRYPTHEADER  CRYPTHEADER,  *LPCRYPTHEADER;
typedef struct _DESTFILEINFO DESTFILEINFO, *LPDESTFILEINFO;
typedef struct _FILEINFOS    FILEINFOS,    *LPFILEINFOS;

/*!
* @brief This class can encrypt and decrypt and preview files
*/
class CLASS_DECLSPEC SdkCryptFile
{
public:

    /*!
    * @brief The function encrypt file name for a key file. The file name will marked by base64.
    *
    * @param strFileName    [I/ ] The input file name.
    *
    * @return The encrypted file name.
    */
    static CryptString EncryptFileName(IN const CryptString& strFileName);

    /*!
    * @brief The default constructor function.
    *
    * @param isDefProgressBar   [I/ ] Show default progress bar.
    *
    * @remark This constructor function is used for previewing file,
    *         if you use the instance which constructed
    *         by this function to encrypt or decrypt files, the result is failure.
    *         if you want to encrypt or decrypt files,
    *         please use another overload constructor function to construct instance.
    */
    SdkCryptFile(BOOL isDefProgressBar = TRUE);

    /*!
    * @brief The default destructor.
    */
    ~SdkCryptFile();

    /*!
    * @brief The function can encrypt and decrypt files.
    *
    * @param operationType  [I/ ] The operation type, you must give CRYPT_OP_ENCRYPT or CRYPT_OP_DECRYPT,
    *                             or the function will return CRYPT_OTHER_ERRRO.
    *
    * @return CRYPT_ERROR_SUCCEED is success, other values are failure
    *
    * @remark The public interface for decryption and encryption, give different operation type, this
    *         function will operate different function.
    */
    CRYPT_RESULT BeginCrypt(IN CRYPT_OP_TYPE operationType);

    /*!
    * @brief Set the cryptographic password.
    *
    * @param lpPassword     [I/ ] The password of cryptographic files.
    *
    * @return CRYPT_ERROR_SUCCEED is success, other values are failure.
    */
    CRYPT_RESULT SetCryptPassword(IN LPCTSTR lpPassword);

    /*!
    * @brief Set the cryptographic key path.
    *
    * @param lpKeyPath      [I/ ] The path of cryptographic key.
    *
    * @return CRYPT_ERROR_SUCCEED is success, other values are failure.
    */
    CRYPT_RESULT SetCryptKeyPath(IN LPCTSTR lpKeyPath);

    /*!
    * @brief Set the file path list.
    * 
    * @param vctFilePathList    [I/ ] The reference of the vector, this parameter
    *                                 determines which files will be encrypted or decrypted.
    * @param isDelOriginalFile  [I/ ] This parameter determines delete original files or not,
    *                                 TRUE is to delete, FALSE is not to delete, default value is TRUE
    *
    * @remark The overload constructor function, which used to construct instance to encrypt or decrypt file, 
    *         you also can use this constructor function to construct instance to preview file,
    *         but the first parameter cannot be omit.
    */
    CRYPT_RESULT SetCryptFiles(IN const vector<CryptString>& vctFilePathList, IN BOOL isDelOriginalFile = TRUE);

    /*!
    * @brief Set the crypt sink interface which can notify client events.
    *
    * @param pCryptFileSink     [I/ ] The pointer to ICryptFileNotify.
    *
    * @return CRYPT_ERROR_SUCCEED is success, other values are failure.
    */
    CRYPT_RESULT SetCryptFileSink(IN ICryptFileNotify *pCryptFileSink);

    /*!
    * @brief Cancel the crypt operation.
    *
    * @return CRYPT_ERROR_SUCCEED is success, other values are failure.
    */
    CRYPT_RESULT CancelCrypt();

    /*!
    * @brief Get the error string according to the result code.
    *
    * @param nResultCode    [I/ ] The result code.
    *
    * @return The error string.
    */
    CryptString GetErrorString(CRYPT_RESULT nResultCode);

private:

    /*!
    * @brief Initialize the data for encrypting or decrypting
    *
    * @param operationType  [I/ ] The operation type, you must give CRYPT_OP_ENCRYPT or CRYPT_OP_DECRYPT.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error.
    */
    CRYPT_RESULT InitCryptData(IN CRYPT_OP_TYPE operationType);

    /*!
    * @brief Initialize the scene for encrypting or decrypting
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error.
    */
    CRYPT_RESULT InitCryptScene();

    /*!
    * @brief Initialize the header of destination file.
    *
    * @param lpDestFileInfos  [ /O] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error
    */
    CRYPT_RESULT InitCryptHeader(OUT LPFILEINFOS lpDestFileInfos);

    /*!
    * @brief Get the header from encrypted file, this function is only for decryption
    *
    * @param lpFileInfos    [ /O] The reference of FILEINFOS, which determines which
    *                             file`s header will be initialized.
    *
    * @return CRYPT_ERROR_SUCCEED is success, others are failure
    */
    CRYPT_RESULT ReadCryptHeader(OUT LPFILEINFOS lpFileInfos);

    /*!
    * @brief Write the crypt file header to the destination file, this function is used
    *        for encryption
    *
    * @param lpDestFileInfos  [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error
    */
    CRYPT_RESULT WriteCryptHeader(IN LPFILEINFOS lpDestFileInfos);

    /*!
    * @brief Check the header is valid or invalid
    *
    * @param cryptHeader    [I/ ] The reference of FILEINFOS, which determines which header will be get
    *
    * @return CRYPT_ERROR_SUCCEED is valid, others are invalid.
    */
    CRYPT_RESULT CheckCryptHeader(IN const CRYPTHEADER& cryptHeader);

    /*!
    * @brief Encrypt a file
    *
    * @param lpFileInfos      [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error
    */
    CRYPT_RESULT EncryptFile(IN LPFILEINFOS lpFileInfos);

    /*!
    * @brief The file is small, which size is below CRITICAL_VALUE(10MB), in this function,
                 will use normal I/O operator.
    *
    * @param lpFileInfos      [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error
    */
    CRYPT_RESULT EncryptSmallFile(IN LPFILEINFOS lpFileInfos);

    /*!
    * @brief This file is big one, which size is bigger then CRITICAL_VALUE(10MB),
    *        in this function, will use the file mapping to operate the file,
    *        in order to improve the time of operation to files.
    *
    * @param lpFileInfos      [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error
    */
    CRYPT_RESULT EncryptBigFile(IN LPFILEINFOS lpFileInfos);

    /*!
    * @brief Decrypt a file
    *
    * @param lpFileInfos      [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error
    */
    CRYPT_RESULT DecryptFile(IN LPFILEINFOS fileInfos);

    /*!
    * @brief Decrypt a small file
    *
    * @param lpFileInfos      [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error.
    */
    CRYPT_RESULT DecryptSmallFile(IN LPFILEINFOS fileInfos);

    /*!
    * @brief Decrypt a big file
    *
    * @param lpFileInfos      [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error.
    */
    CRYPT_RESULT DecryptBigFile(IN LPFILEINFOS lpFileInfos);

    /*!
    * @brief Open a file and fill the FILEINFOS data structure.
    *
    * @param lpFileInfos      [I/ ] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    *
    * @return The operation result. CRYPT_ERROR_SUCCEED is OK, others are error.
    */
    CRYPT_RESULT OpenFile(IN OUT LPFILEINFOS lpFileInfos);

    /*!
    * @brief Create destination encrypted files and get its handle.
    *
    * @param lpDestFileInfos  [I/O] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    * @param isEncrypt        [I/ ] This parameter determines the function create file
    *                               for encrypting or decrypting.
    *
    * @return CRYPT_ERROR_SUCCEED is success, other is failure.
    */
    CRYPT_RESULT CreateFile(IN OUT LPFILEINFOS lpDestFileInfos, IN BOOL isEncrypt = TRUE);

    /*!
    * @brief Get the destination file path
    *
    * @param lpDestFileInfos  [I/O] The reference of FILEINFOS, which determines which
    *                               file`s header will be initialized.
    * @param isEncrypt        [I/ ] This parameter determines the function create file
    *                               for encrypting or decrypting.
    */
    void GetDestFilePath(IN OUT LPFILEINFOS lpDestFileInfo, IN BOOL isEncrypt = TRUE);

    /*!
    * @brief Delete files.
    * 
    * @param fileInfos          [I/ ] The reference of the FILEINFOS, which determines which file will be delete.
    * @param isDelSrcOrDestFile [I/ ] This parameter determines delete original file 
    *                                 or destination file. TRUE is deleting original file,
    *                                 FALSE is deleting destination file, default is TRUE.
    */
    void DeleteFiles(IN LPFILEINFOS lpFileInfos, IN BOOL isDelSrcOrDestFile = TRUE);

    /*!
    * @brief Free all handles of one file.
    * 
    * @param lpFileInfos        [I/ ] The reference of the FILEINFOS, which determines which file will be freed.
    */
    void CloseHandles( IN LPFILEINFOS lpFileInfos );

    /*!
    * @brief Replace the destination file.
    *
    * @param lpFileInfo [I/ ] The file information.
    * @param isEncrypt  [I/ ] The operation is encryption or not
    *
    * @return TRUE replace the destination file, FALSE does not replace destination file.
    */
    BOOL ReplaceDestFile(IN OUT LPFILEINFOS lpFileInfo, IN BOOL isEncrypt = FALSE);

    /*!
    * @brief Indicates whether cancel crypt operation.
    *
    * @return TRUE if cancel, otherwise FALSE.
    */
    BOOL HasCancelled();

    /*!
    * @brief Set the percent of all files
    */
    void SetPercent();

    /*!
    * @brief get the destination file path.
    *
    * @param strFilePath        [I/ ] The reference of the file full path. this path must be full path of a file.
    *
    * @return The file name.
    */
    CryptString GetFileName(IN const CryptString& strFilePath);

    /*!
    * @brief Convert error code to string..
    *
    * @param dwErrorCode    [I/ ] The error code, which determines what message will be displayed.
    *
    * @return The error code string
    */
    CryptString GetErrorCodeString(DWORD dwErrorCode);

    /*!
    * @brief Show the message box.
    *
    * @param nResultCode    [I/ ] The error code, which determines what message will be displayed.
    * @param strFilePath    [I/ ] The file path.
    *
    * @return The error message string.
    */
    CryptString FormatErrorMessage(IN CRYPT_RESULT nResultCode, IN const CryptString& strFilePath);

    /*!
    * @brief Load the string from specified instance.
    *
    * @param uResId     [I/ ] The resource id.
    *
    * @return The string.
    */
    CryptString LoadResString(UINT32 uResId);

    /*!
    * @brief Called when crypt operation begins.
    */
    void OnCryptBegin(IN CRYPT_OP_TYPE operationType);

    /*!
    * @brief Called when crypt operation finishes.
    */
    void OnCryptFinish();

    /*!
    * @brief Called process next file.
    *
    * @dwCurIndex       [I/ ] The current index of files.
    * @lpCurFilePath    [I/ ] The path of the file.
    */
    void OnCryptNext(IN DWORD dwCurIndex, IN LPCTSTR lpCurFilePath);

    /*!
    * @brief Called when an error occurs.
    *
    * @lpErrorMsg       [I/ ] The error message of operation.
    */
    void OnCryptError(IN LPCTSTR lpErrorMsg);

    /*!
    * @brief Called when should update the process of operation.
    *
    * @param dwCompleted [I/ ] The completed number.
    * @param dwTotal     [I/ ] The total number.
    */
    void OnProgressChanged(IN DWORD dwCompleted, IN DWORD dwTotal);

    /*!
    * @brief Called when inquire user to replace existed files or not.
    *
    * @param pResult     [ /O] One value of CRYPT_INQUIRE_RESULT.
    * @param lpFilePath  [I/ ] Current conflicted file name.
    */
    void OnReplaceFiles(OUT CRYPT_INQUIRE_RESULT *pResult, IN LPCTSTR lpFileName);

private:

    BOOL                    m_bDelOriginalFiles;        // Delete original files.
    BOOL                    m_hasCancelCrypt;           // Cancel crypt operation or not.
    UINT32                  m_nFileNumbers;             // The numbers of files to be encrypted.
    UINT32                  m_nFileIndex;               // The index of already disposed files.
    DWORD                   m_dwAllocationGranularity;  // The system allocation granularity.
    CryptString             m_strPassword;              // The cryptographic password.
    CryptString             m_strUserName;              // The current username.
    CryptString             m_strDomainName;            // The current username domain name.
    CryptString             m_strDomainUserName;        // The current username and the domain name.
    CryptString             m_strCryptKeyPath;          // The path of crypt key.
    vector<LPFILEINFOS>     m_vFileInfoList;            // The file information list.
    vector<CryptString>     m_vFilePathList;            // The file full path list.
    CRYPTMEMBLOB            m_HeaderBlock;              // Header memory buffer.
    CRYPTMEMBLOB            m_SmallFileBlock;           // Small file memory buffer.
    SdkCrypt               *m_pCrypt;                   // Pointer to the object of TFPUCrypt.
    ICryptFileNotify       *m_pCryptFileSink;           // The crypt sink.
    SdkProgressDialog      *m_pProgressDialog;          // The progress dialog.
};

END_NAMESPACE_COMMON

#endif // TFPUCRYPTFILE__H
#endif // __cplusplus
