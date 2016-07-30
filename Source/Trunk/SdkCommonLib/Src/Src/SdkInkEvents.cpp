/*!
* @file SdkInkEvents.cpp
* 
* @brief Implementation of the CInkEvents class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/19 12:00
* @version 1.0.0
*/

#include "stdafx.h"
#include "SdkInkEvents.h"

USING_NAMESPACE_COMMON

//////////////////////////////////////////////////////////////////////////

SdkInkEvents::SdkInkEvents()
{
}

//////////////////////////////////////////////////////////////////////////

SdkInkEvents::~SdkInkEvents()
{
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkInkEvents::AdviseInkEvent(IN IUnknown *pUnknown)
{
    if ( NULL == pUnknown )
    {
        return E_FAIL;
    }

    //DWORD dwEventCookie = 0xFEFEFEFE;
    IInkOverlay *pInkOverlay = NULL;
    IInkRecognizerContext *pInkRecogContext = NULL;
    IInkDisp *pInkDisp = NULL;

    pUnknown->QueryInterface(IID_PPV_ARGS(&pInkOverlay));
    if ( NULL != pInkOverlay )
    {
        return IInkOverlayEventsImpl<SdkInkEvents>::DispEventAdvise(pInkOverlay);
        //return ::AtlAdvise(pInkOverlay, (IUnknown*)static_cast<IInkOverlayEventsImpl*>(this), __uuidof(_IInkOverlayEvents), &dwEventCookie);
    }

    pUnknown->QueryInterface(IID_PPV_ARGS(&pInkRecogContext));
    if ( NULL != pInkRecogContext )
    {
        return IInkRecognitionEventsImpl<SdkInkEvents>::DispEventAdvise(pInkRecogContext);
        //return ::AtlAdvise(pInkRecogContext, (IUnknown*)static_cast<IInkRecognitionEventsImpl*>(this), __uuidof(_IInkRecognitionEvents), &dwEventCookie);
    }

    pUnknown->QueryInterface(IID_PPV_ARGS(&pInkDisp));
    if( NULL != pInkDisp)
    {
        return IInkDispEventsImpl<SdkInkEvents>::DispEventAdvise(pInkDisp);
        //return ::AtlAdvise(pInkDisp, (IUnknown*)static_cast<IInkDispEventsImpl*>(this), __uuidof(_IInkEvents), &dwEventCookie);
    }

    return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkInkEvents::UnadviseInkEvent(IN IUnknown *pUnknown)
{
    if ( NULL == pUnknown )
    {
        return E_FAIL;
    }

    IInkOverlay *pInkOverlay = NULL;
    IInkRecognizerContext *pInkRecogContext = NULL;
    IInkDisp *pInkDisp = NULL;

    pUnknown->QueryInterface(IID_PPV_ARGS(&pInkOverlay));
    if ( NULL != pInkOverlay )
    {
        return IInkOverlayEventsImpl<SdkInkEvents>::DispEventUnadvise(pInkOverlay);
    }

    pUnknown->QueryInterface(IID_PPV_ARGS(&pInkRecogContext));
    if ( NULL != pInkRecogContext )
    {
        return IInkRecognitionEventsImpl<SdkInkEvents>::DispEventUnadvise(pInkRecogContext);
    }

    pUnknown->QueryInterface(IID_PPV_ARGS(&pInkDisp));
    if( NULL != pInkDisp)
    {
        return IInkDispEventsImpl<SdkInkEvents>::DispEventUnadvise(pInkDisp);
    }

    return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnRecognitionWithAlternates(
    IInkRecognitionResult* pIInkRecoResult,
    VARIANT vCustomParam,
    InkRecognitionStatus RecognitionStatus)
{
    // Not implement currently
    UNREFERENCED_PARAMETER(pIInkRecoResult);
    UNREFERENCED_PARAMETER(vCustomParam);
    UNREFERENCED_PARAMETER(RecognitionStatus);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnStroke(
    IInkCursor* pIInkCursor,
    IInkStrokeDisp* pInkStroke,
    VARIANT_BOOL* pbCancel)
{
    // Not implement currently
    UNREFERENCED_PARAMETER(pIInkCursor);
    UNREFERENCED_PARAMETER(pInkStroke);
    UNREFERENCED_PARAMETER(pbCancel);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnGesture(
    IInkCursor* pIInkCursor,
    IInkStrokes* pInkStrokes,
    VARIANT vGestures,
    VARIANT_BOOL* pbCancel)
{
    // Not implement currently
    UNREFERENCED_PARAMETER(pIInkCursor);
    UNREFERENCED_PARAMETER(pInkStrokes);
    UNREFERENCED_PARAMETER(vGestures);
    UNREFERENCED_PARAMETER(pbCancel);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnSelectionMoved(IInkRectangle *OldSelectionRect)
{
    // Not implement currently
    UNREFERENCED_PARAMETER(OldSelectionRect);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnSelectionResized(IInkRectangle *OldSelectionRect)
{
    // Not implement currently
    UNREFERENCED_PARAMETER(OldSelectionRect);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnNewPackets(
    IInkCursor *Cursor,
    IInkStrokeDisp *Stroke,
    long PacketCount,
    VARIANT *PacketData)
{
    // Not implement currently
    UNREFERENCED_PARAMETER(Cursor);
    UNREFERENCED_PARAMETER(Stroke);
    UNREFERENCED_PARAMETER(PacketCount);
    UNREFERENCED_PARAMETER(PacketData);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnSelectionChanged()
{
    // Not implement currently
}

/////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnStrokesDeleting(IInkStrokes *Strokes)
{
    // Not implement currently
     UNREFERENCED_PARAMETER(Strokes);
}

/////////////////////////////////////////////////////////////////////////

void SdkInkEvents::OnInkAdded(VARIANT StrokeIds)
{
    // Not implement currently
    UNREFERENCED_PARAMETER(StrokeIds);
}
