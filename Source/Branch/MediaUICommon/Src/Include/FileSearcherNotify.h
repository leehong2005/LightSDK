/*!
* @file FileSearcherNotify.h
* 
* @brief This file define FileSearcherNotify class to give notification to caller.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/15
*/

#ifdef __cplusplus
#ifndef _FILESEARCHERNOTIFY_H_
#define _FILESEARCHERNOTIFY_H_

#include "Common.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

class FileSearcher;

/*!
* @brief The FileSearcherNotify class, is used to notify caller.
*/
class CLASS_DECLSPEC FileSearcherNotify
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~FileSearcherNotify() {};

    /*!
    * @brief Called when find a result.
    *
    * @param pfileSearcher  [I/ ] The FileSearcher instance pointer.
    * @param lpFilePath     [I/ ] The full path of the found file.
    *                             lpFilePath maybe points a block of temporary memory, If called want
    *                             to duplicate the content of the memory, should copy the memory, not
    *                             points to lpFilePath directly.
    */
    virtual void OnSearchOneResult(IN FileSearcher *pfileSearcher, IN LPCTSTR lpFilePath);

    /*!
    * @brief Called when find a result.
    *
    * @param pfileSearcher  [I/ ] The FileSearcher instance pointer.
    */
    virtual void OnSearchFinish(IN FileSearcher *pfileSearcher);
};

END_NAMESPACE

#endif // _FILESEARCHERNOTIFY_H_
#endif // __cplusplus