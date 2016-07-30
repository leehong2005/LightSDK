/*!
* @file QueryScope.h
* 
* @brief This file defines class QueryScope using to create search scope.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#ifdef __cplusplus
#ifndef _QUERYSCOPE_H_
#define _QUERYSCOPE_H_

#include "Common.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The predefined searching scope.
*/
typedef enum _QUERYSCOPETYPE
{
    QUERY_SCOPE_NONE            = 0x00000000,
    QUERY_SCOPE_PICTURES        = 0x00000001,
    QUERY_SCOPE_MUSIC           = 0x00000002,
    QUERY_SCOPE_VIDEO           = 0x00000004,
    QUERY_SCOPE_DECUMENT        = 0x00000008,
    QUERY_SCOPE_PUBLIC          = 0x00000010,
    QUERY_SCOPE_SAMPLEMUSIC     = 0x00000020,
    QUERY_SCOPE_DESKTOP         = 0x00000040,
    QUERY_SCOPE_STARTMENU       = 0x00000080,
    QUERY_SCOPE_FAVORITES       = 0x00000100,
    QUERY_SCOPE_COMPUTER        = 0x00000200,
    QUERY_SCOPE_EXTERNALSTORAGE = 0x00000400,

} QUERYSCOPETYPE;

/*!
* @brief QueryScope class.
*/
class CLASS_DECLSPEC QueryScope
{
public:

    /*!
    * @brief Initialize the scope map, should be called before any calling to this class's functions.
    */
    static void InitializeMap();

    /*!
    * @brief The constructor functions.
    */
    QueryScope();

    /*!
    * @brief The destructor functions.
    */
    ~QueryScope();

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
    * @brief Get the scope, IShellItemArray instance.
    *
    * @param ppsia          [ /O] Output IShellItemArray instance.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT GetScope(OUT IShellItemArray **ppsia);

    /*!
    * @brief Clear scope values.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT ClearScope();

private:

    /*!
    * @brief Get scope type from m_nScopeType.
    *
    * @param vctScopeTypes  {I/O] The reference to vector<QUERYSCOPETYPE>.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT GetScopeType(OUT vector<QUERYSCOPETYPE> &vctScopeTypes);

private:

    /*!
    * @brief QUERYSCOPETYPE to KNOWNFOLDERID map.
    */
    typedef map <QUERYSCOPETYPE, KNOWNFOLDERID> TYPETOFOLDERIDMAP;

    /*!
    * @brief QUERYSCOPETYPE to KNOWNFOLDERID pair.
    */
    typedef pair<QUERYSCOPETYPE, KNOWNFOLDERID> TYPETOFOLDERIDPAIR;

    INT32                       m_nScopeType;               // The scope type, maybe combined of QUERYSCOPETYPE's value.
    UINT32                      m_uScopePathsCount;         // The count of scope path array.
    PCWSTR                     *m_pszScopePaths;            // The scope path array.
    static TYPETOFOLDERIDMAP    s_mapTypeToFolderID;        // The Type to Folder ID map.
};

END_NAMESPACE

#endif // _QUERYSCOPE_H_
#endif // __cplusplus
