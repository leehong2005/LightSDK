/*!
* @file MenuList.h
* 
* @brief This file defines the MenuList class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/14
*/

#ifdef __cplusplus
#ifndef _LISTBOX_H_
#define _LISTBOX_H_

#include "CommonInclude.h"
#include "ViewLayout.h"
#include "PopupView.h"
#include "ImageButtonEx.h"
#include "D2DSolidColorBrush.h"
#include "D2DBitmapBrush.h"
#include "ListViewEventHandler.h"
#include "ViewOnMouseHandler.h"

class CLASS_DECLSPEC ListBox : public PopupView, 
                               public ViewOnClickHandler,
                               public ViewOnMouseHandler
{
public:

    /*!
    * @brief The constructor function.
    */
    ListBox();

    /*!
    * @brief The destructor function.
    */
    virtual ~ListBox();

    /*!
    * @brief Show or hide the view.
    *
    * @param isShow     [I/ ] The flag to indicate show or hide the view.
    */
    virtual void ShowView(BOOL isShow);

    /*!
    * @brief Add an item to the menu.
    * 
    * @param lpText     [I/ ] The child text.
    * 
    * @return TRUE: success / FALSE: failure.
    */
    virtual BOOL AddItem(IN LPCWSTR lpText);

    /*!
    * @brief Add an item to the menu.
    * 
    * @param uResId     [I/ ] The string resource identifier.
    * @param hInst      [I/ ] The handle to instance from which load string.
    * 
    * @return TRUE: success / FALSE: failure.
    */
    virtual BOOL AddItem(UINT32 uResId, HMODULE hInst = NULL);

    /*!
    * @brief Add an item to the menu which has special index.
    * 
    * @param lpText          [I/ ] The child text.
    * @param uIndex          [I/ ] The index.
    * 
    * @return TRUE: success / FALSE: failure.
    */
    virtual BOOL AddItem(IN LPCWSTR lpText, UINT32 uIndex);

    /*!
    * @brief Call this method delete special index item.
    * 
    * @param uIndex.        [I/ ] The item index.
    * 
    * @return True:success/False:failure.
    */
    virtual BOOL DeleteItem(UINT32 uIndex);

    /*!
    * @brief Remove all menu view
    *
    * @return True: remove success.
    *               remove failure
    */
    virtual BOOL RemoveAllItem();

    /*!
    * @brief Called to judge whether or not change children's status.
    */
    virtual void SelecteItem(UINT32 index);

    /*!
    * @brief Set the menu item text by index.
    *
    * @param index                 [I/ ] The menu item index.
    * @param lpText                [I/ ] The text.
    * 
    * @return True:success/False:failure.
    */
    virtual BOOL SetItemText(IN UINT32 index, IN LPCWSTR lpText);

    /*!
    * @brief Get the selected item's text.
    *
    * @param lpText                 [I/O] The text.
    * @param uSize                  [I/O] Buffer size.
    * 
    * @return True:success/False:failure.
    */
    virtual BOOL GetSelItemText(OUT LPWSTR lpText, IN UINT32 uSize);

    /*!
    * @brief Get selected item text length.
    */
    virtual UINT32 GetSelItemTextLength();

    /*!
    * @brief Call this method to get text According to menuItem index.
    * 
    * @param index                  [I/ ] The menuItem index.
    * @param lpText                 [I/O] The text.
    * @param uSize                  [I/O] Buffer size.
    * 
    * @return True:success/False:failure.
    */
    virtual BOOL GetItemText(IN UINT32 index, OUT LPWSTR lpText, IN UINT32 uSize);

    /*!
    * @brief Call to get menu item count.
    * 
    * @return menu item count.
    */
    virtual UINT32 GetItemCount();

    /*!
    * @brief Call this method get selected index.
    * 
    * @return selected index.
    */
    virtual INT32 GetSelIndex();

    /*!
    * @brief Call this method set index item to be selected.
    */
    virtual void SetCurSel(INT32 nIndex);

    /*!
    * @brief Set the parent of the view, generally, it is called when added to a layout.
    *
    * @param pParentView    [I/ ] The parent view.
    */
    virtual void SetParent(ViewLayout *pParentView);

    /*!
    * @brief Call this method to set item text color when has focus.
    * 
    * @param color          [I/ ] The color.
    */
    virtual void SetItemFocusColor(IN const D2D1_COLOR_F& color);

    /*!
    * @brief Set the menu event handler.
    *
    * @param pListEventHandler      [I/ ] The handler to set.
    */
    virtual void SetListEventHandler(IN ListViewEventHandler *pListEventHandler);

protected:

    /*!
    * @brief Update layout.
    */
    virtual void UpdateLayout();

    /*!
    * @brief Set the specified item focus state.
    *
    * @param nSelIndex      [I/ ] The index of item.
    * @param fAddState      [I/ ] The flag for adding or removing state.
    */
    virtual void SetItemFocusState(INT32 nSelIndex, BOOL fAddState = TRUE);

    /*!
    * @brief Called when a mouse event is dispatched to a view.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSender    [I/ ] The pointer to ViewElement specifies event's sender.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL OnMouse(const LPMSG lpMsg, ViewElement *pSender, ViewElement *pSource);

    /*!
    * @brief Called when a child is clicked.
    *
    * @param pView          [I/ ] The child which is clicked.
    */
    virtual void OnClick(ViewElement* view);

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

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
    * @brief Called to draw selected item color background in special position.
    * 
    * @param nIndex         [I/ ] The index of the item to be draw selected background.
    * @param pRenderTarget  [I/ ] The renderTarget object.
    */
    virtual void OnDrawSelBackground(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget);

private:

    FLOAT                           m_fItemMargin;          // Item margin
    INT32                           m_nTempSelIndex;        // Temp selected index.
    INT32                           m_nSelIndex;            // The focused child's Id.
    BOOL                            m_isDrawTempSelBK;      // Draw temporary selected background.
    D2DBitmap                      *m_pItemFocusBitmap;     // The focused item bitmap.
    D2DBitmap                      *m_pItemNormapBitmap;    // The focused item bitmap.
    ListViewEventHandler           *m_pListEventHandler;    // The eventHandler.
};

#endif // _LISTBOX_H_
#endif // __cplusplus
