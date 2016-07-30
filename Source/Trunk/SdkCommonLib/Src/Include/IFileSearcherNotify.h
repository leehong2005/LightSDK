/*!
* @file IFileSearcherNotify.h
* 
* @brief This file define IFileSearcherNotify class to give notification to caller.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/15
*/

#ifdef __cplusplus
#ifndef _IFILESEARCHERNOTIFY_H_
#define _IFILESEARCHERNOTIFY_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

class SdkFileSearcher;

/*!
* @brief The IFileSearcherNotify class, is used to notify caller.
*/
class CLASS_DECLSPEC IFileSearcherNotify
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IFileSearcherNotify() {};

    /*!
    * @brief Called when find a result.
    *
    * @param pfileSearcher  [I/ ] The SdkFileSearcher instance pointer.
    * @param lpFilePath     [I/ ] The full path of the found file.
    *                             lpFilePath maybe points a block of temporary memory, If called want
    *                             to duplicate the content of the memory, should copy the memory, not
    *                             points to lpFilePath directly.
    */
    virtual void OnSearchOneResult(IN SdkFileSearcher *pfileSearcher, IN LPCTSTR lpFilePath) = 0;

    /*!
    * @brief Called when find a result.
    *
    * @param pfileSearcher  [I/ ] The SdkFileSearcher instance pointer.
    */
    virtual void OnSearchFinish(IN SdkFileSearcher *pfileSearcher) = 0;
};

END_NAMESPACE_COMMON

#endif // _IFILESEARCHERNOTIFY_H_
#endif // __cplusplus