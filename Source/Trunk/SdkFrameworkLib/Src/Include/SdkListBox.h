/*!
* @file MenuList.h
* 
* @brief This file defines the MenuList class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/14
*/

#ifdef __cplusplus
#ifndef _SDKLISTBOX_H_
#define _SDKLISTBOX_H_

#include "SdkViewLayout.h"

BEGIN_NAMESPACE_VIEWS

class CLASS_DECLSPEC SdkListBox : public SdkViewLayout,
                                  public IViewOnClickHandler,
                                  public IViewOnMouseHandler
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkListBox();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkListBox();

    /*!
    * @brief Show or hide the view.
    *
    * @param isShow     [I/ ] The flag to indicate show or hide the view.
    */
    virtual void ShowView(BOOL isShow);

    /*!
    * @brief Enable or disable pop-up property.
    *
    * @param isPopup  [I/ ] TRUE if enable, otherwise return FALSE.
    */
    virtual void SetPopup(BOOL isPopup);

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
    virtual BOOL RemoveAllItems();

    /*!
    * @brief Called to judge whether or not change children's status.
    *
    * @param nIndex     [I/ ] Specifies the zero-based index of the item to select,
    *                         if this parameter is -1, any current selection in the list
    *                         will be selected.
    */
    virtual void SelecteItem(int nIndex);

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
    virtual BOOL GetSelText(OUT LPWSTR lpText, IN UINT32 uSize);

    /*!
    * @brief Get selected item text length.
    */
    virtual UINT32 GetSelTextLength();

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
    virtual void SetParent(SdkViewLayout *pParentView);

    /*!
    * @brief Set the menu event handler.
    *
    * @param pListEventHandler      [I/ ] The handler to set.
    */
    virtual void SetListBoxEventHandler(IN IListBoxEventHandler *pListEventHandler);

protected:

    /*!
    * @brief Update layout.
    */
    virtual void UpdateLayout();

    /*!
    * @brief Called when a mouse event is dispatched to a view.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSender    [I/ ] The pointer to SdkViewElement specifies event's sender.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL OnMouse(const LPMSG lpMsg, SdkViewElement *pSender, SdkViewElement *pSource);

    /*!
    * @brief Called when a child is clicked.
    *
    * @param pView          [I/ ] The child which is clicked.
    */
    virtual void OnClick(SdkViewElement* view);

    /*!
    * @brief Called when a view has been clicked and held.
    *
    * @param pView      [I/ ] The event source.
    *
    * @return TRUE if consumed the long click, FALSE otherwise.
    */
    virtual BOOL OnLongClick(SdkViewElement *pView);

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
    * @brief Called to draw items.
    * 
    * @param pRenderTarget  [I/ ] The renderTarget object.
    */
    virtual void OnDrawAllItems(IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Called to draw selected item color background in special position.
    * 
    * @param nIndex         [I/ ] The index of the item to be draw selected background.
    * @param pRenderTarget  [I/ ] The renderTarget object.
    */
    virtual void OnDrawSelectedBk(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Called to draw hover item color background in special position.
    * 
    * @param nIndex         [I/ ] The index of the item to be draw selected background.
    * @param pRenderTarget  [I/ ] The renderTarget object.
    */
    virtual void OnDrawHoverBk(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Called to draw pressed item color background in special position.
    * 
    * @param nIndex         [I/ ] The index of the item to be draw selected background.
    * @param pRenderTarget  [I/ ] The renderTarget object.
    */
    virtual void OnDrawPressBk(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget);

private:

    /*!
    * @brief The internal data of list box.
    */
    struct _LISTBOX_INTERNALDATA;

    _LISTBOX_INTERNALDATA      *m_pListBoxData;     // The list box's data.
};

END_NAMESPACE_VIEWS

#endif // _SDKLISTBOX_H_
#endif // __cplusplus
