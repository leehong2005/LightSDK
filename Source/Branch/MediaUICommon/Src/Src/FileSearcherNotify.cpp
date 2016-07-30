/*!
* @file FileSearcherNotify.cpp
* 
* @brief This file define FileSearcherNotify class to give notification to caller.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/15
*/

#include "FileSearcherNotify.h"

USING_NAMESPACE_COMMON

void FileSearcherNotify::OnSearchOneResult(IN FileSearcher *pfileSearcher, IN LPCTSTR lpFilePath)
{
    UNREFERENCED_PARAMETER(pfileSearcher);
    UNREFERENCED_PARAMETER(lpFilePath);
}

//////////////////////////////////////////////////////////////////////////

void FileSearcherNotify::OnSearchFinish(IN FileSearcher *pfileSearcher)
{
    UNREFERENCED_PARAMETER(pfileSearcher);
}
