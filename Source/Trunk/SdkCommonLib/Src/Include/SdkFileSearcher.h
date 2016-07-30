/*!
* @file SdkFileSearcher.h
* 
* @brief This file defines class SdkFileSearcher using to search file from disk.
*        The file's kind map is stored in register key, the path is 
*        HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Explorer\KindMap.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#ifdef __cplusplus
#ifndef _SDKFILESEARCHER_H_
#define _SDKFILESEARCHER_H_

#include <process.h>
#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "SdkQueryCondition.h"
#include "SdkQueryScope.h"
#include "IFileSearcherNotify.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The SdkFileSearcher class is used to search files from disk.
*/
class CLASS_DECLSPEC SdkFileSearcher
{
public:

    /*!
    * @brief The constructor functions.
    */
    SdkFileSearcher();

    /*!
    * @brief The destructor functions.
    */
    ~SdkFileSearcher();

    /*!
    * @brief Set the search condition, it should be called before calling to Search() function.
    *
    * @param queryKind      [I/ ] The query kind, should on one or combined of QUERYCONDITIONKIND values.
    *                             relative value such as: QUERY_KIND_MUSIC, QUERY_KIND_VIDEO, QUERY_KIND_PICTURE, QUERY_KIND_DOCUMENT.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SetCondition(INT32 queryKind);

    /*!
    * @brief Set the search condition, it should be called before calling to Search() function.
    *
    * @param ppszQuerys     [I/ ] The pointer to array of PCWSTR, each item of array typically is file extension.
    *                             such as "*.mp3", "*.png".
    * @param uCount         [I/ ] Indicates the count of the array.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SetCondition(LPCWSTR *ppszQuerys, UINT32 uCount);

    /*!
    * @brief Set the condition
    *
    * @param pQueryCondition    [I/ ] The condition object.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SetCondition(const SdkQueryCondition *pQueryCondition);

    /*!
    * @brief Get condition type.
    *
    * @return One or combined ofQUERYCONDITIONKIND values.
    */
    INT32 GetCodition() const;

    /*!
    * @brief Set the searching scope.
    *
    * @param scopeType      [I/ ] The scope type, one or combined of any value of QUERYSCOPETYPE enumeration.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SetScope(INT32 scopeType);

    /*!
    * @brief Set the search condition, it should be called before calling to Search() function.
    *
    * @param ppszQuerys     [I/ ] The pointer to array of PCWSTR, each item of array typically is file path.
    *                             such as "C:\\myFolder".
    * @param uCount         [I/ ] Indicates the count of the array.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SetScope(PCWSTR *pszScopePaths, UINT32 uCount);

    /*!
    * @brief Set search scope.
    *
    * @param pQueryScope    [I/ ] The SdkQueryScope object.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SetScope(const SdkQueryScope *pQueryScope);

    /*!
    * @brief Call this function begin to search files.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT Search();

    /*!
    * @brief Call this function begin to search files asynchronously.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SearchAsync();

    /*!
    * @brief Cancel search operation. This function only has effect when user searches files asynchronously.
    *        Caller will not get found results.
    */
    void CancelSearch();

    /*!
    * @brief Stop search,  This function only has effect when user searches files asynchronously.
    *        Caller can get found results.
    */
    void StopSearch();

    /*!
    * @brief Clear the search result.
    */
    void ClearSearchResult();

    /*!
    * @brief Clear search condition.
    */
    void ClearSearchCondition();

    /*!
    * @brief Clear search scope.
    */
    void ClearSearchScope();

    /*!
    * @brief Specifies Query notification. If you specifies a non-null pointer, the querying results
    *        will not push to result vector.
    *
    * @param pSearcherNotify    [I/ ] Pointer to IQueryNotify class, This function will
    *                                 not manage the memory pointed by pQueryNotify.
    *
    * @remark This function should be called before calling to Search() function.
    */
    void SetSearchNotify(IFileSearcherNotify *pSearcherNotify);

    /*!
    * @brief Get search result.
    *
    * @return The result list.
    */
    const vector<wstring>& GetSearchResult();

protected:

    /*!
    * @brief Indicates cancel to search or not.
    */
    BOOL HasCancelled();

    /*!
    * @brief Enumerate search result from an identifier list.
    *
    * @param pidl       [I/ ] The pointer to PIDL.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT EnumSearchResult(IN LPITEMIDLIST pidl);

    /*!
    * @brief Get shell item info pointed by pidl parameter.
    *
    * @param psf        [I/ ] The pointer to IShellFolder.
    * @param pidl       [I/ ] The pointer to PIDL.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT GetShellItemInfo(IN IShellFolder *psf, IN LPCITEMIDLIST pidl);

    /*!
    * @brief Search thread procedure.
    *
    * @param lpParameter    [I/ ] The thread data passed to the function using the lpParameter parameter of the CreateThread.
    *
    * @return The return value indicates the success or failure of this function.
    */
    static unsigned int WINAPI SearchThreadProc(LPVOID lpParameter);

private:

    BOOL                         m_hasStopped;                  // Stop search or not.
    BOOL                         m_hasCancel;                   // Indicates cancel to query or not.
    INT32                        m_nQueryConditionKind;         // Query condition kind.
    HANDLE                       m_uThreadHandle;               // The handle of thread.
    SdkQueryScope               *m_pTempQueryScope;             // Temp query scope, should not release it.
    SdkQueryScope               *m_pQueryScope;                 // The SdkQueryScope object.
    SdkQueryCondition           *m_pTempQueryCondition;         // Temp query condition, should not release it.
    SdkQueryCondition           *m_pQueryCondition;             // The SdkQueryCondition object.
    IFileSearcherNotify         *m_pSearcherNotify;             // The searcher notify.
    ISearchFolderItemFactory    *m_pSearchFolderItemFactory;    // Query manager.
    vector<wstring>              m_vctSearchResults;            // Search result.
};

END_NAMESPACE_COMMON

#endif // _SDKFILESEARCHER_H_
#endif // __cplusplus
