/*!
* @file SdkD2DTheme.h
* 
* @brief This file defines SdkD2DTheme class for managing the drawing for views.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/22
*/

#ifdef __cplusplus
#ifndef _SDKD2DTHEME_H_
#define _SDKD2DTHEME_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_THEME

/*!
* @brief The SdkD2DTheme class.
*/
class SdkD2DTheme
{
public:

    /*!
    * @brief Create the instance of D2D theme.
    *
    * @return TRUE if succeeds, otherwise FALSE.
    */
    static BOOL CreateD2DThemeInstance();

    /*!
    * @brief Create or get the instance of SdkD2DTheme class.
    *
    * @return The pointer to SdkD2DTheme class.
    */
    static SdkD2DTheme* GetD2DThemeInstance();

    /*!
    * @brief Delete the instance of SdkD2DTheme class. Typically it is called when the application
    *        is being exited.
    */
    static void DeleteD2DThemeInstance();

public:

    virtual void OnPushClip(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        ID2D1Layer *pLayer,
        FLOAT fRadiusX = 0,
        FLOAT fRadiusY = 0);

    virtual void OnPopClip(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT);

    virtual void OnDrawBackground(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBrush  *pBrush,
        const D2D1_RECT_F& rc,
        FLOAT fRadiusX = 0,
        FLOAT fRadiusY = 0);

    virtual void OnDrawBackground(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawBorder(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBrush *pBorderBrush,
        const D2D1_RECT_F& rc,
        FLOAT fBorderW = 1.0f,
        FLOAT fRadiusX = 0,
        FLOAT fRadiusY = 0);

    virtual void OnDrawBitmap(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawOverlayBitmap(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawButton(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawComboBox(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawRadioButton(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        D2DBrush *pBrush,
        BOOL isChecked,
        BOOL isEnable,
        const D2D1_RECT_F& rc);

    virtual void OnDrawRadioButton(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawCheckBox(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawProgressBar(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawProgressBarThumb(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        FLOAT fCurPercent,
        const D2D1_RECT_F& rc);

    virtual void OnDrawRatingView(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        FLOAT fBitmapSize,
        const D2D1_RECT_F& rc);

    virtual void OnDrawTabViewTab(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        const D2D1_RECT_F& rc);

    virtual void OnDrawTabViewSingleRoundCorner(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        BOOL fLeftCorner,
        const D2D1_RECT_F& rc);

private:

    virtual void OnDrawButtonBitmap(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        ID2D1Bitmap *pD2DBitmap,
        FLOAT destX,
        FLOAT destY,
        FLOAT destWidth,
        FLOAT destHeight,
        FLOAT srcX,
        FLOAT srcY,
        FLOAT srcWidth,
        FLOAT srcHeight);

private:

    static SdkD2DTheme      *s_pD2DTheme;       // The pointer to SdkD2DTheme.
};

END_NAMESPACE_THEME

#endif // _SDKD2DTHEME_H_
#endif // __cplusplus
