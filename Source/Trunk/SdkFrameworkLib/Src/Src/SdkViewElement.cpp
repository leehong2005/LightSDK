/*!
* @file SdkViewElement.cpp
* 
* @brief This file defines the base view class for various visual elements.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "stdafx.h"
#include "SdkViewElement.h"
#include "SdkViewLayout.h"
#include "SdkD2DTheme.h"
#include "D2DSolidColorBrush.h"

USING_NAMESPACE_VIEWS

BEGIN_NAMESPACE_VIEWS

#define MAX_BORDER_WIDTH            4.0f
#define MOUSE_MOVE_RANGE            5

/*!
* @brief The view transform information.
*/
typedef struct _VIEW_TRANSFORMINFO
{
    FLOAT           dx;                     // Translate x value.
    FLOAT           dy;                     // Translate y value.
    FLOAT           sx;                     // Scale x value.
    FLOAT           sy;                     // Scale y value.
    FLOAT           angle;                  // Rotation angle.
    POINTF          scaleCenterPt;          // Scale center point.
    POINTF          rotateCenterPt;         // Rotation center point.

} VIEW_TRANSFORMINFO, *LPVIEW_TRANSFORMINFO;

END_NAMESPACE_VIEWS


/*!
* @brief The internal data for the SdkViewElement class.
*/
struct NAMESPACE_VIEWS::_INTERNALDATA
{
    FLOAT                    m_fBorderWidth;             // The border width.
    FLOAT                    m_fRadiusX;                 // Round corner radius of x.
    FLOAT                    m_fRadiusY;                 // Round corner radius of y.
    POINT                    m_preMousePt;               // The previous mouse point.
    INT32                    m_nViewIndex;               // The view's index in view container.
    INT32                    m_nViewId;                  // The identifier of current view.
    INT32                    m_nViewState;               // The current state of view.
    BOOL                     m_hasTSRMatrix;             // Indicates whether has T, S, R matrix.
    BOOL                     m_isFocused;                // Indicates focused or not.
    PVOID                    m_pTag;                     // The pointer to view tag.
    LPCTSTR                  m_lpClassName;              // The class name, do not delete the point.
    D2D1_COLOR_F             m_bkColor;                  // The color of background.
    D2D1_COLOR_F             m_borderColor;              // The border color.
    VIEW_TRANSFORMINFO       m_transformInfo;            // The transform data.
    Matrix3x2F               m_viewMatrix;               // The matrix of view.
    VIEW_STYLE               m_viewStyle;                // The view style.
    ID2D1Layer              *m_pClipLayer;               // The clip layer.
    D2DBrush                *m_pD2DBrush;                // The background brush;
    D2DBrush                *m_pBorderBrush;             // The border color brush.
    D2DBitmap               *m_pBKD2DBitmap;             // The pointer which points to the object of D2DBitmap.
    SdkViewLayout           *m_pParentView;              // The parent view, do NOT release it.
    SdkAnimation            *m_pAnimation;               // The animation to be started.
    IViewOnMouseHandler     *m_pViewMouseHandler;        // The mouse event handler.
    IViewOnKeyHandler       *m_pViewKeyHandler;          // The key event handler.
    IViewOnClickHandler     *m_pClickHandler;            // The click handler.
};


//////////////////////////////////////////////////////////////////////////

