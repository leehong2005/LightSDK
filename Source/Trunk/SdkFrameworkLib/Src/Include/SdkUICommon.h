/*!
* @file SdkUICommon.h
* 
* @brief This file defines common utility class used for framework.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/22
*/

#ifdef __cplusplus
#ifndef _SDKUICOMMON_H_
#define _SDKUICOMMON_H_


//========================================================================

//
// The Common header files
//
#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "..\..\..\SdkResourceLib\Resources\Resource.h"



//========================================================================
//
// Class declaration.
//

//
// Declaration windows classes
//
BEGIN_NAMESPACE_WINDOW
class SdkWindow;
class SdkWindowForm;
class SdkWindowDialog;
class SdkMessageBox;
END_NAMESPACE_WINDOW


//
// Declaration view classes.
//
BEGIN_NAMESPACE_VIEWS
class SdkViewElement;
class SdkViewLayout;
class SdkComboBox;
class SdkEditBox;
class SdkButton;
class SdkImageView;
class SdkListBox;
class SdkOverlapView;
class SdkProgressBar;
class SdkRadioButton;
class SdkRatingView;
class SdkScrollBar;
class SdkSeekBar;
class SdkTabHeader;
class SdkTabView;
class SdkTextView;
class SdkViewPreviewEx;
class SdkSlideBase;
class SdkImagePreviewLayout;
class SdkDataSetObserver;
END_NAMESPACE_VIEWS


//
// Declaration animation classes
//
BEGIN_NAMESPACE_ANIMATION
class SdkAnimation;
class SdkAlphaAnimation;
class SdkAnimationCom;
class SdkAnimationSet;
class SdkAnimationTimerEventHandler;
class SdkRotateAnimation;
class SdkScaleAnimation;
class SdkStoryboardEventHandler;
class SdkTranslateAnimation;
END_NAMESPACE_ANIMATION


//
// Declaration UILib classes
//
BEGIN_NAMESPACE_UILIB
class SdkApplication;
END_NAMESPACE_UILIB


//
// Declaration D2D classes.
//
BEGIN_NAMESPACE_D2D
class D2DAnimatedGif;
class D2DBitmap;
class D2DBitmapBrush;
class D2DBrush;
class D2DCustomTextRenderer;
class D2DDevice;
class D2DLinearGradientBrush;
class D2DRadialGradientBrush;
class D2DRectUtility;
class D2DResource;
class D2DSolidColorBrush;
class D2DTextLayout;
class D2DUtility;
END_NAMESPACE_D2D


//
// Declaration Theme classes
//
BEGIN_NAMESPACE_THEME
class SdkD2DTheme;
class SdkResManager;
END_NAMESPACE_THEME


//
// Declaration Event classes.
//
BEGIN_NAMESPACE_CALLBACK
class IAnimationListener;
class IAnimationTimerListener;
class ID2DDeviceStateChange;
class IComboBoxEventHandler;
class IListBoxEventHandler;
class ISlideBaseEventHandler;
class ITabHeaderEventHandler;
class ITabViewEventHandler;
class ISwitchEventHandler;
class IViewOnClickHandler;
class IViewOnKeyHandler;
class IViewOnMouseHandler;
class IRatingViewEventHandler;
class ICheckBoxEventHandler;
class IRadioButtonEventHandler;
class IProgressBarEventHandler;
END_NAMESPACE_CALLBACK


//
// Declaration D3D classes.
//
BEGIN_NAMESPACE_D3D
class D3DDevice;
END_NAMESPACE_D3D



//========================================================================

//
// Using namespaces
//
USING_NAMESPACE_WINDOW
USING_NAMESPACE_VIEWS
USING_NAMESPACE_ANIMATION
USING_NAMESPACE_D2D
USING_NAMESPACE_D3D
USING_NAMESPACE_THEME
USING_NAMESPACE_CALLBACK
USING_NAMESPACE_UILIB



//========================================================================

//
// Instance
//
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif



//========================================================================

//
// View class name
//
#define CLASSNAME_BUTTON             _T("SdkButton")
#define CLASSNAME_CHECKBOX           _T("SdkCheckBox")
#define CLASSNAME_COMBOBOX           _T("SdkComboBox")
#define CLASSNAME_EDITBOX            _T("SdkEditBox")
#define CLASSNAME_GIFVIEW            _T("SdkGifView")
#define CLASSNAME_GROUPBOX           _T("SdkGroupBox")
#define CLASSNAME_IMAGEVIEW          _T("SdkImageView")
#define CLASSNAME_LISTBOX            _T("SdkListBox")
#define CLASSNAME_OVERLAP            _T("SdkOverlapView")
#define CLASSNAME_PROGRESS           _T("SdkProgressBar")
#define CLASSNAME_RADIOBUTTON        _T("SdkRadioButton")
#define CLASSNAME_RATINGVIEW         _T("SdkRatingView")
#define CLASSNAME_SCROLLBAR          _T("SdkScrollBar")
#define CLASSNAME_SEEKBAR            _T("SdkSeekBar")
#define CLASSNAME_TABHEADER          _T("SdkTabHeader")
#define CLASSNAME_TABVIEW            _T("SdkTabView")
#define CLASSNAME_TEXTVIEW           _T("SdkTextView")
#define CLASSNAME_VIEWELEMENT        _T("SdkViewElement")
#define CLASSNAME_VIEWLAYOUT         _T("SdkViewLayout")



