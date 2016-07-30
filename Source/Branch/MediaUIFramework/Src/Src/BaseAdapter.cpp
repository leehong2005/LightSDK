/*!
* @file BaseAdapter.cpp
* 
* @brief This file defines class BaseAdapter, which acts as a bridge between an view layout and
*        underlying data for that view element.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#include "BaseAdapter.h"
#include "DataSetObserver.h"

BaseAdapter::BaseAdapter() : m_pDataSetObserver(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

BaseAdapter::~BaseAdapter()
{
}

//////////////////////////////////////////////////////////////////////////

ViewElement* BaseAdapter::GetView(INT32 nPos, ViewElement *pConvertView, ViewLayout *pParent)
{
    UNREFERENCED_PARAMETER(nPos);
    UNREFERENCED_PARAMETER(pConvertView);
    UNREFERENCED_PARAMETER(pParent);

    return NULL;
}

void BaseAdapter::GetViewData(INT32 nPos, ViewElement *pConvertView)
{
    UNREFERENCED_PARAMETER(nPos);
    UNREFERENCED_PARAMETER(pConvertView);
}

//////////////////////////////////////////////////////////////////////////

INT32 BaseAdapter::GetCount()
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////

void BaseAdapter::DeleteItem(INT32 nPos)
{
    UNREFERENCED_PARAMETER(nPos);
}

//////////////////////////////////////////////////////////////////////////

void BaseAdapter::DeleteAll()
{
    // Not implements.
}

//////////////////////////////////////////////////////////////////////////

BOOL BaseAdapter::IsEmpty()
{
    return (0 == GetCount());
}

//////////////////////////////////////////////////////////////////////////

void BaseAdapter::NotifyDataSetChanged()
{
    if (NULL != m_pDataSetObserver)
    {
        m_pDataSetObserver->OnDataChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void BaseAdapter::SetDataSetObserver(DataSetObserver *pDataSetObserver)
{
    m_pDataSetObserver = pDataSetObserver;
}