SdkViewElement::SdkViewElement() : m_pWindow(NULL),
                                   m_pInternalData(new INTERNALDATA())
{
    ZeroMemory(m_pInternalData, sizeof(INTERNALDATA));

    m_pInternalData->m_viewMatrix           = Matrix3x2F::Identity();
    m_pInternalData->m_pD2DBrush            = NULL;
    m_pInternalData->m_pBorderBrush         = new D2DSolidColorBrush();
    m_pInternalData->m_bkColor              = D2D1::ColorF(ColorF::Black);
    m_pInternalData->m_borderColor          = D2D1::ColorF(ColorF::Gray);
    m_pInternalData->m_nViewState           = VIEW_STATE_ENABLE | VIEW_STATE_VISIBLE;
    m_pInternalData->m_viewStyle            = VIEW_STYLE_NORMAL;
    m_pInternalData->m_hasTSRMatrix         = FALSE;
    m_pInternalData->m_isFocused            = FALSE;
    m_pInternalData->m_nViewId              = -1;
    m_pInternalData->m_fBorderWidth         = -1.0f;
    m_pInternalData->m_fRadiusX             = 0.0f;
    m_pInternalData->m_fRadiusY             = 0.0f;
    m_pInternalData->m_preMousePt.x         = 0;
    m_pInternalData->m_preMousePt.y         = 0;
    m_pInternalData->m_transformInfo.sx     = 1.0f;
    m_pInternalData->m_transformInfo.sy     = 1.0f;

    m_pInternalData->m_pBorderBrush->SetColor(D2D1::ColorF(ColorF::Black));

    ZeroMemory(&m_layoutInfo, sizeof(LAYOUTINFO));

    m_layoutInfo.width      = 100.0f;
    m_layoutInfo.height     = 100.0f;
    m_layoutInfo.minWidth   = 1.0f;
    m_layoutInfo.minHeight  = 1.0f;
    m_layoutInfo.maxHeight  = 2500.0f * 20;
    m_layoutInfo.maxWidth   = 2500.0f * 20;

    SetClassName(CLASSNAME_VIEWELEMENT);
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement::~SdkViewElement()
{
    SAFE_DELETE(m_pInternalData->m_pD2DBrush);
    SAFE_DELETE(m_pInternalData->m_pBorderBrush);
    SAFE_DELETE(m_pInternalData->m_pBKD2DBitmap);
    SAFE_RELEASE(m_pInternalData->m_pClipLayer);

    SAFE_DELETE(m_pInternalData);

    if (NULL != m_pWindow)
    {
        D2DDevice *pD2DDevice = m_pWindow->GetD2DDevices();
        if (NULL != pD2DDevice)
        {
            pD2DDevice->UnregisterD2DDeviceNotification(this);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnPaint()
{
    if ( !IsVisible() || (NULL == m_pWindow) )
    {
        return;
    }

    D2DDevice *pD2DDevice = m_pWindow->GetD2DDevices();
    if ( NULL == pD2DDevice )
    {
        return;
    }

    ID2D1RenderTarget *pRenderTarget = NULL;
    pD2DDevice->GetRenderTarget(&pRenderTarget);

    Matrix3x2F animMatrix = Matrix3x2F::Identity();
    // Set the matrix to render target got from animation.
    if ( IsAnimMatrixEnable() && (NULL != m_pInternalData->m_pAnimation) )
    {
        TRANSFORMINFO info;
        if ( SUCCEEDED(m_pInternalData->m_pAnimation->GetTransform(&info)) )
        {
            animMatrix = *(Matrix3x2F::ReinterpretBaseType(&(info.matrixTransform)));
        }
    }

    // Combine the T, S, and R matrix;
    CombineTSRMatrix();
    Matrix3x2F absoluteMatrix = GetAbsoluteMatrix();
    absoluteMatrix = absoluteMatrix * animMatrix;
    pRenderTarget->SetTransform(absoluteMatrix);

    PushClip(pRenderTarget);
    // All drawing operation should be finished in this virtual method.
    OnDrawItem(pRenderTarget);
    PopClip(pRenderTarget);

    // After drawing, set identity matrix to the target.
    pRenderTarget->SetTransform(Matrix3x2F::Identity());

    // If the window on which view located is NOT layered window, we use paint to drive animation,
    // If the window is layered window, we use animation timer to drive animation.
    if ( !m_pWindow->IsLayeredWindow() &&
         (NULL != m_pInternalData->m_pAnimation) &&
         IsAnimMatrixEnable() )
    {
        // If the animation does not finish, go on to invalidate window.
        BOOL isFinish = FALSE;
        if ( SUCCEEDED(m_pInternalData->m_pAnimation->IsFinish(&isFinish)) )
        {
            if ( isFinish )
            {
                BOOL isClearAnim = ((GetState() & VIEW_STATE_CLEARANIMATION));
                if ( isClearAnim )
                {
                    ClearAnimation();
                }
            }
            else
            {
                ForceInvalidate();
            }
        }
    }

    SAFE_RELEASE(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F absRc = { 0, 0 };
    GetAbsoluteRect(absRc);

    // Draw view's background.
    OnDrawBackground(pRenderTarget, absRc);

    // Draw view's border.
    OnDrawBoarder(pRenderTarget, absRc);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::Invalidate(BOOL isUpdateNow)
{
    if (NULL != m_pWindow)
    {
        if (isUpdateNow)
        {
            m_pWindow->Invalidate(TRUE);
        }
        else
        {
            m_pWindow->AddWindowState(WINDOW_STATE_INVALIDATE);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    height = GET_RIGHT_VALUE(height, m_layoutInfo.maxHeight, m_layoutInfo.minHeight);
    width  = GET_RIGHT_VALUE(width,  m_layoutInfo.maxWidth,  m_layoutInfo.minWidth);

    BOOL fChanged = FALSE;
    fChanged = (m_layoutInfo.x != x) || (m_layoutInfo.y != y) ||
               (m_layoutInfo.width != width) || (m_layoutInfo.height != height);

    m_layoutInfo.x      = x;
    m_layoutInfo.y      = y;
    m_layoutInfo.height = height;
    m_layoutInfo.width  = width;

    // Call this method to give notification to sub class.
    OnLayout(fChanged, x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetMaxWidth(FLOAT maxWidth)
{
    m_layoutInfo.maxWidth = maxWidth;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetMaxHeight(FLOAT maxHeight)
{
    m_layoutInfo.maxHeight = maxHeight;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetMinWidth(FLOAT minWidth)
{
    m_layoutInfo.minWidth = minWidth;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetMinHeight(FLOAT minHeight)
{
    m_layoutInfo.minHeight = minHeight;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::GetLayoutInfo(OUT LAYOUTINFO* pLayoutInfo) const
{
    if (NULL != pLayoutInfo)
    {
        *pLayoutInfo = m_layoutInfo;
    }
}

//////////////////////////////////////////////////////////////////////////

SdkWindow* SdkViewElement::GetWindow()
{
    return m_pWindow;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetWindow(SdkWindow *pWindow)
{
    if ( (NULL != pWindow) && (pWindow != m_pWindow) )
    {
        m_pWindow = pWindow;
        D2DDevice *pD2DDevice = m_pWindow->GetD2DDevices();
        if (NULL != pD2DDevice)
        {
            pD2DDevice->RegisterD2DDeviceNotification(this);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::ShowView(BOOL isShow)
{
    if ( isShow != IsVisible() )
    {
        SetVisible(isShow);
        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetOnMouseHandler(IN IViewOnMouseHandler *pViewMouseHandler)
{
    m_pInternalData->m_pViewMouseHandler = pViewMouseHandler;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetOnKeyHandler(IN IViewOnKeyHandler *pViewKeyHandler)
{
    m_pInternalData->m_pViewKeyHandler = pViewKeyHandler;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetOnClickHandler(IN IViewOnClickHandler *pClickHandler)
{
    m_pInternalData->m_pClickHandler = pClickHandler;

    SetClickable(NULL != pClickHandler);
    SetLongClickable(NULL != pClickHandler);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetBkColor(const ColorF &color)
{
    if ( NULL == m_pInternalData->m_pD2DBrush )
    {
        m_pInternalData->m_pD2DBrush = new D2DSolidColorBrush();
    }

    m_pInternalData->m_bkColor = color;
    m_pInternalData->m_pD2DBrush->SetColor(color);
}

//////////////////////////////////////////////////////////////////////////

ColorF SdkViewElement::GetBkColor()
{
    D2D1_COLOR_F color = m_pInternalData->m_bkColor;

    return ColorF(color.r, color.g, color.b, color.a);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::SetBkImage(IN UINT32 uResId, HINSTANCE hInstance, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if ( NULL == m_pInternalData->m_pBKD2DBitmap )
    {
        m_pInternalData->m_pBKD2DBitmap = new D2DBitmap();
    }

    BOOL isSuccess = m_pInternalData->m_pBKD2DBitmap->LoadFromResource(uResId, hInstance, uDestWidth, uDestHeight);
    if ( isSuccess )
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }
    else
    {
        RemoveFlag(VIEW_STATE_CLIPVIEW);
        SAFE_DELETE(m_pInternalData->m_pBKD2DBitmap);
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::SetBkImage(IN HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if (NULL == m_pInternalData->m_pBKD2DBitmap)
    {
        m_pInternalData->m_pBKD2DBitmap = new D2DBitmap();
    }

    BOOL isSuccess = m_pInternalData->m_pBKD2DBitmap->LoadFromHBITMAP(hBitmap, uDestWidth, uDestHeight);
    if ( isSuccess )
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }
    else
    {
        RemoveFlag(VIEW_STATE_CLIPVIEW);
        SAFE_DELETE(m_pInternalData->m_pBKD2DBitmap);
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::SetBkImage(IN LPCWSTR lpfileName, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if ( NULL == m_pInternalData->m_pBKD2DBitmap )
    {
        m_pInternalData->m_pBKD2DBitmap = new D2DBitmap();
    }

    BOOL isSuccess = m_pInternalData->m_pBKD2DBitmap->LoadFromFile(lpfileName, uDestWidth, uDestHeight);
    if ( isSuccess )
    {
        AddFlag(VIEW_STATE_CLIPVIEW);
    }
    else
    {
        RemoveFlag(VIEW_STATE_CLIPVIEW);
        SAFE_DELETE(m_pInternalData->m_pBKD2DBitmap);
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetBkBrush(IN D2DBrush *pBKBrush)
{
    SAFE_DELETE(m_pInternalData->m_pD2DBrush);
    m_pInternalData->m_pD2DBrush = pBKBrush;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetAnimation(SdkAnimation *pAnimation)
{
    m_pInternalData->m_pAnimation = pAnimation;
}

//////////////////////////////////////////////////////////////////////////

SdkAnimation* SdkViewElement::GetAnimation() const
{
    return m_pInternalData->m_pAnimation;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::StartAnimation(DOUBLE dDelaySecond)
{
    if (NULL != m_pInternalData->m_pAnimation)
    {
        m_pInternalData->m_pAnimation->Start(dDelaySecond);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::ClearAnimation()
{
    m_pInternalData->m_pAnimation = NULL;
    ForceInvalidate();
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetParent(SdkViewLayout *pParentView)
{
    m_pInternalData->m_pParentView = pParentView;
}

//////////////////////////////////////////////////////////////////////////

SdkViewLayout* SdkViewElement::GetParent() const
{
    return m_pInternalData->m_pParentView;
}

//////////////////////////////////////////////////////////////////////////

LPCTSTR SdkViewElement::GetClassName()
{
    return m_pInternalData->m_lpClassName;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SdkViewElement::FindEventSource(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);

    if (IsEnable() && IsVisible())
    {
        FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
        FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
        BOOL isTouchOn = IsPtInRect(xPos, yPos);
        if ( isTouchOn )
        {
            // Set the event source and target.
            if (NULL != m_pWindow)
            {
                // Save the target when the message is WM_LBUTTONDOWN.
                if ( WM_LBUTTONDOWN == message )
                {
                    m_pWindow->WindowViews.EventTargetView = this;
                }
            }
        }

        return (isTouchOn ? this : NULL);
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::DispatchKeyboardEvent(const LPMSG lpMsg)
{
    BOOL handled = PerformOnKeyboardEvent(lpMsg, this);

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::DispatchMouseEvent(const LPMSG lpMsg)
{
    BOOL handled = FALSE;
    SdkViewElement *pSource = this;

    // At this time, the pSource may be invisible although mouse left button has down on it,
    // so that the view can NOT receive message, such as WM_LBUTTONUP, we should make sure 
    // the WM_LBUTTONDOWN match WM_LBUTTONUP, so always perform mouse event.
    BOOL isVisible = pSource->IsVisible();
    pSource->SetVisible(TRUE);
    handled = pSource->PerformOnMouseEvent(lpMsg, pSource);
    pSource->SetVisible(isVisible);

    if ( !handled )
    {
        SdkViewElement *pParentView = pSource->GetParent();
        while (NULL != pParentView)
        {
            handled = pParentView->PerformOnMouseEvent(lpMsg, pSource);
            if ( handled )
            {
                break;
            }

            pParentView = pParentView->GetParent();
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::PreDispatchMouseEvent(const LPMSG lpMsg, SdkViewElement *pSource)
{
    BOOL handled = PerformOnMouseEvent(lpMsg, pSource);

    return handled;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::ClearAssocData()
{
    SAFE_DELETE(m_pInternalData->m_pBKD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsClickable()
{
    return (VIEW_STATE_CLICKABLE == (m_pInternalData->m_nViewState & VIEW_STATE_CLICKABLE));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsLongClickable()
{
    return (VIEW_STATE_LONGCLICKABLE == (m_pInternalData->m_nViewState & VIEW_STATE_LONGCLICKABLE));
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetClickable(BOOL isClickable)
{
    if (isClickable)
    {
        AddFlag(VIEW_STATE_CLICKABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_CLICKABLE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetLongClickable(BOOL isLongClickable)
{
    if ( isLongClickable )
    {
        AddFlag(VIEW_STATE_LONGCLICKABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_LONGCLICKABLE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetFocusable(BOOL isFocusable)
{
    if ( isFocusable )
    {
        AddFlag(VIEW_STATE_FOCUSABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_FOCUSABLE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetSelected(BOOL isSelected)
{
    BOOL isCurSelected = IsSelected();

    if ( isSelected )
    {
        AddFlag(VIEW_STATE_SELECTED);
    }
    else
    {
        RemoveFlag(VIEW_STATE_SELECTED);
    }

    if ( isSelected != isCurSelected )
    {
        OnSelectChenaged(this, isSelected);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetEnable(BOOL isEnable)
{
    if ( isEnable )
    {
        AddFlag(VIEW_STATE_ENABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_ENABLE);
    }

    if ( isEnable )
    {
        m_pInternalData->m_viewStyle = IsSelected() ? VIEW_STYLE_SELECTED : VIEW_STYLE_NORMAL;
    }
    else
    {
        m_pInternalData->m_viewStyle = VIEW_STYLE_DISABLE;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetVisible(BOOL isVisible)
{
    BOOL curVisible = IsVisible();

    if (isVisible)
    {
        AddFlag(VIEW_STATE_VISIBLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_VISIBLE);
    }

    if (curVisible!= isVisible)
    {
        OnVisibilityChanged(this, isVisible);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetFocus()
{
    if ( IsFocusable() )
    {
        SdkWindow *pWindow = GetWindow();
        if ( NULL != pWindow )
        {
            SdkViewElement *pOldFocusView = pWindow->WindowViews.CurrentFocusView;

            if ( pOldFocusView != this )
            {
                // Set the current focus view to the current view (this).
                pWindow->WindowViews.CurrentFocusView = this;

                if ( NULL != pOldFocusView )
                {
                    pOldFocusView->m_pInternalData->m_isFocused = FALSE;
                    pOldFocusView->OnFocusChanged(pOldFocusView, FALSE);
                }

                // Set the flag to TRUE.
                m_pInternalData->m_isFocused = TRUE;
                // Notify the current view that it has gained focus.
                OnFocusChanged(pOldFocusView, TRUE);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsFocused()
{
    return m_pInternalData->m_isFocused;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsSelected()
{
    return (VIEW_STATE_SELECTED == (m_pInternalData->m_nViewState & VIEW_STATE_SELECTED));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsFocusable()
{
    return (VIEW_STATE_FOCUSABLE == (m_pInternalData->m_nViewState & VIEW_STATE_FOCUSABLE));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsEnable()
{
    return (VIEW_STATE_ENABLE == (m_pInternalData->m_nViewState & VIEW_STATE_ENABLE));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsVisible()
{
    return (VIEW_STATE_VISIBLE == (m_pInternalData->m_nViewState & VIEW_STATE_VISIBLE));
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetRoundCornerEnable(BOOL fEnable)
{
    if ( fEnable )
    {
        AddFlag(VIEW_STATE_ROUNDCORNERENABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_ROUNDCORNERENABLE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetRoundCornerRadius(FLOAT fRadiusX, FLOAT fRadiusY)
{
    m_pInternalData->m_fRadiusX = fRadiusX;
    m_pInternalData->m_fRadiusY = fRadiusY;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetBorderWidth(FLOAT fBorderWidth)
{
    fBorderWidth = (fBorderWidth > MAX_BORDER_WIDTH) ? MAX_BORDER_WIDTH : fBorderWidth;

    m_pInternalData->m_fBorderWidth = fBorderWidth;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetBorderColor(const ColorF& borderColor)
{
    m_pInternalData->m_borderColor.a = borderColor.a;
    m_pInternalData->m_borderColor.r = borderColor.r;
    m_pInternalData->m_borderColor.g = borderColor.g;
    m_pInternalData->m_borderColor.b = borderColor.b;

    if ( NULL != m_pInternalData->m_pBorderBrush )
    {
        m_pInternalData->m_pBorderBrush->SetColor(borderColor);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetTag(PVOID pTag)
{
    m_pInternalData->m_pTag = pTag;
}

//////////////////////////////////////////////////////////////////////////

PVOID SdkViewElement::GetTag() const
{
    return m_pInternalData->m_pTag;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetId(UINT id)
{
    m_pInternalData->m_nViewId = id;
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkViewElement::GetId() const
{
    return m_pInternalData->m_nViewId;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::Translate(FLOAT dx, FLOAT dy, FLOAT dz)
{
    UNREFERENCED_PARAMETER(dz);

    m_pInternalData->m_hasTSRMatrix = TRUE;
    m_pInternalData->m_transformInfo.dx = dx;
    m_pInternalData->m_transformInfo.dy = dy;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::Scale(FLOAT sx, FLOAT sy, POINTF centerPt)
{
    m_pInternalData->m_hasTSRMatrix = TRUE;
    m_pInternalData->m_transformInfo.sx = sx;
    m_pInternalData->m_transformInfo.sy = sy;
    m_pInternalData->m_transformInfo.scaleCenterPt = centerPt;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::Rotate(FLOAT angle, POINTF centerPt)
{
    m_pInternalData->m_hasTSRMatrix = TRUE;
    m_pInternalData->m_transformInfo.angle = angle;
    m_pInternalData->m_transformInfo.rotateCenterPt = centerPt;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetViewPos(FLOAT x, FLOAT y)
{
    SetLayoutInfo(x, y, GetWidth(), GetHeight());
    //m_layoutInfo.x = x;
    //m_layoutInfo.y = y;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetViewSize(FLOAT w, FLOAT h)
{
    SetLayoutInfo(GetLeft(), GetTop(), w, h);
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkViewElement::GetLeft() const
{
    return m_layoutInfo.x;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkViewElement::GetTop() const
{
    return m_layoutInfo.y;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkViewElement::GetRight() const
{
    return (m_layoutInfo.x + m_layoutInfo.width);
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkViewElement::GetBottom() const
{
    return (m_layoutInfo.y + m_layoutInfo.height);
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkViewElement::GetWidth() const
{
    return m_layoutInfo.width;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkViewElement::GetHeight() const
{
    return m_layoutInfo.height;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsPtInRect(FLOAT x, FLOAT y)
{
    POINT outPt = { 0, 0 };
    GetAbsolutePoint(outPt);

    Matrix3x2F allMatrix = GetAbsoluteMatrix() * GetAnimationMatrix();
    Matrix3x2F matTranslate = Matrix3x2F::Translation((FLOAT)outPt.x, (FLOAT)outPt.y);
    Matrix3x2F matInvert = allMatrix;
    matInvert = matTranslate * matInvert;
    matInvert.Invert();

    D2D1_POINT_2F srcPT = D2D1::Point2F(x, y);
    D2D1_POINT_2F desPT = matInvert.TransformPoint(srcPT);

    // The new x and y is converted with the matrix transform.
    x = desPT.x;
    y = desPT.y;

    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;
    m_pWindow->GetDesktopDpi(&dpiX, &dpiY);

    if ( (IsVisible()) &&
         (x >= dpiX * m_layoutInfo.x) && (x <= dpiX * (m_layoutInfo.x + m_layoutInfo.width)) &&
         (y >= dpiY * m_layoutInfo.y) && (y <= dpiY * (m_layoutInfo.y + m_layoutInfo.height)) )
    {
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::RequestLayout()
{
    SetLayoutInfo(GetLeft(), GetTop(), GetWidth(), GetHeight());
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkViewElement::GetViewRect()
{
    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;

    if ( NULL != m_pWindow )
    {
        m_pWindow->GetDesktopDpi(&dpiX, &dpiY);
    }

    D2D1_RECT_F rect = D2D1::RectF(
        m_layoutInfo.x * dpiX,
        m_layoutInfo.y * dpiY,
        (m_layoutInfo.x + m_layoutInfo.width) * dpiX,
        (m_layoutInfo.y + m_layoutInfo.height) * dpiY
        );

    return rect;
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkViewElement::GetDrawingRect()
{
    D2D1_RECT_F drawRc = { 0 };
    GetAbsoluteRect(drawRc);

    Matrix3x2F matTSR = GetAbsoluteMatrix();
    Matrix3x2F matAni = GetAnimationMatrix();
    matTSR = matTSR * matAni;

    if (!matTSR.IsIdentity())
    {
        D2D1_POINT_2F leftTopPT = { drawRc.left, drawRc.top };
        D2D1_POINT_2F leftBottomPT = { drawRc.left, drawRc.bottom };
        D2D1_POINT_2F rightTopPT = { drawRc.right, drawRc.top };
        D2D1_POINT_2F rightBottomPT = { drawRc.right, drawRc.bottom };

        leftTopPT = matTSR.TransformPoint(leftTopPT);
        leftBottomPT = matTSR.TransformPoint(leftBottomPT);
        rightTopPT = matTSR.TransformPoint(rightTopPT);
        rightBottomPT = matTSR.TransformPoint(rightBottomPT);

        FLOAT minLeft   = MIN(MIN(leftTopPT.x, leftBottomPT.x), MIN(rightTopPT.x, rightBottomPT.x));
        FLOAT minTop    = MIN(MIN(leftTopPT.y, leftBottomPT.y), MIN(rightTopPT.y, rightBottomPT.y));
        FLOAT maxRight  = MAX(MAX(leftTopPT.x, leftBottomPT.x), MAX(rightTopPT.x, rightBottomPT.x));
        FLOAT maxBottom = MAX(MAX(leftTopPT.y, leftBottomPT.y), MAX(rightTopPT.y, rightBottomPT.y));

        drawRc.left   = minLeft;
        drawRc.top    = minTop;
        drawRc.right  = maxRight;
        drawRc.bottom = maxBottom;
    }

    return drawRc;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::BringToFront()
{
    SdkViewLayout *pParentView = GetParent();
    if (NULL != pParentView && IsVisible())
    {
        pParentView->BringViewToFront(this);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::CancelViewEvent(BOOL isCancel)
{
    if (isCancel)
    {
        AddFlag(VIEW_STATE_CANCELEVENT);
        RemoveFlag(VIEW_STATE_PRESSED);
        Invalidate();
    }
    else
    {
        RemoveFlag(VIEW_STATE_CANCELEVENT);
    }
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkViewElement::GetState() const
{
    return m_pInternalData->m_nViewState;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnWindowDestroy(SdkWindow *pWindow)
{
    UNREFERENCED_PARAMETER(pWindow);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(fChanged);
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);
    UNREFERENCED_PARAMETER(width);
    UNREFERENCED_PARAMETER(height);

    m_pInternalData->m_hasTSRMatrix = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnDrawBoarder(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F& absRc)
{
    FLOAT fBorderWidth = m_pInternalData->m_fBorderWidth;

    if ( fBorderWidth >= 1.0f )
    {
        // Do not delete the pointer to SdkD2DTheme.
        SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

        BOOL isRCEnable = (GetState() & VIEW_STATE_ROUNDCORNERENABLE);

        // Draw the boarder, with round corner.
        pD2DTheme->OnDrawBorder(
            this,
            pRenderTarget,
            m_pInternalData->m_pBorderBrush,
            absRc,
            fBorderWidth,
            isRCEnable ? m_pInternalData->m_fRadiusX : 0,
            isRCEnable ? m_pInternalData->m_fRadiusY : 0);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnDrawBackground(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F& absRc)
{
    // Get the instance of SdkD2DTheme, do not delete the pointer.
    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

    // If the background image is not NULL, we first draw the bitmap,
    // the background color is ignored.
    if ( NULL != m_pInternalData->m_pBKD2DBitmap )
    {
        pD2DTheme->OnDrawBackground(
            this,
            pRenderTarget,
            m_pInternalData->m_pBKD2DBitmap,
            absRc);
    }
    else
    {
        // Draw view's background color.
        if (NULL != m_pInternalData->m_pD2DBrush)
        {
            BOOL isRCEnable = (GetState() & VIEW_STATE_ROUNDCORNERENABLE);

            pD2DTheme->OnDrawBackground(
                this,
                pRenderTarget,
                m_pInternalData->m_pD2DBrush,
                absRc,
                isRCEnable ? m_pInternalData->m_fRadiusX : 0,
                isRCEnable ? m_pInternalData->m_fRadiusY : 0);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);

    if ( !IsEnable() || !IsVisible() )
    {
        return FALSE;
    }

    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isTouchOn = IsPtInRect(xPos, yPos);
    BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
    BOOL isRPressed = (VIEW_STATE_RIGHTPRESSED == (GetState() & VIEW_STATE_RIGHTPRESSED));
    BOOL isSelected = IsSelected();
    BOOL handled = FALSE;

    switch (message)
    {
    case WM_RBUTTONDOWN:
        {
            if ( isTouchOn )
            {
                m_pInternalData->m_preMousePt.x = (LONG)xPos;
                m_pInternalData->m_preMousePt.y = (LONG)yPos;
                AddFlag(VIEW_STATE_RIGHTPRESSED);
                RemoveFlag(VIEW_STATE_CANCELEVENT);
                handled = ((NULL != m_pInternalData->m_pClickHandler) || IsLongClickable());
            }
        }
        break;

    case WM_RBUTTONUP:
        {
            BOOL isCancel = (VIEW_STATE_CANCELEVENT == (GetState() & VIEW_STATE_CANCELEVENT));
            if ( isTouchOn && isRPressed && IsLongClickable() && !isCancel )
            {
                handled = PerformLongClick();
            }
            RemoveFlag(VIEW_STATE_PRESSED);
            RemoveFlag(VIEW_STATE_CANCELEVENT);
        }
        break;

    case WM_LBUTTONDOWN:
        {
            if ( isTouchOn )
            {
                m_pInternalData->m_preMousePt.x = (LONG)xPos;
                m_pInternalData->m_preMousePt.y = (LONG)yPos;
                AddFlag(VIEW_STATE_PRESSED);
                RemoveFlag(VIEW_STATE_CANCELEVENT);
                SetStyle(isSelected ? VIEW_STYLE_SELECTEDDOWN : VIEW_STYLE_MOUSEDOWN);
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            BOOL isToggle = (VIEW_STATE_TOGGLE == (GetState() & VIEW_STATE_TOGGLE));
            BOOL isCancel = (VIEW_STATE_CANCELEVENT == (GetState() & VIEW_STATE_CANCELEVENT));
            BOOL isCancelPerform = IsCancelClick((LONG)xPos, (LONG)yPos);
            BOOL isClickable = IsClickable();

            // If the view's state is toggle, and touch on and pressed, select or dis-select the view.
            if ( isToggle && isTouchOn && isPressed )
            {
                SetSelected(!isSelected);
                isSelected = IsSelected();
            }

            // Set the style normal or hover.
            SetStyle(isSelected ? VIEW_STYLE_SELECTED : VIEW_STYLE_NORMAL);
            RemoveFlag(VIEW_STATE_PRESSED);
            RemoveFlag(VIEW_STATE_CANCELEVENT);

            if ( isTouchOn && isPressed && isClickable && !isCancel && !isCancelPerform )
            {
                // Add this view to event chain, Click operation will finish after all events are processed.
                GetWindow()->AddEventViews(this);
            }
        }
        break;

    case WM_MOUSEMOVE:
        {
            BOOL isCancel = IsCancelClick((LONG)xPos, (LONG)yPos);
            if ( isPressed && isCancel )
            {
                AddFlag(VIEW_STATE_CANCELEVENT);
            }
        }
        break;

    case WM_MOUSELEAVE:
        {
            if ( !isPressed )
            {
                SetStyle(isSelected ? VIEW_STYLE_SELECTED : VIEW_STYLE_NORMAL);
            }
        }
        break;

    case WM_MOUSEHOVER:
        {
            if ( !isPressed )
            {
                SetStyle(isSelected ? VIEW_STYLE_SELECTEDOVER : VIEW_STYLE_MOUSEHOVER);
            }
        }
        break;
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnFocusChanged(SdkViewElement *pView, BOOL fGetFocus)
{
    UNREFERENCED_PARAMETER(pView);
    UNREFERENCED_PARAMETER(fGetFocus);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnVisibilityChanged(SdkViewElement *pView, BOOL fVisible)
{
    UNREFERENCED_PARAMETER(pView);
    UNREFERENCED_PARAMETER(fVisible);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnSelectChenaged(SdkViewElement *pView, BOOL fSelected)
{
    UNREFERENCED_PARAMETER(pView);
    UNREFERENCED_PARAMETER(fSelected);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::PushClip(ID2D1RenderTarget *pRenderTarget)
{
    HRESULT hr = S_OK;

    if ( NULL == m_pInternalData->m_pClipLayer )
    {
        hr = pRenderTarget->CreateLayer(&(m_pInternalData->m_pClipLayer));
    }

    if ( SUCCEEDED(hr) )
    {
        BOOL isRCEnable = (GetState() & VIEW_STATE_ROUNDCORNERENABLE);

        SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
        pD2DTheme->OnPushClip(
            this,
            pRenderTarget,
            m_pInternalData->m_pClipLayer,
            isRCEnable ? m_pInternalData->m_fRadiusX : 0,
            isRCEnable ? m_pInternalData->m_fRadiusY : 0);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::PopClip(ID2D1RenderTarget *pRenderTarget)
{
    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
    pD2DTheme->OnPopClip(this, pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::PerformClick()
{
    if ( (NULL != m_pInternalData->m_pClickHandler) && IsClickable() && IsEnable() )
    {
        m_pInternalData->m_pClickHandler->OnClick(this);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::PerformLongClick()
{
    if ( (NULL != m_pInternalData->m_pClickHandler) &&
         IsLongClickable() &&
         IsEnable() )
    {
        return m_pInternalData->m_pClickHandler->OnLongClick(this);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::PerformOnMouseEvent(const LPMSG lpMsg, SdkViewElement *pSource)
{
    BOOL handled = FALSE;

    if ( IsEnable() && IsVisible() )
    {
        if (NULL != m_pInternalData->m_pViewMouseHandler)
        {
            handled = m_pInternalData->m_pViewMouseHandler->OnMouse(lpMsg, this, pSource);
        }

        if (!handled)
        {
            handled = OnMouseEvent(lpMsg->message, lpMsg->wParam, lpMsg->lParam);
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::PerformOnKeyboardEvent(const LPMSG lpMsg, SdkViewElement *pSource)
{
    BOOL handled = FALSE;

    if ( IsEnable() && IsVisible() )
    {
        if (NULL != m_pInternalData->m_pViewKeyHandler)
        {
            handled = m_pInternalData->m_pViewKeyHandler->OnKey(lpMsg, this, pSource);
        }

        if (!handled)
        {
            handled = OnKeyboardEvent(lpMsg->message, lpMsg->wParam, lpMsg->lParam);
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F SdkViewElement::GetCurMatrix()
{
    return GetTSRMatrix() * GetAnimationMatrix();
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F SdkViewElement::GetTSRMatrix()
{
    POINT outPT = { (LONG)m_layoutInfo.x, (LONG)m_layoutInfo.y };
    GetAbsolutePoint(outPT);

    D2D1_POINT_2F scaleCenter =
    {
        m_pInternalData->m_transformInfo.scaleCenterPt.x + (FLOAT)outPT.x,
        m_pInternalData->m_transformInfo.scaleCenterPt.y + (FLOAT)outPT.y
    };

    D2D1_POINT_2F rotateCenter = 
    {
        m_pInternalData->m_transformInfo.rotateCenterPt.x + (FLOAT)outPT.x,
        m_pInternalData->m_transformInfo.rotateCenterPt.y + (FLOAT)outPT.y
    };

    Matrix3x2F matTrans = Matrix3x2F::Translation(m_pInternalData->m_transformInfo.dx, m_pInternalData->m_transformInfo.dy);
    Matrix3x2F matScale = Matrix3x2F::Scale(m_pInternalData->m_transformInfo.sx, m_pInternalData->m_transformInfo.sy, scaleCenter);
    Matrix3x2F matRotate = Matrix3x2F::Rotation(m_pInternalData->m_transformInfo.angle, rotateCenter);

    return (matTrans * matScale * matRotate);
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F SdkViewElement::GetAnimationMatrix()
{
    Matrix3x2F animMatrix = Matrix3x2F::Identity();

    BOOL isAnimMatEnable = IsAnimMatrixEnable();
    if (NULL != m_pInternalData->m_pAnimation && isAnimMatEnable)
    {
        TRANSFORMINFO info;
        if (SUCCEEDED(m_pInternalData->m_pAnimation->GetTransform(&info)))
        {
            animMatrix = *(Matrix3x2F::ReinterpretBaseType(&(info.matrixTransform)));
        }
    }

    return animMatrix;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsAnimMatrixEnable()
{
    return !(VIEW_STATE_DISABLEANIMMAT == (GetState() & VIEW_STATE_DISABLEANIMMAT));
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::GetAbsoluteRect(OUT RECT& outRc)
{
    POINT outPt = { 0, 0 };
    GetAbsolutePoint(outPt);
    RECT rc =
    {
        (LONG)m_layoutInfo.x,
        (LONG)m_layoutInfo.y,
        (LONG)m_layoutInfo.x + (LONG)m_layoutInfo.width,
        (LONG)m_layoutInfo.y + (LONG)m_layoutInfo.height
    };

    OffsetRect(&rc, outPt.x, outPt.y);
    outRc = rc;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::GetAbsoluteRect(OUT D2D1_RECT_F& outRc)
{
    RECT rc = { 0 };
    GetAbsoluteRect(rc);

    D2D1_RECT_F rect = { (FLOAT)rc.left, (FLOAT)rc.top, (FLOAT)rc.right,(FLOAT)rc.bottom };
    outRc = rect;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::GetAbsolutePoint(IN OUT POINT& outPt)
{
    SdkViewLayout *pParentView = m_pInternalData->m_pParentView;

    FLOAT offsetX = 0.0f;
    FLOAT offsetY = 0.0f;

    while (NULL != pParentView)
    {
        FLOAT dx = pParentView->GetLeft();
        FLOAT dy = pParentView->GetTop();
        offsetX += dx;
        offsetY += dy;

        pParentView = pParentView->GetParent();
    }

    outPt.x += (LONG)offsetX;
    outPt.y += (LONG)offsetY;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::GetAbsolutePoint(IN OUT D2D1_POINT_2F& outPt)
{
    POINT pt = { 0 };
    GetAbsolutePoint(pt);

    outPt.x += (FLOAT)pt.x;
    outPt.y += (FLOAT)pt.y;
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F SdkViewElement::GetAbsoluteMatrix()
{
    SdkViewLayout *pParentView = m_pInternalData->m_pParentView;
    Matrix3x2F matrix = m_pInternalData->m_viewMatrix;

    while (NULL != pParentView)
    {
        Matrix3x2F viewMatrix = pParentView->m_pInternalData->m_viewMatrix;
        Matrix3x2F animMatrix = Matrix3x2F::Identity();

        if (pParentView->IsAnimMatrixEnable() && NULL != pParentView->m_pInternalData->m_pAnimation)
        {
            TRANSFORMINFO info;
            if (SUCCEEDED(pParentView->m_pInternalData->m_pAnimation->GetTransform(&info)))
            {
                animMatrix = *(Matrix3x2F::ReinterpretBaseType(&(info.matrixTransform)));
            }
        }

        matrix = matrix * viewMatrix * animMatrix;
        pParentView = pParentView->GetParent();
    }

    return matrix;
}

//////////////////////////////////////////////////////////////////////////

D2D1_POINT_2F SdkViewElement::ConvertPoint(IN const D2D1_POINT_2F& srcPt, BOOL fRelative)
{
    // Convert the specified point to another point relative to the view.
    D2D1_POINT_2F desPT = srcPt;

    Matrix3x2F allMatrix = GetAbsoluteMatrix() * GetAnimationMatrix();
    Matrix3x2F matTranslate = Matrix3x2F::Identity();

    // If the destination point is relative to view's bound, translate absolute point to
    // relative point (0, 0).
    if ( fRelative )
    {
        POINT outPt = { (LONG)GetLeft(), (LONG)GetTop() };
        GetAbsolutePoint(outPt);
        matTranslate = Matrix3x2F::Translation((FLOAT)outPt.x, (FLOAT)outPt.y);
    }

    Matrix3x2F matInvert = allMatrix;
    matInvert = matTranslate * matInvert;
    matInvert.Invert();

    desPT = matInvert.TransformPoint(srcPt);

    return desPT;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewElement::IsCancelClick(LONG xPos, LONG yPos)
{
    LONG deltaX = (m_pInternalData->m_preMousePt.x - (LONG)xPos);
    LONG deltaY = (m_pInternalData->m_preMousePt.y - (LONG)yPos);

    BOOL isCancel = !( (abs(deltaX) <= MOUSE_MOVE_RANGE) && (abs(deltaY) <= MOUSE_MOVE_RANGE) );
    BOOL isCancelClickDisable = (VIEW_STATE_DISABLECANCELEVENT == (VIEW_STATE_DISABLECANCELEVENT & GetState()));
    if (isCancelClickDisable)
    {
        return FALSE;
    }

    return isCancel;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetStyle(VIEW_STYLE style)
{
    // It is meaningful for setting view's style while it is click-able, because
    // setting view's style is not meaningful for some views, such as text view, view layout, etc.
    // so add this check statement to avoid unnecessary painting.
    if ( IsClickable() )
    {
        if ( m_pInternalData->m_viewStyle != style )
        {
            m_pInternalData->m_viewStyle = style;
            Invalidate();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

VIEW_STYLE SdkViewElement::GetStyle() const
{
    return m_pInternalData->m_viewStyle;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::AddFlag(VIEW_STATE state)
{
    m_pInternalData->m_nViewState |= state;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::RemoveFlag(VIEW_STATE state)
{
    m_pInternalData->m_nViewState &= ~state;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::CombineTSRMatrix()
{
    if (m_pInternalData->m_hasTSRMatrix)
    {
        m_pInternalData->m_viewMatrix = GetTSRMatrix();
        m_pInternalData->m_hasTSRMatrix = FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::ForceInvalidate()
{
    if (NULL != m_pWindow)
    {
        //m_pWindow->AddWindowState(WINDOW_STATE_INVALIDATE);
        m_pWindow->Invalidate(TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewElement::SetClassName(LPCTSTR lpClassName)
{
    m_pInternalData->m_lpClassName = lpClassName;
}
