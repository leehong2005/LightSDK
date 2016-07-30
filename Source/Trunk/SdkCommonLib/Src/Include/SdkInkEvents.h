/*!
* @file SdkInkEvents.h
* 
* @brief This class defines the events of ink, the implement of these events should be done in its derived class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/19 12:00
* @version 1.0.0
*/

#ifdef __cplusplus
#ifndef _SDKINKEVENT_H_
#define _SDKINKEVENT_H_

#include "IInkRecognitionEventsImpl.h"
#include "IInkOverlayEventsImpl.h"
#include "IInkDispEventsImpl.h"

USING_NAMESPACE_COMMON

BEGIN_NAMESPACE_COMMON

/*!
* @brief The base class of ink events.
*/
class CLASS_DECLSPEC SdkInkEvents : public IInkRecognitionEventsImpl<SdkInkEvents>, 
                                    public IInkOverlayEventsImpl<SdkInkEvents>,
                                    public IInkDispEventsImpl<SdkInkEvents>
{
public:

    /*!
    * @brief The default constructor function.
    */
    SdkInkEvents(void);

    /*!
    * @brief The default destructor function.
    */
    virtual ~SdkInkEvents(void);

    /*!
    * @brief Call this method to establish a connection with the event source represented by pUnknown.
    *        Derived class should call this method to establish a connection with the event source.
    *
    * @param *pUnknown  [I/ ] A pointer to the IUnknown interface of the event source object.
    *
    * @return HRESULT S_OK or any failure HRESULT value.
    */
    virtual HRESULT AdviseInkEvent(IN IUnknown *pUnknown);

    /*!
    * @brief Breaks the connection with the event source represented by pUnknown.
    *        This method should be called by derived class when the event connection is no longer used.
    *
    * @param *pUnknown  [I/ ] A pointer to the IUnknown interface of the event source object.
    *
    * @return HRESULT S_OK or any failure HRESULT value.
    */
    virtual HRESULT UnadviseInkEvent(IN IUnknown *pUnknown);

    /*!
    * @brief Occurs when the InkRecognizerContext has generated results after calling the
    *        BackgroundRecognizeWithAlternates method.
    */
    virtual void OnRecognitionWithAlternates(
        IInkRecognitionResult* pIInkRecoResult,
        VARIANT vCustomParam,
        InkRecognitionStatus RecognitionStatus);

    /*!
    * @brief Occurs when the user draws a new stroke on any tablet.
    *
    * @param pIInkCursor [I/ ] The IInkCursor object that generated the Stroke event.
    * @param pInkStroke  [I/ ] The collected IInkStrokeDisp object.
    * @param pbCancel    [I/ ] Specifies whether the event should be canceled. If TRUE, the collection of the stroke is canceled.
    */
    virtual void OnStroke(
        IInkCursor* pIInkCursor,
        IInkStrokeDisp* pInkStroke,
        VARIANT_BOOL* pbCancel);

    /*!
    * @brief Occurs when an application-specific gesture is recognized.
    *
    * @param pIInkCursor [I/ ] The IInkCursor object that generated the Gesture event.
    * @param pInkStrokes [I/ ] The IInkStrokes collection that the recognizer returned as the gesture.
    * @param vGestures   [I/ ] An array of IInkGesture objects, in order of confidence, from the recognizer.
    * @param pbCancel    [I/ ] Whether the collection of this gesture should be canceled,
    *                          such as not to erase the ink and to fire the Stroke event.
    */
    virtual void OnGesture(
        IInkCursor* pIInkCursor,
        IInkStrokes* pInkStrokes,
        VARIANT vGestures,
        VARIANT_BOOL* pbCancel);

    /*!
    * @brief Occurs when an application-specific gesture is recognized.
    *
    * @param OldSelectionRect   [I/ ] The bounding rectangle of the selected InkStrokes collection as it existed 
    *                                 before the SelectionMoved event fired.
    *
    * @remark There is limit that the selection strokes can be moved out of 
    *         client rectangle. The function will implement this function.
    */
    virtual void OnSelectionMoved(IInkRectangle *OldSelectionRect);

    /*!
    * @brief Occurs when the size of the current selection has changed, for example through
    *        alterations to the user interface, cut-and-paste procedures, or the Selection property.
    *
    * @param OldSelectionRect   [I/ ] The bounding rectangle of the selected InkStrokes collection as it existed 
    *                                 before the SelectionResized event fired.
    */
    virtual void OnSelectionResized(IInkRectangle *OldSelectionRect);

    /*!
    * @brief Occurs when the ink collector receives packet.
    *
    * @param Cursor             [I/ ] The IInkCursor object that generated the NewInAirPackets event.
    * @param Stroke             [I/ ] Specifies the IInkStrokeDisp object.
    * @param PacketCount        [I/ ] The number of packets received for a IInkStrokeDisp object.
    * @param PacketData         [I/ ] An array that contains the selected data for the packet.
    */
    virtual void OnNewPackets(
        IInkCursor *Cursor, 
        IInkStrokeDisp *Stroke, 
        long PacketCount, 
        VARIANT *PacketData);

    /*!
    * @brief This function can be called when the selection of ink within the control has changed.
    */
    virtual void OnSelectionChanged();

    /*!
    * @brief This function can be called before strokes are deleted from the Ink property.
    */
    virtual void OnStrokesDeleting(IInkStrokes *Strokes);

    /*!
    * @brief Occurs when a stroke is added to the InkDisp object.
    */
    virtual void OnInkAdded(VARIANT StrokeIds);
};


//////////////////////////////////////////////////////////////////////////
//
// Initialize the members of IInkRecognitionEventsImpl and  IInkOverlayEventsImpl.
//

const _ATL_FUNC_INFO IInkRecognitionEventsImpl<SdkInkEvents>::mc_AtlFuncInfo[COUNT_RECOG_EVENTS] =
{ CC_STDCALL, VT_EMPTY, 3, {VT_UNKNOWN, VT_VARIANT, VT_I4} };

const _ATL_FUNC_INFO IInkOverlayEventsImpl<SdkInkEvents>::mc_AtlFuncInfo[COUNT_OVERLAY_EVENTS] = 
{
    {CC_STDCALL, VT_EMPTY, 3, {VT_UNKNOWN, VT_UNKNOWN, VT_BOOL|VT_BYREF}},
    {CC_STDCALL, VT_EMPTY, 4, {VT_UNKNOWN, VT_UNKNOWN, VT_VARIANT, VT_BOOL|VT_BYREF}},
    {CC_STDCALL, VT_EMPTY, 1, {VT_UNKNOWN}},
    {CC_STDCALL, VT_EMPTY, 1, {VT_UNKNOWN}},
    {CC_STDCALL, VT_EMPTY, 4, {VT_BYREF|VT_VARIANT, VT_I4, VT_UNKNOWN, VT_UNKNOWN}},
    {CC_STDCALL, VT_EMPTY, 0, {}},
    {CC_STDCALL, VT_EMPTY, 1, {VT_UNKNOWN}}
};

const _ATL_FUNC_INFO IInkDispEventsImpl<SdkInkEvents>::mc_AtlFuncInfo[COUNT_INKDISP_EVENTS] =
{
    CC_STDCALL, VT_EMPTY, 1, {VT_VARIANT}
};

END_NAMESPACE_COMMON

#endif // _SDKINKEVENT_H_
#endif // __cplusplus
