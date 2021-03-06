/*!
* @file IInkDispEventsImpl.h
* 
* @brief IInkDispEventsImpl.h
* The template is derived from the ATL's IDispEventSimpleImpl to implement a sink for the IInkDispEvents.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2009/12/07
* @version 1.0.0
*/

#ifdef __cplusplus
#ifndef _IINKEVENTSIMPLE_H_
#define _IINKEVENTSIMPLE_H_

#include "SdkInkCommon.h"

BEGIN_NAMESPACE_COMMON

#define SINK_ID                 1
#define COUNT_INKDISP_EVENTS    2

template <class T>
class ATL_NO_VTABLE IInkDispEventsImpl : 
    public IDispEventSimpleImpl<SINK_ID, IInkDispEventsImpl<T>, &(__uuidof(_IInkEvents))>
{
public:
    // ATL structure with the type information for the event, 
    // handled in this template. (Initialized in the AdvReco.cpp)
    static const _ATL_FUNC_INFO mc_AtlFuncInfo[COUNT_INKDISP_EVENTS];

    BEGIN_SINK_MAP(IInkDispEventsImpl)
        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkEvents),
        DISPID_IEInkAdded,
        &InkAdded, 
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[0]))
    END_SINK_MAP()

    /*!
    * @brief Occurs when a stroke is added to the InkDisp object.
    */
    void __stdcall InkAdded(VARIANT StrokeIds)
    {
        T* pT = static_cast<T*>(this);
        pT->OnInkAdded(StrokeIds);
    }
};

END_NAMESPACE_COMMON

#endif // _IINKEVENTSIMPLE_H_
#endif // __cplusplus
