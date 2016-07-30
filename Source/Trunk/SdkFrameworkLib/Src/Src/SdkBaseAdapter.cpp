/*!
* @file SdkBaseAdapter.cpp
* 
* @brief This file defines class SdkBaseAdapter, which acts as a bridge between an view layout and
*        underlying data for that view element.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#include "stdafx.h"
#include "SdkBaseAdapter.h"
#include "SdkDataSetObserver.h"

USING_NAMESPACE_VIEWS

SdkBaseAdapter::SdkBaseAdapter() : m_pDataSetObserver(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

SdkBaseAdapter::~SdkBaseAdapter()
{
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SdkBaseAdapter::GetView(INT32 nPos, SdkViewElement *pConvertView, SdkViewLayout *pParent)
{
    UNREFERENCED_PARAMETER(nPos);
    UNREFERENCED_PARAMETER(pConvertView);
    UNREFERENCED_PARAMETER(pParent);

    return NULL;
}

void SdkBaseAdapter::GetViewData(INT32 nPos, SdkViewElement *pConvertView)
{
    UNREFERENCED_PARAMETER(nPos);
    UNREFERENCED_PARAMETER(pConvertView);
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkBaseAdapter::GetCount()
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////

void SdkBaseAdapter::DeleteItem(INT32 nPos)
{
    UNREFERENCED_PARAMETER(nPos);
}

//////////////////////////////////////////////////////////////////////////

void SdkBaseAdapter::DeleteAll()
{
    // Not implements.
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkBaseAdapter::IsEmpty()
{
    return (0 == GetCount());
}

//////////////////////////////////////////////////////////////////////////

void SdkBaseAdapter::NotifyDataSetChanged()
{
    if (NULL != m_pDataSetObserver)
    {
        m_pDataSetObserver->OnDataChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkBaseAdapter::SetDataSetObserver(SdkDataSetObserver *pDataSetObserver)
{
    m_pDataSetObserver = pDataSetObserver;
}