//========================================================================

//
// SdkWindow class name and title name
//
#define WINDOWFORMCLASSNAME         _T("WINDOWFORM")
#define WINDOWFORMWINDOWNAME        _T("SdkWindowForm")

#define MEDIAUICLASSNAME            WINDOWFORMCLASSNAME
#define MEDIAUIWINDOWNAME           _T("Media Browser")



BEGIN_NAMESPACE_VIEWS


//========================================================================
//
// The structure, enumeration and macros for SdkTextView.
//


/*!
* @brief The text dock direction.
*/
typedef enum _TEXT_DOCK_DIRECTION
{
    TEXT_DOCK_DIRECTION_DEFAULT = 0x0000,       // The default, no docking.
    TEXT_DOCK_DIRECTION_LEFT    = 0x0001,       // Dock at left
    TEXT_DOCK_DIRECTION_TOP     = 0x0002,       // Dock at top
    TEXT_DOCK_DIRECTION_RIGHT   = 0x0003,       // Dock at right
    TEXT_DOCK_DIRECTION_BOTTOM  = 0x0004,       // Dock at bottom

} TEXT_DOCK_DIRECTION;


/*!
* @brief Alignment of paragraph text along the reading direction axis relative to 
*        the leading and trailing edge of the layout box.
*/
typedef enum _TEXT_ALIGNMENT
{
    TEXT_ALIGNMENT_LEFT,                        // Align left.
    TEXT_ALIGNMENT_RIGHT,                       // Align right.
    TEXT_ALIGNMENT_CENTER                       // Align center.

} TEXT_ALIGNMENT;


/*!
* @brief Alignment of paragraph text along the flow direction axis relative to the
*        flow beginning and ending edge of the layout box.
*/
typedef enum _TEXT_PARAGRAPH_ALIGNMENT
{
    TEXT_PARAGRAPH_ALIGNMENT_TOP,               // Align top.
    TEXT_PARAGRAPH_ALIGNMENT_BOTTOM,            // Align bottom.
    TEXT_PARAGRAPH_ALIGNMENT_CENTER             // Align center.

} TEXT_PARAGRAPH_ALIGNMENT;


/*!
* @brief Overall metrics associated with text after layout.
*        All coordinates are in device independent pixels (DIPs).
*/
typedef struct _TEXT_METRICS
{
    /// <summary>
    /// Left-most point of formatted text relative to layout box
    /// (excluding any glyph overhang).
    /// </summary>
    FLOAT left;

    /// <summary>
    /// Top-most point of formatted text relative to layout box
    /// (excluding any glyph overhang).
    /// </summary>
    FLOAT top;

    /// <summary>
    /// The width of the formatted text ignoring trailing whitespace
    /// at the end of each line.
    /// </summary>
    FLOAT width;

    /// <summary>
    /// The width of the formatted text taking into account the
    /// trailing whitespace at the end of each line.
    /// </summary>
    FLOAT widthIncludingTrailingWhitespace;

    /// <summary>
    /// The height of the formatted text. The height of an empty string
    /// is determined by the size of the default font's line height.
    /// </summary>
    FLOAT height;

    /// <summary>
    /// Initial width given to the layout. Depending on whether the text
    /// was wrapped or not, it can be either larger or smaller than the
    /// text content width.
    /// </summary>
    FLOAT layoutWidth;

    /// <summary>
    /// Initial height given to the layout. Depending on the length of the
    /// text, it may be larger or smaller than the text content height.
    /// </summary>
    FLOAT layoutHeight;

    /// <summary>
    /// The maximum reordering count of any line of text, used
    /// to calculate the most number of hit-testing boxes needed.
    /// If the layout has no bidirectional text or no text at all,
    /// the minimum level is 1.
    /// </summary>
    UINT32 maxBidiReorderingDepth;

    /// <summary>
    /// Total number of lines.
    /// </summary>
    UINT32 lineCount;

} TEXT_METRICS, *LPTEXT_METRICS;


//========================================================================
//
// The structure, enumeration and macros for SdkOverlapView.
//


