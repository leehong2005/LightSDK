/*!
* @file SdkTabHeader.h
* 
* @brief This file defines the SdkTabView class.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#ifdef __cplusplus
#ifndef _SDKTABHEADER_H_
#define _SDKTABHEADER_H_

#include "SdkViewLayout.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The Tab control
*/
class CLASS_DECLSPEC SdkTabHeader : public SdkViewLayout, public IViewOnClickHandler
{
public:

    /*!
    * @brief The constructor method.
    */
    SdkTabHeader();

    /*!
    * @brief The destructor method.
    */
    virtual ~SdkTabHeader();

    /*!
    * @brief Insert a tab to specified index.
    *
    * @param nIndex         [I/ ] The index of tab to be inserted.
    * @param lpText         [I/ ] The tab showing on the tab.
    * @param dwData         [I/ ] The address of the associated data with the new inserted tag.
    *
    * @return TRUE if succeed to insert a tab, FALSE otherwise.
    */
    BOOL InsertTab(int nIndex, IN LPCTSTR lpText, LONG_PTR dwData);

    /*!
    * @brief Delete tab at the specified index.
    *
    * @param nIndex         [I/ ] The index of tab to be deleted.
    *
    * @return TRUE if succeed to delete the tab, FALSE otherwise.
    */
    BOOL DeleteTab(int nIndex);

    /*!
    * @brief Set the data of tab by parameter nIndex. If the tab has a data already, this 
    *        new data will replace the old data.
    *
    * @param nIndex         [I/ ] The index of tab to be updated text.
    * @param dwData         [I/ ] The data address associated with this tab.
    *
    * @return TRUE if succeed, FALSE otherwise.
    */
    BOOL SetTabData(int nIndex, LONG_PTR dwData);

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
    * @brief Set the tab information.
    *
    * @param nIndex         [I/ ] The index of tab to be updated information.
    * @param lpTabInfo      [I/ ] The pointer to TAB_INFO structure.
    *
    * @return TRUE if succeed to insert a tab, FALSE otherwise.
    */
    BOOL SetTabInfo(int nIndex, IN const LPTAB_INFO lpTabInfo);

    /*!
    * @brief Offset the one or more tabs towards left(up) or right(down).
    *        negative: left(up), plus: right(down).
    *
    * @param nTabNum        [I/ ] The number of the tab to offset, usually -1, or 1.
    */
    void SetTabOffset(int nTabNum);

    /*!
    * @brief Select the tab at the specified index.
    *
    * @param nIndex         [I/ ] The selected tab index.
    */
    void SetSelTab(int nIndex);

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
    * @brief Set the Tab header event handler pointer to get notification when selected tab changed.
    *
    * @param pEventHandler  [I/ ] The pointer to ITabHeaderEventHandler.
    */
    void SetTabHeaderEventHandler(ITabHeaderEventHandler *pEventHandler);

    /*!
    * @brief Get the tab information at specified index.
    *
    * @param nIndex         [I/ ] The index of tab to be got information.
    * @param lpTabInfo      [ /O] The pointer to TAB_INFO structure to receive tab information.
    *
    * @return TRUE if succeed to insert a tab, FALSE otherwise.
    */
    BOOL GetTabInfo(int nIndex, OUT LPTAB_INFO lpTabInfo);

    /*!
    * @brief Get the data address of the tab specified by nIndex parameter. The data is usually set by 
    *        calling InsertTab or SetTabData methods.
    *
    * @param nIndex         [I/ ] The index of the tab, based zero.
    *
    * @return The address of the data.
    */
    LONG_PTR GetTabData(int nIndex);

    /*!
    * @brief Get the selected tab index.
    *
    * @return The index of the selected tab.
    */
    int GetSelTab() const;

    /*!
    * @brief Get the number of tabs.
    *
    * @return The number of tabs.
    */
    int GetTabCount() const;

protected:

    /*!
    * @brief Call this method to initialize.
    */
    void InitializeViews();

    /*!
    * @brief Call this method to show or hide next and previous tab button.
    */
    void ShowNextPrevTabButtons();

    /*!
    * @brief Get the tab index according to th point.
    *
    * @param xPos           [I/ ] The x-coordinate.
    * @param yPos           [I/ ] The y-coordinate.
    *
    * @return The index of the tab, -1 if the point is not in tab's rectangle.
    */
    int GetTabIndexFromPoint(FLOAT xPos, FLOAT yPos);

    /*!
    * @brief Get the drawing bitmap according to the specified tab index.
    *
    * @param nTabIndex      [I/ ] The index of tab.
    *
    * @return The pointer to D2DBitmap.
    */
    D2DBitmap* GetDrawingTabBitmap(int nTabIndex);

    /*!
    * @brief Call this method when user click the add tab button.
    */
    virtual void OnAddTabButtonClick();

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Call this method to draw all tabs.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    * @param rc             [I/ ] The reference to D2D1_RECT_F to indicate the rectangle of tab.
    */
    virtual void OnDrawTabs(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &rc);

    /*!
    * @brief Call this method to draw text.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    * @param nTabIndex      [I/ ] The index of tab to be drawing.
    * @param rc             [I/ ] The reference to D2D1_RECT_F to indicate the rectangle of tab.
    */
    virtual void OnDrawTabText(ID2D1RenderTarget *pRenderTarget, int nTabIndex, const D2D1_RECT_F &rc);

    /*!
    * @brief Call this method to draw the add tab button.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    * @param rc             [I/ ] The rectangle of the drawing.
    */
    virtual void OnDrawAddTabButton(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &rc);

    /*!
    * @brief The destructor function.
    *
    * @param view       [I/ ] The pointer SdkViewElement, you should NOT delete the pointer.
    */
    virtual void OnClick(SdkViewElement *pView);

    /*!
    * @brief Called when a view has been clicked and held.
    *
    * @param pView      [I/ ] The event source.
    *
    * @return TRUE if consumed the long click, FALSE otherwise.
    */
    virtual BOOL OnLongClick(SdkViewElement *pView);

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

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] message.
    * @param wParam         [I/ ] parameter.
    * @param lParam         [I/ ] parameter.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

protected:

    /*!
    * @brief The tab header internal data.
    */
    struct _TABHEADER_INTERNALDATA;

    _TABHEADER_INTERNALDATA  *m_pTabHeaderData;     // The tab-header internal data.
};

END_NAMESPACE_VIEWS

#endif // _SDKTABHEADER_H_
#endif // __cplusplus
