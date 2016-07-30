/*!
* @file SdkBaseAdapter.h 
* 
* @brief This file defines class SdkBaseAdapter, which acts as a bridge between an view layout and
*        underlying data for that view element.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#ifdef __cplusplus
#ifndef _SDKBASEADAPTER_H_
#define _SDKBASEADAPTER_H_

#include "SdkCommon.h"
#include "SdkUICommon.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The bridge of adapter view and associated data.
*/
class CLASS_DECLSPEC SdkBaseAdapter
{
public:

    friend class SdkAdapterView;

    /*!
    * @brief The constructor function.
    */
    SdkBaseAdapter();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkBaseAdapter();

    /*!
    * @brief Get the view element which will be represented in the adapter view.
    *
    * @param nPos           [I/ ] The position of the item within the adapter's data set of the item whose view we want.
    * @param pConvertView   [I/ ] The old view to reuse, you should check that this view is nun-null.
    * @param pParent        [I/ ] he parent that this view will eventually be attached to, not used.
    *
    * @return The view corresponding to the data at the specified position.
    *
    * @remark Generally speaking, derived class provides corresponding view and not load associated data
    *         when implements this function.
    */
    virtual SdkViewElement* GetView(INT32 nPos, SdkViewElement *pConvertView, SdkViewLayout *pParent);

    /*!
    * @brief Load data for view on the adapter view. Derived class should override this method
    *        to provide associated data for the specified view.
    *
    * @param nPos           [I/ ] The position of the item within the adapter's data set of the item whose view we want.
    * @param pConvertView   [I/ ] The old view to reuse, you should check that this view is nun-null.
    */
    virtual void GetViewData(INT32 nPos, SdkViewElement *pConvertView);

    /*!
    * @brief How many views are in the data set represented by this adapter.
    *
    * @return The number of views.
    */
    virtual INT32 GetCount();

    /*!
    * @brief Delete data from adapter at specified index.
    *
    * @param nPos   [I/ ] The index.
    */
    virtual void DeleteItem(INT32 nPos);

    /*!
    * @brief Delete all items in adapter. When all children are removed from SdkAdapterView, 
    *        this function will be invoked, derived class should override this method to 
    *        delete all data associated with views in SdkAdapterView.
    */
    virtual void DeleteAll();

    /*!
    * @brief Indicates the data is empty or not.
    *
    * @return TRUE if data set is empty, otherwise FALSE.
    */
    virtual BOOL IsEmpty();

    /*!
    * @brief Notify data set is changed.
    */
    virtual void NotifyDataSetChanged();

protected:

    /*!
    * @brief Set the SdkDataSetObserver object to notify the data set changed callback.
    *
    * @param pDataSetObserver   [I/ ] The SdkDataSetObserver object.
    */
    void SetDataSetObserver(SdkDataSetObserver *pDataSetObserver);

protected:

    SdkDataSetObserver     *m_pDataSetObserver;    // The data set observer object.
};

END_NAMESPACE_VIEWS

#endif // _SDKBASEADAPTER_H_
#endif // __cplusplus
