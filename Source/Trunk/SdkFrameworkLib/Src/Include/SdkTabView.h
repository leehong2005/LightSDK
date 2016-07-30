/*!
* @file SdkTabView.h
* 
* @brief This file defines the SdkTabView class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#ifdef __cplusplus
#ifndef _SDKTABVIEW_H_
#define _SDKTABVIEW_H_

#include "SdkViewLayout.h"
#include "ITabHeaderEventHandler.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The Tab control
*/
class CLASS_DECLSPEC SdkTabView : public SdkViewLayout,
                                  public ITabHeaderEventHandler
{
public:

    /*!
    * @brief The constructor method.
    */
    SdkTabView();

    /*!
    * @brief The destructor method.
    */
    virtual ~SdkTabView();

    /*!
    * @brief Insert a tab at specified index.
    *
    * @param nIndex         [I/ ] The index of tab to be inserted.
    * @param lpText         [I/ ] The tab showing on the tab.
    *
    * @return TRUE if succeed, FALSE otherwise.
    */
    BOOL InsertTab(int nIndex, IN LPCTSTR lpTabText);

    /*!
    * @brief Delete a tab at specified index.
    *
    * @param nIndex         [I/ ] The index of tab to be deleted.
    *
    * @return TRUE if succeed, FALSE otherwise.
    */
    BOOL DeleteTab(int nIndex);

    /*!
    * @brief Call this method to add a specified view to specified tab page.
    *
    * @param nIndex         [I/ ] The index of the tab.
    * @param pTabView       [I/ ] The view to be added to tab page.
    *
    * @return TRUE if succeeds to add view, otherwise FALSE.
    */
    BOOL AddTabView(int nIndex, SdkViewElement *pTabView);

    /*!
    * @brief Call this method to delete a specified view from specified tab page.
    *
    * @param nIndex         [I/ ] The index of the tab.
    * @param pTabView       [I/ ] The view to be deleted from tab page.
    *
    * @return TRUE if succeeds to delete view, otherwise FALSE.
    */
    BOOL DeleteTabView(int nIndex, SdkViewElement *pTabView);

    /*!
    * @brief Select the tab at the specified index.
    *
    * @param nIndex         [I/ ] The selected tab index.
    */
    void SetSelTab(int nIndex);

    /*!
    * @brief Set the text of tab.
    *
    * @param nIndex         [I/ ] The index of tab to be updated text.
    * @param lpText         [I/ ] The text showing on the tab.
    *
    * @return TRUE if succeed to insert a tab, FALSE otherwise.
    */
    BOOL SetTabText(int nIndex, IN LPCTSTR lpText);

    /*!
    * @brief Set the width of tab, if the docking is left or right, the parameter
    *        is the height of the tab.
    *
    * @param nTabWidth      [I/ ] The width of tab.
    */
    void SetTabWidth(int nTabWidth);

    /*!
    * @brief Set flag to indicates the add tab button visible or invisible.
    *
    * @param fVisible       [I/ ] TRUE if visible, FALSE if invisible.
    */
    void SetAddTabVisible(BOOL fVisible);

    /*!
    * @brief Set the tab dock direction.
    *
    * @param eDockDir       [I/ ] One value of TAB_DOCK_DIR enumeration.
    */
    void SetTabDockDir(TAB_DOCK_DIR eDockDir);

    /*!
    * @brief Set the ITabViewEventHandler to get notification of the tab view.
    *
    * @param pEventHandler  [I/ ] The pointer to ITabViewEventHandler.
    */
    void SetTabViewEventHandler(ITabViewEventHandler *pEventHandler);

    /*!
    * @brief Get the selected tab index.
    *
    * @return The index of the selected tab.
    */
    int GetSelTab() const;

    /*!
    * @brief Get the count of tabs.
    *
    * @return The count of tabs.
    */
    int GetTabCount();

    /*!
    * @brief Get the text of the specified tab.
    *
    * @param nIndex         [I/ ] The index of the tab.
    * @param lpTabText      [ /O] The buffer to receive the text of the tab.
    */
    BOOL GetTabText(int nIndex, LPTSTR lpTabText);

protected:

    /*!
    * @brief Get the page associated with the specified tab.
    *
    * @param nIndex         [I/ ] The index of the tab.
    *
    * @return The pointer to SdkViewLayout, the tab page, user can add views to this page.
    */
    SdkViewLayout* GetTabPage(int nIndex);

    /*!
    * @brief Call this method to set layout of tab header, in this method, it will calculate
    *        the size of tab header according to the docking direction.
    *
    * @param width          [I/ ] The width of view.
    * @param height         [I/ ] The height of view.
    *
    * @return The rectangle of tag pages, the rectangle is relative coordinates, not absolute.
    */
    D2D1_RECT_F SetTabHeaderLayout(FLOAT width, FLOAT height);

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Called when selection of tab is changed.
    *
    * @param pTabHeader     [I/ ] The event source.
    * @param nOldIndex      [I/ ] The old selected index.
    * @param nCurIndex      [I/ ] The current selected index.
    */
    virtual void OnSelectedChanged(SdkTabHeader *pTabHeader, int nOldIndex, int  nCurIndex);

    /*!
    * @brief Called when user click the add tab button, the adding operation should be
    *        performed by client side.
    *
    * @param pTabHeader     [I/ ] The event source.
    */
    virtual BOOL OnInsertTab(SdkTabHeader *pTabHeader);

    /*!
    * @brief Called when user delete a tab by clicking the close button on the tab, before 
    *        invoking this method, the tab has been deleted. in the implement of this method,
    *        the client side should do the cleanup operation if there is associated data to 
    *        the deleted tab.
    *
    * @param pTabHeader     [I/ ] The event source.
    * @param nDelIndex      [I/ ] The index of the deleted tab.
    */
    virtual BOOL OnDeleteTab(SdkTabHeader *pTabHeader, int nDelIndex);

    /*!
    * @brief Called when the layout of view is changed.
    *
    * @param fChanged       [I/ ] Indicates whether is changed.
    * @param left           [I/ ] The left value.
    * @param top            [I/ ] The top value.
    * @param width          [I/ ] The width of view.
    * @param height         [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

private:

    /*!
    * @brief The Tab view internal data.
    */
    struct _TABVIEW_INTERNALDATA;

    _TABVIEW_INTERNALDATA   *m_pInternalData;   // Internal data pointer.
    SdkTabHeader            *m_pTabHeader;      // The tab header view.
};

END_NAMESPACE_VIEWS

#endif // _SDKTABVIEW_H_
#endif // __cplusplus
