/*!
* @file ImageButtonEx.h
* 
* @brief This file defines the ImageButton extension class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang & Li Hong
* @date 2011/01/12
*/

#ifdef __cplusplus
#ifndef _IMAGEBUTTONEX_H_
#define _IMAGEBUTTONEX_H_

#include "ImageButton.h"

/*!
* @brief The ImageButton extension class,implement special OnMouseEvent method.
*/
class CLASS_DECLSPEC ImageButtonEx : public ImageButton
{
public:

    /*!
    * @brief The constructor function.
    *
    * @param isShowDefBK    [I/ ] Indicates whether show default background image or not.
    */
    ImageButtonEx(BOOL isShowDefBK = FALSE);

    /*!
    * @brief The destructor function.
    */
    virtual ~ImageButtonEx();

    /*!
    * @brief Call this method to make ImageButton keep focus state.
    * 
    * @param keepFocus      [I/ ] While TRUE keep focus state, otherwise normal state.
    */
    void KeepFocusState(IN BOOL keepFocus);

    /*!
    * @brief Set the flag to indicates whether remove focus stated automatically when click.
    * 
    * @param keepFocus      [I/ ] While TRUE keep focus state, otherwise normal state.
    */
    void AutoRemoveFocusState(IN BOOL autoRemoveFocus);

    /*!
    * @brief Call this method to set ImageButton on focus state.
    */
    void SetFocusState();

    /*!
    * @brief Call this method to remove focus state.
    */
    void RemoveFocusState();

protected:

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

private:

    BOOL        m_isAutoRemoveFocus;            // Indicates whether remove focus automatically.
    BOOL        m_isKeepFocus;                  // The boolean of has or not on focus state.
    BOOL        m_isPreSelected;                // The focus state of view.Such as TRUE or FALSE.
};


#endif // _IMAGEBUTTONEX_H_
#endif // __cplusplus
