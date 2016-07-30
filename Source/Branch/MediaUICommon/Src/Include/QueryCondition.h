/*!
* @file QueryCondition.h
* 
* @brief This file defines class QueryScope using to create search condition.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#ifdef __cplusplus
#ifndef _QUERYCONDITION_H_
#define _QUERYCONDITION_H_

#include "Common.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The search condition kind
*/
typedef enum _QUERYCONDITIONKIND
{
    QUERY_KIND_NONE         = 0x00000000,
    QUERY_KIND_MUSIC        = 0x00000001,
    QUERY_KIND_VIDEO        = 0x00000002,
    QUERY_KIND_PICTURE      = 0x00000004,
    QUERY_KIND_DOCUMENT     = 0x00000008,
    QUERY_KIND_LINK         = 0x00000010,
    QUERY_KIND_RECENT       = 0x00000020,

} QUERYCONDITIONKIND;

/*!
* @brief QueryCondition class to manage searching condition.
*/
class CLASS_DECLSPEC QueryCondition
{
public:

    /*!
    * @brief The constructor functions.
    */
    QueryCondition();

    /*!
    * @brief The destructor functions.
    */
    ~QueryCondition();

    /*!
    * @brief Get the condition, ICondition instance.
    *
    * @param ppc            [ /O] Output ICondition instance.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT GetCondition(ICondition **ppc);

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
    * @brief Set the search condition, it should be called before calling to Search() function.
    *
    * @param queryKind      [I/ ] The query kind, should on one or combined of QUERYCONDITIONKIND values.
    *                             relative value such as: QUERY_KIND_MUSIC, QUERY_KIND_VIDEO, QUERY_KIND_PICTURE, QUERY_KIND_DOCUMENT.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT SetCondition(INT32 queryKind);

    /*!
    * @brief Clear the query condition.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT ClearCondtions();

private:

    /*!
    * @brief Create the query parser.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT CreateQueryParser();

    /*!
    * @brief parse the structured query.
    *
    * @param kind       [I/ ] The query kind.
    * @param ppc        [I/ ] Created condition instance.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT ParseStructuredQuery(QUERYCONDITIONKIND kind, OUT ICondition **ppc);

    /*!
    * @brief Create the query parser.
    *
    * @param pszString  [I/ ] The query string, typically is file extension, such as "*.mp3".
    * @param ppc        [I/ ] Created condition instance.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT ParseStructuredQuery(PCWSTR pszString, OUT ICondition **ppc);

    /*!
    * @brief Create the query parser.
    *
    * @param vctKinds   [I/O] The reference to vector<QUERYCONDITIONKIND>.
    *
    * @return S_OK if succeed, otherwise E_FAIL.
    */
    HRESULT GetQueryKindList(OUT vector<QUERYCONDITIONKIND> &vctKinds);

private:

    INT32                m_nQueryKind;          // The query kind value.
    UINT32               m_uQueryCount;         // The count of query condition array.
    PCWSTR              *m_pszQuerys;           // The query condition array.
    IQueryParser        *m_pQueryParser;        // The query parser.
};

END_NAMESPACE

#endif // _QUERYCONDITION_H_
#endif // __cplusplus
