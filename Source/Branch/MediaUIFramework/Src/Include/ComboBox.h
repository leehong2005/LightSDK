/*!
* @file ComboBox.h
* 
* @brief This file defines the ComboBox class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/02/11
*/

#ifdef __cplusplus
#ifndef _COMBOBOX_H_
#define _COMBOBOX_H_

#include "TextView.h"
#include "ImageButton.h"
#include "ListBox.h"
#include "ViewLayout.h"
#include "ListViewEventHandler.h"
#include "ComboBoxEventHandler.h"
#include "ViewOnClickHandler.h"

/*!
* @brief The ComboBox class.
*/
class CLASS_DECLSPEC ComboBox : public ViewLayout, 
                                public ViewOnClickHandler,
                                public ListViewEventHandler
{
public:

    /*!
    * @brief The constructor function.
    */
    ComboBox();

    /*!
    * @brief The destructor function.
    */
    virtual ~ComboBox();

    /*!
    * @brief Add an item to the menu.
    *
    * @param sItemText      [I/ ] The child text.
    */
    virtual void AddItem(IN LPCWSTR lpItemText);

    /*!
    * @brief Add an item to the menu.
    * 
    * @param uResId     [I/ ] The string resource identifier.
    * @param hInst      [I/ ] The handle to instance from which load string.
    */
    virtual void AddItem(UINT32 uResId, HMODULE hInst = NULL);

    /*!
    * @brief Add an item to the menu which has special index.
    * 
    * @param lpItemText      [I/ ] The child text.
    * @param uIndex          [I/ ] The index.
    * 
    * @return TRUE: success / FALSE: failure.
    */
    virtual void AddItem(IN LPCWSTR lpItemText, UINT32 uIndex);

    /*!
    * @brief Call this method delete special index item.
    * 
    * @param uIndex.        [I/ ] The item index.
    */
    virtual void DeleteItem(UINT32 uIndex);

    /*!
    * @brief Remove all menu view
    *
    * @return True: remove success.
    *               remove failure
    */
    virtual void RemoveAllItem();

    /*!
    * @brief Called to set text to comboBox.
    * 
    * @param lpText     [I/ ] The text.
    */
    virtual void SetComboBoxText(IN LPWSTR lpText);

    /*!
    * @brief Set the menu item text by index.
    *
    * @param menuItemIndex         [I/ ] The menu item index.
    * @param lpText                [I/ ] The text.
    */
    virtual void SetItemText(IN INT32 menuItemIndex, IN LPWSTR lpText);

    /*!
    * @brief Set the menu item text by index.
    *
    * @param menuItemIndex         [I/ ] The menu item index.
    * @param uResId     [I/ ] The string resource identifier.
    * @param hInst      [I/ ] The handle to instance from which load string.
    */
    virtual void SetItemText(IN INT32 menuItemIndex, IN UINT32 uResId, HMODULE hInst = NULL);

    /*!
    * @brief Call this method set index item to be selected.
    */
    virtual void SetCurSel(INT32 nIndex);

    /*!
    * @brief Call this method get selected index.
    * 
    * @return selected index.
    */
    virtual INT32 GetCurSel();

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
    * @brief Show or hide list view of combobox that has pop-up property.
    *
    * @param isShow     [I/ ] TRUE if show, FALSE if hide.
    */
    virtual void ShowPopupView(BOOL isShow = TRUE);

    /*!
    * @brief Set the comboBox event handler.
    * 
    * @param pComboBoxEventHadler      [I/ ] The handler to set.
    */
    virtual void SetComboBoxEventHandler(IN ComboBoxEventHandler *pComboBoxEventHandler);

protected:

    /*!
    * @brief Initialize comboBox. It must be called.
    */
    virtual void Initialize();

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
    * @brief Called when view item is clicked.
    * 
    * @param view   [I/ ] The clicked ViewElement.
    */
    virtual void OnClick(ViewElement* view);

    /*!
    * @brief Called when selected item is changed.
    * 
    * @param pView  [I/ ] The event source.
    */
    virtual void OnSelectedChanged(ListBox *pView);

    /*!
    * @brief Called when visibility is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnVisibleChanged(ListBox *pView);

private:

    BOOL                     m_isDeleteList;            // Indicates whether delete list.
    TextView                *m_pTextView;               // The view of show text.
    ImageButton             *m_pImgBtn;                 // The created ImageButton object.
    ListBox                 *m_pMenuList;               // THe MenuList add to comboBox.
    ComboBoxEventHandler    *m_pComboBoxEventHandler;   // The eventHandler.
};

#endif // _COMBOBOX_H_
#endif // __cplusplus

