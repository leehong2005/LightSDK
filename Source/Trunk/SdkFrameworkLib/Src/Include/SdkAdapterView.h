/*!
* @file SdkAdapterView.h
* 
* @brief This file defines class SdkAdapterView, An SdkAdapterView is a view whose children are determined by an Adapter.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#ifdef __cplusplus
#ifndef _SDKADAPTERVIEW_H_
#define _SDKADAPTERVIEW_H_

#include "SdkCommon.h"
#include "SdkViewLayout.h"
#include "SdkDataSetObserver.h"
#include "SdkBaseAdapter.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief Represents the view whose children are decided by an Adapter.
*/
class CLASS_DECLSPEC SdkAdapterView : public SdkViewLayout, public SdkDataSetObserver
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkAdapterView();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkAdapterView();

    /*!
    * @brief Sets the adapter that provides the data and the views to represent the data in this view.
    *
    * @param pAdaptr    [I/ ] The SdkBaseAdapter object.
    */
    virtual void SetAdapter(SdkBaseAdapter *pAdapter);

    /*!
    * @brief Get the adapter object.
    *
    * @return SdkBaseAdapter pointer, may be NULL.
    */
    virtual SdkBaseAdapter* GetAdapter() const;

protected:

    /*!
    * @brief Call this method when need to draw some stuffs in render target.
    */
    virtual void OnPaint();

    /*!
    * @brief Get child view from adapter.
    */
    virtual void CreateViewFromAdapter();

    /*!
    * @brief Load view's associated data from adapter.
    */
    virtual void GetViewDataFromAdapter();

    /*!
    * @brief Cancel to get view's associated data from adapter.
    */
    virtual void CancelGetViewDataFromAdapter();

    /*!
    * @brief Indicates whether has cancelled to get view's data.
    *
    * @return TRUE if has cancelled, FALSE otherwise.
    */
    virtual BOOL HasCancelGetViewData();

    /*!
    * @brief Calculate child views' start and end index.
    *        Derived class should implement this function to tell rendering views' index.
    *
    * @param pStartIndex    [ /O] The start index of first rendering view.
    * @param pEndIndex      [ /O] The end index of last rendering view.
    */
    virtual void CalcChildViewIndex(OUT INT32 *pStartIndex, OUT INT32 *pEndIndex);

    /*!
    * @brief Virtualize view's according to specified start and end index.
    *
    * @param nStartIndex    [I/ ] The start index of first rendering view.
    * @param nEndIndex      [I/ ] The end index of last rendering view.
    */
    virtual void VirtualizeChildView(INT32 nStartIndex, INT32 nEndIndex);

    /*!
    * @brief Set view's data according to specified start and end index.
    *
    * @param nStartIndex    [I/ ] The start index of first rendering view.
    * @param nEndIndex      [I/ ] The end index of last rendering view.
    */
    virtual BOOL SetViewAssocData(INT32 nStartIndex, INT32 nEndIndex);

    /*!
    * @brief Clear the associated data with the view.
    */
    virtual void ClearViewAssocData();

    /*!
    * @brief This method is called when the entire data set has changed.
    */
    virtual void OnDataChanged();

    /*!
    * @brief Search thread procedure.
    *
    * @param lpParameter    [I/ ] The thread data passed to the function using the lpParameter parameter of the CreateThread.
    *
    * @return The return value indicates the success or failure of this function.
    */
    static unsigned int WINAPI GetViewDataThreadProc(LPVOID lpParameter);

protected:

    INT32                m_nStartIndex;                 // The start index.
    INT32                m_nEndIndex;                   // The end index.
    BOOL                 m_isFirstGetView;              // Indicates whether is first time to get views.
    BOOL                 m_hasCancelGetViewData;        // Indicates has cancelled to get view's data.
    HANDLE               m_uThreadHandle;               // The handle of thread.
    SdkBaseAdapter         *m_pBaseAdapter;                // The Adapter to provides data and views.
};

END_NAMESPACE_VIEWS

#endif // _SDKADAPTERVIEW_H_
#endif // __cplusplus