/*!
* @brief The style for overlap view.
*/
typedef enum _OVERLAP_STYLE
{
    OVERLAP_STYLE_NONE                      = 0,    // The overlap view none style.
    OVERLAP_STYLE_FIRST                     = 1,    // The overlap view first default style.
    OVERLAP_STYLE_SECOND                    = 2,    // The overlap view second default style.
    OVERLAP_STYLE_THIRD                     = 4,    // The overlap view third default style.

} OVERLAP_STYLE;


/*!
* @brief The matrix information of the overlap bitmap.
*/
typedef struct _OVERLAP_TRANSFORMINFO
{
    FLOAT               dx;                         // Translate x value.
    FLOAT               dy;                         // Translate y value.
    FLOAT               sx;                         // Scale x value.
    FLOAT               sy;                         // Scale y value.
    FLOAT               skewx;                      // Skew x value.
    FLOAT               skewy;                      // Skew y value.
    FLOAT               angle;                      // Rotation angle.
    FLOAT               opacity;                    // Opacity.
    D2D1_POINT_2F       scaleCenterPt;              // Scale center point.
    D2D1_POINT_2F       rotateCenterPt;             // Rotation center point.
    D2D1_POINT_2F       skewCenterPt;               // Skew center point.
    D2D1_MATRIX_3X2_F   matrixTranform;             // Transformation matrix;

} OVERLAP_TRANSFORMINFO, *LPOVERLAP_TRANSFORMINFO;


//========================================================================
//
// The structure, enumeration and macros for SdkTabHeader.
//


/*!
* @brief The attributes of a tab item, it is used by SdkTabHeader view.
*/
typedef struct _TAB_INFO
{
    BOOL        isSelected;                         // Indicated the tab is selected or not.
    int         nMinWidth;                          // The minimum width.
    int         cchTextMax;                         // The maximum size of text.
    int         cchTipTextMax;                      // The maximum size of tool tip text.
    LPTSTR      lpszText;                           // The displaying text.
    LPTSTR      lpszToolTip;                        // The tool tip text.
    LONG_PTR    dwData;                             // The associated data.

} TAB_INFO, *LPTAB_INFO;


/*!
* @brief The enumeration for tab dock direction.
*/
typedef enum _TAB_DOCK_DIR
{
    TAB_DOCK_NONE       = 0,                        // None dock.
    TAB_DOCK_TOP        = 1,                        // The top dock direction, (default).
    TAB_DOCK_LEFT       = 2,                        // The left dock direction.
    TAB_DOCK_RIGHT      = 3,                        // The right dock direction.
    TAB_DOCK_BOTTOM     = 4,                        // The bottom dock direction.

} TAB_DOCK_DIR;


//========================================================================
//
// The structure, enumeration and macros for Image view.
//


/*!
* @brief Image stretch mode.
*/
typedef enum _IMAGE_STRETCH_MODE
{
    IMAGE_STRETCH_MODE_FILL         = 0,            // The image fill whole view.
    IMAGE_STRETCH_MODE_CENTER       = 1,            // The image is in center.
    IMAGE_STRETCH_MODE_MARGIN       = 2,            // The image can set margin with background.
    IMAGE_STRETCH_MODE_FIT          = 3,            // The image can keep original aspect ratio.
    IMAGE_STRETCH_MODE_FIT_TOP      = 4,            // The image located at the top of view.
    IMAGE_STRETCH_MODE_FIT_LEFT     = 5,            // The image located at the left of view.
    IMAGE_STRETCH_MODE_FIT_RIGHT    = 6,            // The image located at the right of view.
    IMAGE_STRETCH_MODE_FIT_BOTTOM   = 7,            // The image located at the bottom of view.

} IMAGE_STRETCH_MODE;


END_NAMESPACE_VIEWS


//========================================================================
//
// The structure, enumeration and macros for D2D Device.
//

BEGIN_NAMESPACE_D2D

/*!
* @brief The enumeration value of device state.
*/
typedef enum _DEVICE_STATECHANGE_VALUE
{
    DEVICE_STATECHANGE_VALUE_NONE       = 0,        // N/A.
    DEVICE_STATECHANGE_VALUE_RESIZE     = 1,        // Device resize.
    DEVICE_STATECHANGE_VALUE_RECREATE   = 2,        // Device recreate.
    DEVICE_STATECHANGE_VALUE_CHANGE     = 3,        // Device change.

} DEVICE_STATECHANGE_VALUE;


/*!
* @brief The enumeration value of device state.
*/
typedef enum _DEVICE_TARGET_TYPE
{
    DEVICE_TARGET_TYPE_NONE             = 0,        // Unknown target type.
    DEVICE_TARGET_TYPE_HWND             = 1,        // HWND target.
    DEVICE_TARGET_TYPE_MEMORY           = 2,        // Bitmap target.
    DEVICE_TARGET_TYPE_DC               = 3,        // DC target.

} DEVICE_TARGET_TYPE;

END_NAMESPACE_D2D

#endif // _SDKUICOMMON_H_
#endif // __cplusplus
