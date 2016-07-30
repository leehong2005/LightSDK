/*!
* @file ViewElement.cpp
* 
* @brief This file defines the base view class for various visual elements.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "ViewElement.h"
#include "ViewLayout.h"
#include "D2DSolidColorBrush.h"

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

/*!
* @brief The internal data for the ViewElement class.
*/
struct INTERNALDATA
{
    FLOAT                   m_fBorderWidth;             // The border width.
    FLOAT                   m_fRoundCornerRadiusX;      // Round corner radius of x.
    FLOAT                   m_fRoundCornerRadiusY;      // Round corner radius of y.
    POINT                   m_preMousePt;               // The previous mouse point.
    INT32                   m_nViewIndex;               // The view's index in view container.
    INT32                   m_nViewId;                  // The identifier of current view.
    INT32                   m_nViewState;               // The current state of view.
    BOOL                    m_hasClearAssocData;        // Has clear associated data or not.
    BOOL                    m_hasTSRMatrix;             // Indicates whether has T, S, R matrix.
    PVOID                   m_pTag;                     // The pointer to view tag.
    D2D1_COLOR_F            m_bkColor;                  // The color of background.
    D2D1_COLOR_F            m_borderColor;              // The border color.
    D2D1_COLOR_F            m_selectColor;              // The select border color.
    VIEW_TRANSFORMINFO      m_transformInfo;            // The transform data.
    Matrix3x2F              m_viewMatrix;               // The matrix of view.
    ID2D1Layer             *m_pClipLayer;               // The clip layer.
    D2DBrush               *m_pD2DBrush;                // The background brush;
    D2DBrush               *m_pBorderBrush;             // The border color brush.
    D2DBrush               *m_pSelectBrush;             // The selected brush.
    D2DBitmap              *m_pBKD2DBitmap;             // The pointer which points to the object of D2DBitmap.
    ViewLayout             *m_pParentView;              // The parent view, do NOT release it.
    Animation              *m_pAnimation;               // The animation to be started.
    ViewOnMouseHandler     *m_pViewMouseHandler;        // The mouse event handler.
    ViewOnTouchHandler     *m_pViewTouchHandler;        // The touch event handler.
    ViewOnKeyHandler       *m_pViewKeyHandler;          // The key event handler.
    ViewOnClickHandler     *m_pClickHandler;            // The click handler.
    ViewOnLongClickHandler *m_pLongClickHandler;        // The long click handler.
};


//////////////////////////////////////////////////////////////////////////

ViewElement::ViewElement() : m_pWindow(NULL),
                             m_pInternalData(new INTERNALDATA())
{
    ZeroMemory(m_pInternalData, sizeof(INTERNALDATA));

    m_pInternalData->m_viewMatrix           = Matrix3x2F::Identity();
    m_pInternalData->m_pD2DBrush            = new D2DSolidColorBrush();
    m_pInternalData->m_bkColor              = D2D1::ColorF(0, 0);
    m_pInternalData->m_borderColor          = D2D1::ColorF(ColorF::Gray);
    m_pInternalData->m_selectColor          = D2D1::ColorF((231.f / 255.f), (78.f / 255.f), (78.f / 255.f));
    m_pInternalData->m_nViewState           = VIEW_STATE_NORMAL | VIEW_STATE_ENABLE | VIEW_STATE_VISIBLE;
    m_pInternalData->m_hasClearAssocData    = TRUE;
    m_pInternalData->m_hasTSRMatrix         = FALSE;
    m_pInternalData->m_nViewId              = -1;
    m_pInternalData->m_fBorderWidth         = -1.0f;
    m_pInternalData->m_fRoundCornerRadiusX  = 8.0f;
    m_pInternalData->m_fRoundCornerRadiusY  = 8.0f;
    m_pInternalData->m_preMousePt.x         = 0;
    m_pInternalData->m_preMousePt.y         = 0;
    m_pInternalData->m_transformInfo.sx     = 1.0f;
    m_pInternalData->m_transformInfo.sy     = 1.0f;

    ZeroMemory(&m_layoutInfo, sizeof(LAYOUTINFO));

    m_layoutInfo.width      = 100.0f;
    m_layoutInfo.height     = 100.0f;
    m_layoutInfo.minWidth   = 1.0f;
    m_layoutInfo.minHeight  = 1.0f;
    m_layoutInfo.maxHeight  = 2500.0f * 20;
    m_layoutInfo.maxWidth   = 2500.0f * 20;
}

//////////////////////////////////////////////////////////////////////////

ViewElement::~ViewElement()
{
    SAFE_DELETE(m_pInternalData->m_pD2DBrush);
    SAFE_DELETE(m_pInternalData->m_pBorderBrush);
    SAFE_DELETE(m_pInternalData->m_pBKD2DBitmap);
    SAFE_DELETE(m_pInternalData->m_pSelectBrush);
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

void ViewElement::OnPaint()
{
    if ( !IsVisible() || (NULL == m_pWindow) )
    {
        return;
    }

    D2DDevice *pD2DDevice = m_pWindow->GetD2DDevices();
    if (NULL == pD2DDevice)
    {
        return;
    }

    ID2D1RenderTarget *pRenderTarget = NULL;
    pD2DDevice->GetRenderTarget(&pRenderTarget);

    Matrix3x2F animMatrix = Matrix3x2F::Identity();
    // Set the matrix to render target got from animation.
    if ( IsAnimationMatrixEnable() && (NULL != m_pInternalData->m_pAnimation) )
    {
        TRANSFORMINFO info;
        if (SUCCEEDED(m_pInternalData->m_pAnimation->GetTransform(&info)))
        {
            animMatrix = *(Matrix3x2F::ReinterpretBaseType(&(info.matrixTransform)));
        }
    }

    // Combine the matrix;
    CombineTSRMatrix();
    Matrix3x2F absoluteMatrix = GetAbsoluteMatrix();
    absoluteMatrix = absoluteMatrix * animMatrix;
    pRenderTarget->SetTransform(absoluteMatrix);

    PushClip(pRenderTarget);
    OnDrawItem(pRenderTarget);
    PopClip(pRenderTarget);

    pRenderTarget->SetTransform(Matrix3x2F::Identity());

    // If the window on which view located is NOT layered window, we use paint to drive animation,
    // If the window is layered window, we use animation timer to drive animation.
    if (!m_pWindow->IsLayeredWindow() && (NULL != m_pInternalData->m_pAnimation) && IsAnimationMatrixEnable())
    {
        // If the animation does not finish, go on to invalidate window.
        BOOL isFinish = FALSE;
        if (SUCCEEDED(m_pInternalData->m_pAnimation->IsFinish(&isFinish)))
        {
            if (isFinish)
            {
                BOOL isClearAnim = ((GetState() & VIEW_STATE_CLEARANIMATION));
                if (isClearAnim)
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

void ViewElement::OnPaintEx(HDC hDC, const LPRECT lpRect)
{
    if ((NULL == hDC) || (NULL == lpRect) || (NULL == m_pWindow))
    {
        return;
    }

    D2DDevice *pD2DDevice = (NULL != m_pWindow) ? m_pWindow->GetD2DDevices() : NULL;
    if (NULL == pD2DDevice || !IsVisible())
    {
        return;
    }

    FLOAT left   = m_layoutInfo.x;
    FLOAT top    = m_layoutInfo.y;
    FLOAT width  = m_layoutInfo.width;
    FLOAT height = m_layoutInfo.height;

    RECT bindRc   = { 0 };
    bindRc.right  = (lpRect->right  - lpRect->left);
    bindRc.bottom = (lpRect->bottom - lpRect->top);

    m_layoutInfo.x = (FLOAT)-lpRect->left;
    m_layoutInfo.y = (FLOAT)-lpRect->top;
    m_layoutInfo.width  = (FLOAT)bindRc.right;
    m_layoutInfo.height = (FLOAT)bindRc.bottom;

    pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_DC);
    pD2DDevice->BeginDraw(hDC, &bindRc);
    ViewLayout *pParent = GetParent(NULL);
    AddFlag(VIEW_STATE_PAINTALLVIEWS);
    SetParent(NULL);
    OnPaint();
    SetParent(pParent);
    RemoveFlag(VIEW_STATE_PAINTALLVIEWS);
    pD2DDevice->EndDraw();

    m_layoutInfo.x = left;
    m_layoutInfo.y = top;
    m_layoutInfo.width  = width;
    m_layoutInfo.height = height;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F absRc = { 0, 0 };
    GetAbsoluteRect(absRc);

    // Draw view's background.
    OnDrawBackground(pRenderTarget, absRc);

    // Draw view's border.
    OnDrawBoarder(pRenderTarget, absRc);

    // Draw selected border.
    OnDrawSelect(pRenderTarget, absRc);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::Invalidate(BOOL isUpdateNow)
{
    if (NULL != m_pWindow)
    {
        if (isUpdateNow)
        {
            m_pWindow->Invalidate(TRUE);
        }
        else
        {
            //m_pWindow->AddWindowState(WINDOW_STATE_INVALIDATE);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
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

void ViewElement::SetLayoutInfo(IN const LAYOUTINFO* pLayoutInfo)
{
    if (NULL != pLayoutInfo)
    {
        SetLayoutInfo(pLayoutInfo->x, pLayoutInfo->y, pLayoutInfo->width, pLayoutInfo->height);
        SetMargin(pLayoutInfo->leftMargin, pLayoutInfo->topMargin, pLayoutInfo->rightMargin, pLayoutInfo->bottomMargin);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetMargin(FLOAT margin)
{
    m_layoutInfo.leftMargin   = margin;
    m_layoutInfo.topMargin    = margin;
    m_layoutInfo.rightMargin  = margin;
    m_layoutInfo.bottomMargin = margin;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetMargin(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    m_layoutInfo.leftMargin   = left;
    m_layoutInfo.topMargin    = top;
    m_layoutInfo.rightMargin  = right;
    m_layoutInfo.bottomMargin = bottom;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetMaxWidth(FLOAT maxWidth)
{
    m_layoutInfo.maxWidth = maxWidth;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetMaxHeight(FLOAT maxHeight)
{
    m_layoutInfo.maxHeight = maxHeight;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetMinWidth(FLOAT minWidth)
{
    if (m_layoutInfo.minWidth != minWidth)
    {
        m_layoutInfo.minWidth = minWidth;
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetMinHeight(FLOAT minHeight)
{
    m_layoutInfo.minHeight = minHeight;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::GetLayoutInfo(OUT LAYOUTINFO* pLayoutInfo) const
{
    if (NULL != pLayoutInfo)
    {
        *pLayoutInfo = m_layoutInfo;
    }
}

//////////////////////////////////////////////////////////////////////////

Window* ViewElement::GetWindow()
{
    return m_pWindow;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetWindow(Window *pWindow)
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

void ViewElement::ShowView(BOOL isShow)
{
    if (isShow != IsVisible())
    {
        SetVisible(isShow);
        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetOnMouseHandler(IN ViewOnMouseHandler *pViewMouseHandler)
{
    m_pInternalData->m_pViewMouseHandler = pViewMouseHandler;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetOnTouchHandler(IN ViewOnTouchHandler *pViewTouchHandler)
{
    m_pInternalData->m_pViewTouchHandler = pViewTouchHandler;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetOnKeyHandler(IN ViewOnKeyHandler *pViewKeyHandler)
{
    m_pInternalData->m_pViewKeyHandler = pViewKeyHandler;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetOnClickHandler(IN ViewOnClickHandler *pClickHandler)
{
    m_pInternalData->m_pClickHandler = pClickHandler;
    SetClickable(NULL != pClickHandler);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetOnLongClickHandler(IN ViewOnLongClickHandler *pLongClickHandler)
{
    m_pInternalData->m_pLongClickHandler = pLongClickHandler;
    SetLongClickable(NULL != pLongClickHandler);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetBackgroundColor(const ColorF &color)
{
    m_pInternalData->m_bkColor = color;
    AddFlag(VIEW_STATE_DRAWBKCOLOR);
}

//////////////////////////////////////////////////////////////////////////

ColorF ViewElement::GetBackgroundColor()
{
    D2D1_COLOR_F color = m_pInternalData->m_bkColor;

    return ColorF(color.r, color.g, color.b, color.a);
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::SetBackgroundImage(IN UINT32 uResId, HINSTANCE hInstance, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if (NULL == m_pInternalData->m_pBKD2DBitmap)
    {
        m_pInternalData->m_pBKD2DBitmap = new D2DBitmap();
    }

    BOOL isSuccess = m_pInternalData->m_pBKD2DBitmap->LoadFromResource(uResId, hInstance, uDestWidth, uDestHeight);
    if (isSuccess)
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

BOOL ViewElement::SetBackgroundImage(IN HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if (NULL == m_pInternalData->m_pBKD2DBitmap)
    {
        m_pInternalData->m_pBKD2DBitmap = new D2DBitmap();
    }

    BOOL isSuccess = m_pInternalData->m_pBKD2DBitmap->LoadFromHBITMPA(hBitmap,uDestWidth, uDestHeight);
    if (isSuccess)
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

BOOL ViewElement::SetBackgroundImage(IN LPCWSTR lpfileName, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if (NULL == m_pInternalData->m_pBKD2DBitmap)
    {
        m_pInternalData->m_pBKD2DBitmap = new D2DBitmap();
    }

    BOOL isSuccess = m_pInternalData->m_pBKD2DBitmap->LoadFromFile(lpfileName, uDestWidth, uDestHeight);
    if (isSuccess)
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

void ViewElement::SetBackgroundBrush(IN D2DBrush *pBKBrush)
{
    SAFE_DELETE(m_pInternalData->m_pD2DBrush);
    m_pInternalData->m_pD2DBrush = pBKBrush;

    if (NULL != pBKBrush)
    {
        m_pInternalData->m_pD2DBrush->GetColor(&m_pInternalData->m_bkColor);
        AddFlag(VIEW_STATE_DRAWBKCOLOR);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetAnimation(Animation *pAnimation)
{
    m_pInternalData->m_pAnimation = pAnimation;
}

//////////////////////////////////////////////////////////////////////////

Animation* ViewElement::GetAnimation()
{
    return m_pInternalData->m_pAnimation;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::StartAnimation(DOUBLE dDelaySecond)
{
    if (NULL != m_pInternalData->m_pAnimation)
    {
        m_pInternalData->m_pAnimation->AddAnimationListener(this);
        m_pInternalData->m_pAnimation->AddAnimationTimerListener(this);
        m_pInternalData->m_pAnimation->Start(dDelaySecond);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::ClearAnimation()
{
    m_pInternalData->m_pAnimation = NULL;
    ForceInvalidate();
}

//////////////////////////////////////////////////////////////////////////

ViewLayout* ViewElement::GetParent(ViewLayout **ppParentView)
{
    if (NULL != ppParentView)
    {
        *ppParentView = m_pInternalData->m_pParentView;
    }

    return m_pInternalData->m_pParentView;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetParent(ViewLayout *pParentView)
{
    m_pInternalData->m_pParentView = pParentView;
}

//////////////////////////////////////////////////////////////////////////

ViewElement* ViewElement::FindEventSource(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);

    if (IsEnable() && IsVisible())
    {
        FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
        FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
        BOOL isTouchOn = IsPtInRect(xPos, yPos);
        if (isTouchOn)
        {
            // Set the event source and target.
            if (NULL != m_pWindow)
            {
                m_pWindow->WindowViews.EventSourceView = this;

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

BOOL ViewElement::DispatchMessageEvent(const LPMSG lpMsg)
{
    return OnMessageEvent(lpMsg->message, lpMsg->wParam, lpMsg->lParam);
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::DispatchMouseEvent(const LPMSG lpMsg)
{
    BOOL handled = FALSE;
    ViewElement *pSource = this;

    // At this time, the pSource may be invisible although mouse left button has down on it,
    // so that the view can NOT receive message, such as WM_LBUTTONUP, we should make sure 
    // the WM_LBUTTONDOWN match WM_LBUTTONUP, so always perform mouse event.
    BOOL isVisible = pSource->IsVisible();
    pSource->SetVisible(TRUE);
    handled = pSource->PerformOnMouseEvent(lpMsg, pSource);
    pSource->SetVisible(isVisible);

    if (!handled)
    {
        ViewElement *pParentView = pSource->GetParent(NULL);
        while (NULL != pParentView)
        {
            handled = pParentView->PerformOnMouseEvent(lpMsg, pSource);
            if (handled)
            {
                break;
            }

            pParentView = pParentView->GetParent(NULL);
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::DispatchTouchEvent(const LPMSG lpMsg)
{
    ViewElement *pSource = this;
    return pSource->PerformOnTouchEvent(lpMsg, pSource);
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::PreDispathMouseEvent(const LPMSG lpMsg, ViewElement *pSource)
{
    BOOL handled = PerformOnMouseEvent(lpMsg, pSource);
    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::PreDispathTouchEvent(const LPMSG lpMsg, ViewElement *pSource)
{
    BOOL handled = PerformOnTouchEvent(lpMsg, pSource);
    return handled;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::ClearAssocData()
{
    SAFE_DELETE(m_pInternalData->m_pBKD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::PerformAssocEvent()
{
    return PerformClick();
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetSelected(BOOL isSelected)
{
    if (isSelected)
    {
        AddFlag(VIEW_STATE_SELECTED);
    }
    else
    {
        RemoveFlag(VIEW_STATE_SELECTED);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsClickable()
{
    return (VIEW_STATE_CLICKABLE == (m_pInternalData->m_nViewState & VIEW_STATE_CLICKABLE));
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsLongClickable()
{
    return (VIEW_STATE_LONGCLICKABLE == (m_pInternalData->m_nViewState & VIEW_STATE_LONGCLICKABLE));
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetClickable(BOOL isClickable)
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

void ViewElement::SetCancelClickDisable(BOOL isDisable)
{
    if (isDisable)
    {
        AddFlag(VIEW_STATE_DISABLECANCELEVENT);
    }
    else
    {
        RemoveFlag(VIEW_STATE_DISABLECANCELEVENT);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetLongClickable(BOOL isLongClickable)
{
    if (isLongClickable)
    {
        AddFlag(VIEW_STATE_LONGCLICKABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_LONGCLICKABLE);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetFocusable(BOOL isFocusable)
{
    if (isFocusable)
    {
        AddFlag(VIEW_STATE_FOCUSABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_FOCUSABLE);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsFocusable()
{
    return (VIEW_STATE_FOCUSABLE == (m_pInternalData->m_nViewState & VIEW_STATE_FOCUSABLE));
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsEnable()
{
    return (VIEW_STATE_ENABLE == (m_pInternalData->m_nViewState & VIEW_STATE_ENABLE));
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetEnable(BOOL isEnable)
{
    if (isEnable)
    {
        AddFlag(VIEW_STATE_ENABLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_ENABLE);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetVisible(BOOL isVisible)
{
    if (isVisible)
    {
        AddFlag(VIEW_STATE_VISIBLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_VISIBLE);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsVisible()
{
    return (VIEW_STATE_VISIBLE == (m_pInternalData->m_nViewState & VIEW_STATE_VISIBLE));
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsSelected()
{
    return (VIEW_STATE_SELECTED == (m_pInternalData->m_nViewState & VIEW_STATE_SELECTED));
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetRoundCornerEnable(BOOL fEnable)
{
    if (fEnable)
    {
        AddFlag(VIEW_STATE_SHOWROUNDCORNER);
    }
    else
    {
        RemoveFlag(VIEW_STATE_SHOWROUNDCORNER);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetRoundCornerRadius(FLOAT fRadiusX, FLOAT fRadiusY)
{
    m_pInternalData->m_fRoundCornerRadiusX = fRadiusX;
    m_pInternalData->m_fRoundCornerRadiusY = fRadiusY;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetBorderWidth(FLOAT fBorderWidth)
{
    fBorderWidth = (fBorderWidth > MAX_BORDER_WIDTH) ? MAX_BORDER_WIDTH : fBorderWidth;

    m_pInternalData->m_fBorderWidth = fBorderWidth;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetBorderColor(const ColorF& borderColor)
{
    m_pInternalData->m_borderColor.a = borderColor.a;
    m_pInternalData->m_borderColor.r = borderColor.r;
    m_pInternalData->m_borderColor.g = borderColor.g;
    m_pInternalData->m_borderColor.b = borderColor.b;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetTag(PVOID pTag)
{
    m_pInternalData->m_pTag = pTag;
}

//////////////////////////////////////////////////////////////////////////

PVOID ViewElement::GetTag() const
{
    return m_pInternalData->m_pTag;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetId(UINT id)
{
    m_pInternalData->m_nViewId = id;
}

//////////////////////////////////////////////////////////////////////////

INT32 ViewElement::GetId() const
{
    return m_pInternalData->m_nViewId;
}

//////////////////////////////////////////////////////////////////////////

INT32 ViewElement::GetState() const
{
    return m_pInternalData->m_nViewState;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::Translate(FLOAT dx, FLOAT dy, FLOAT dz)
{
    UNREFERENCED_PARAMETER(dz);

    m_pInternalData->m_hasTSRMatrix = TRUE;
    m_pInternalData->m_transformInfo.dx = dx;
    m_pInternalData->m_transformInfo.dy = dy;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::Scale(FLOAT sx, FLOAT sy, POINTF centerPt)
{
    m_pInternalData->m_hasTSRMatrix = TRUE;
    m_pInternalData->m_transformInfo.sx = sx;
    m_pInternalData->m_transformInfo.sy = sy;
    m_pInternalData->m_transformInfo.scaleCenterPt = centerPt;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::Rotate(FLOAT angle, POINTF centerPt)
{
    m_pInternalData->m_hasTSRMatrix = TRUE;
    m_pInternalData->m_transformInfo.angle = angle;
    m_pInternalData->m_transformInfo.rotateCenterPt = centerPt;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::GetAbsoluteRect(OUT RECT& outRc)
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

void ViewElement::GetAbsoluteRect(OUT D2D1_RECT_F& outRc)
{
    RECT rc = { 0 };
    GetAbsoluteRect(rc);

    D2D1_RECT_F rect = { (FLOAT)rc.left, (FLOAT)rc.top, (FLOAT)rc.right,(FLOAT)rc.bottom };
    outRc = rect;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::GetAbsolutePoint(IN OUT POINT& outPt)
{
    ViewLayout *pParentView = m_pInternalData->m_pParentView;

    FLOAT offsetX = 0.0f;
    FLOAT offsetY = 0.0f;

    while (NULL != pParentView)
    {
        FLOAT dx = pParentView->GetLeft();
        FLOAT dy = pParentView->GetTop();
        offsetX += dx;
        offsetY += dy;

        pParentView = pParentView->GetParent(NULL);
    }

    outPt.x += (LONG)offsetX;
    outPt.y += (LONG)offsetY;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::GetAbsolutePoint(IN OUT D2D1_POINT_2F& outPt)
{
    POINT pt = { 0 };
    GetAbsolutePoint(pt);

    outPt.x += (FLOAT)pt.x;
    outPt.y += (FLOAT)pt.y;
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F ViewElement::GetAbsoluteMatrix()
{
    ViewLayout *pParentView = m_pInternalData->m_pParentView;
    Matrix3x2F matrix = m_pInternalData->m_viewMatrix;

    while (NULL != pParentView)
    {
        Matrix3x2F viewMatrix = pParentView->m_pInternalData->m_viewMatrix;
        Matrix3x2F animMatrix = Matrix3x2F::Identity();

        if (pParentView->IsAnimationMatrixEnable() && NULL != pParentView->m_pInternalData->m_pAnimation)
        {
            TRANSFORMINFO info;
            if (SUCCEEDED(pParentView->m_pInternalData->m_pAnimation->GetTransform(&info)))
            {
                animMatrix = *(Matrix3x2F::ReinterpretBaseType(&(info.matrixTransform)));
            }
        }

        matrix = matrix * viewMatrix * animMatrix;
        pParentView = pParentView->GetParent(NULL);
    }

    return matrix;
}


//////////////////////////////////////////////////////////////////////////

void ViewElement::SetViewPos(FLOAT x, FLOAT y)
{
    //SetLayoutInfo(x, y, GetWidth(), GetHeight());
    m_layoutInfo.x = x;
    m_layoutInfo.y = y;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetViewSize(FLOAT w, FLOAT h)
{
    SetLayoutInfo(GetLeft(), GetTop(), w, h);
}

//////////////////////////////////////////////////////////////////////////

FLOAT ViewElement::GetLeft()
{
    return m_layoutInfo.x;
}

//////////////////////////////////////////////////////////////////////////

FLOAT ViewElement::GetTop()
{
    return m_layoutInfo.y;
}

//////////////////////////////////////////////////////////////////////////

FLOAT ViewElement::GetRight()
{
    return (m_layoutInfo.x + m_layoutInfo.width);
}

//////////////////////////////////////////////////////////////////////////

FLOAT ViewElement::GetBottom()
{
    return (m_layoutInfo.y + m_layoutInfo.height);
}

//////////////////////////////////////////////////////////////////////////

FLOAT ViewElement::GetWidth()
{
    return m_layoutInfo.width;
}

//////////////////////////////////////////////////////////////////////////

FLOAT ViewElement::GetHeight()
{
    return m_layoutInfo.height;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsPtInRect(FLOAT x, FLOAT y)
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

    x = desPT.x;
    y = desPT.y;

    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;
    m_pWindow->GetDesktopDpiRatio(&dpiX, &dpiY);

    if ( (TRUE == IsVisible()) &&
        (x >= dpiX * m_layoutInfo.x) && (x <= dpiX * (m_layoutInfo.x + m_layoutInfo.width)) &&
        (y >= dpiY * m_layoutInfo.y) && (y <= dpiY * (m_layoutInfo.y + m_layoutInfo.height)) )
    {
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::RequestLayout()
{
    SetLayoutInfo(GetLeft(), GetTop(), GetWidth(), GetHeight());
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F ViewElement::GetViewRect()
{
    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;
    m_pWindow->GetDesktopDpiRatio(&dpiX, &dpiY);
    return D2D1::RectF( m_layoutInfo.x * dpiX, m_layoutInfo.y * dpiY,
                       (m_layoutInfo.x + m_layoutInfo.width) * dpiX,
                       (m_layoutInfo.y + m_layoutInfo.height) * dpiY );
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F ViewElement::GetDrawingRect()
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

D2D1_POINT_2F ViewElement::ConvertPoint(IN const D2D1_POINT_2F& srcPt)
{
    POINT outPt = { 0, 0 };
    GetAbsolutePoint(outPt);

    Matrix3x2F allMatrix = GetAbsoluteMatrix() * GetAnimationMatrix();
    Matrix3x2F matTranslate = Matrix3x2F::Translation((FLOAT)outPt.x, (FLOAT)outPt.y);
    Matrix3x2F matInvert = allMatrix;
    matInvert = matTranslate * matInvert;
    matInvert.Invert();

    D2D1_POINT_2F desPT = matInvert.TransformPoint(srcPt);

    return desPT;
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F ViewElement::GetCurMatrix()
{
    return GetTSRMatrix() * GetAnimationMatrix();
}

//////////////////////////////////////////////////////////////////////////

Matrix3x2F ViewElement::GetTSRMatrix()
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

Matrix3x2F ViewElement::GetAnimationMatrix()
{
    Matrix3x2F animMatrix = Matrix3x2F::Identity();

    BOOL isAnimMatEnable = IsAnimationMatrixEnable();
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

BOOL ViewElement::IsAnimationMatrixEnable()
{
    return !(VIEW_STATE_DISABLEANIMMAT == (GetState() & VIEW_STATE_DISABLEANIMMAT));
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::BringToFront()
{
    ViewLayout *pParentView = GetParent(NULL);
    if (NULL != pParentView && IsVisible())
    {
        pParentView->BringViewToFront(this);
    }
}

//////////////////////////////////////////////////////////////////////////

INT32 ViewElement::GetViewIndex()
{
    return m_pInternalData->m_nViewIndex;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetViewIndex(INT32 nIndex)
{
    m_pInternalData->m_nViewIndex = nIndex;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsFocused()
{
    return (VIEW_STATE_FOCUSED == (m_pInternalData->m_nViewState & VIEW_STATE_FOCUSED));
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::SetClearAssocData(BOOL hasClear)
{
    m_pInternalData->m_hasClearAssocData = hasClear;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsClearAssocData()
{
    return m_pInternalData->m_hasClearAssocData;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::CancelViewEvent(BOOL isCancel)
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

void ViewElement::OnWindowDestroy(Window *pWindow)
{
    UNREFERENCED_PARAMETER(pWindow);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnAnimationStart(Animation *pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnAnimationPlaying(Animation *pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnAnimationEnd(Animation *pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);

    BOOL isClearAnim = (VIEW_STATE_CLEARANIMATION == (GetState() & VIEW_STATE_CLEARANIMATION));
    if (isClearAnim)
    {
        ClearAnimation();
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnAnimationTimerUpdate(Animation *pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);
    ForceInvalidate();
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(fChanged);
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);
    UNREFERENCED_PARAMETER(width);
    UNREFERENCED_PARAMETER(height);

    m_pInternalData->m_hasTSRMatrix = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnDrawBoarder(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F& absRc)
{
    FLOAT fBorderWidth = m_pInternalData->m_fBorderWidth;

    if ( (fBorderWidth < 1.0f) || (NULL == pRenderTarget) )
    {
        return;
    }

    if (NULL == m_pInternalData->m_pBorderBrush)
    {
        m_pInternalData->m_pBorderBrush = new D2DSolidColorBrush();
        m_pInternalData->m_pBorderBrush->CreateBrush(pRenderTarget);
    }

    ID2D1Brush *pD2DBorderBrush = NULL;
    m_pInternalData->m_pBorderBrush->GetD2DBrush(&pD2DBorderBrush);
    if (NULL != pD2DBorderBrush)
    {
        static_cast<ID2D1SolidColorBrush*>(pD2DBorderBrush)->SetColor(m_pInternalData->m_borderColor);
        // Draw round rectangle or rectangle.
        if (VIEW_STATE_SHOWROUNDCORNER == (GetState() & VIEW_STATE_SHOWROUNDCORNER))
        {
            D2D1_ROUNDED_RECT roundRc =
            {
                absRc,
                m_pInternalData->m_fRoundCornerRadiusX,
                m_pInternalData->m_fRoundCornerRadiusY
            };
            pRenderTarget->DrawRoundedRectangle(roundRc, pD2DBorderBrush, fBorderWidth);
        }
        else
        {
            pRenderTarget->DrawRectangle(absRc, pD2DBorderBrush, fBorderWidth);
        }
    }

    SAFE_RELEASE(pD2DBorderBrush);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnDrawSelect(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F& absRc)
{
    if ( !IsSelected() || (NULL == pRenderTarget) )
    {
        return;
    }

    if (NULL == m_pInternalData->m_pSelectBrush)
    {
        m_pInternalData->m_pSelectBrush = new D2DSolidColorBrush();
        m_pInternalData->m_pSelectBrush->CreateBrush(pRenderTarget);
    }

    ID2D1Brush *pD2DSelectBrush = NULL;
    m_pInternalData->m_pSelectBrush->GetD2DBrush(&pD2DSelectBrush);
    if (NULL != pD2DSelectBrush)
    {
        static_cast<ID2D1SolidColorBrush*>(pD2DSelectBrush)->SetColor(m_pInternalData->m_selectColor);
        D2D1_RECT_F absTempRc = absRc;
        D2DRectUtility::InflateD2DRectF(absTempRc, -1.0f, -1.0f);
        pRenderTarget->DrawRectangle(absTempRc, pD2DSelectBrush, 3.0f);
    }

    SAFE_RELEASE(pD2DSelectBrush);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnDrawBackground(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F& absRc)
{
    // No do draw background color or image, this may be useful for sub views.
    BOOL isNotPaintBK = (m_pInternalData->m_nViewState & VIEW_STATE_NOTPAINTBK);
    if (isNotPaintBK)
    {
        return;
    }

    // Draw view's background image.
    D2DBitmap *pBKD2DBitmap = m_pInternalData->m_pBKD2DBitmap;
    if (NULL != pBKD2DBitmap)
    {
        if (!pBKD2DBitmap->HasInitialized())
        {
            pBKD2DBitmap->Initialize(pRenderTarget);
        }

        ID2D1Bitmap *pD2DBitmap = NULL;
        pBKD2DBitmap->GetD2DBitmap(&pD2DBitmap);
        if (NULL != pD2DBitmap)
        {
            pRenderTarget->DrawBitmap(pD2DBitmap, absRc);
        }
        SAFE_RELEASE(pD2DBitmap);
    }
    else
    {
        // Draw view's background color.
        BOOL isShowRoundCorner = (VIEW_STATE_SHOWROUNDCORNER == (GetState() & VIEW_STATE_SHOWROUNDCORNER));
        BOOL isDrawBKColor = (VIEW_STATE_DRAWBKCOLOR == (GetState() & VIEW_STATE_DRAWBKCOLOR));
        if ( !isDrawBKColor || (NULL == m_pInternalData->m_pD2DBrush) )
        {
            return;
        }

        if (!m_pInternalData->m_pD2DBrush->HasCreatedBrush())
        {
            m_pInternalData->m_pD2DBrush->CreateBrush(pRenderTarget);
        }

        ID2D1Brush *pD2DBrush = NULL;
        m_pInternalData->m_pD2DBrush->GetD2DBrush(&pD2DBrush);
        if (NULL == pD2DBrush)
        {
            return;
        }

        // Set the color according to the alpha animation.
        if (NULL != m_pInternalData->m_pAnimation)
        {
            TRANSFORMINFO info;
            if (SUCCEEDED(m_pInternalData->m_pAnimation->GetTransform(&info)))
            {
                pD2DBrush->SetOpacity((FLOAT)info.dAlpha);
            }
        }

        // Set the background color.
        m_pInternalData->m_pD2DBrush->SetColor(m_pInternalData->m_bkColor);
        if (isShowRoundCorner)
        {
            D2D1_ROUNDED_RECT roundRc =
            {
                absRc,
                m_pInternalData->m_fRoundCornerRadiusX,
                m_pInternalData->m_fRoundCornerRadiusY
            };
            pRenderTarget->FillRoundedRectangle(roundRc, pD2DBrush);
        }
        else
        {
            pRenderTarget->FillRectangle(absRc, pD2DBrush);
        }

        SAFE_RELEASE(pD2DBrush);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnAnimationStart()
{
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnAnimationEnd()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::OnMessageEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::OnTouchEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);

    if (!IsEnable() || !IsVisible())
    {
        return FALSE;
    }

    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isTouchOn = IsPtInRect(xPos, yPos);
    BOOL handled = FALSE;

    switch (message)
    {
    case WM_RBUTTONDOWN:
        {
            if (isTouchOn)
            {
                m_pInternalData->m_preMousePt.x = (LONG)xPos;
                m_pInternalData->m_preMousePt.y = (LONG)yPos;
                AddFlag(VIEW_STATE_RIGHTPRESSED);
                RemoveFlag(VIEW_STATE_CANCELEVENT);
                handled = ((NULL != m_pInternalData->m_pLongClickHandler) || IsLongClickable());
            }
        }
        break;

    case WM_RBUTTONUP:
        {
            BOOL isRPressed = (VIEW_STATE_RIGHTPRESSED == (GetState() & VIEW_STATE_RIGHTPRESSED));
            BOOL isCancel = (VIEW_STATE_CANCELEVENT == (GetState() & VIEW_STATE_CANCELEVENT));
            if (isTouchOn && isRPressed && IsLongClickable() && !isCancel)
            {
                handled = PerformLongClick();
            }
            RemoveFlag(VIEW_STATE_PRESSED);
            RemoveFlag(VIEW_STATE_CANCELEVENT);
        }
        break;

    case WM_LBUTTONDOWN:
        {
            if (isTouchOn)
            {
                m_pInternalData->m_preMousePt.x = (LONG)xPos;
                m_pInternalData->m_preMousePt.y = (LONG)yPos;
                AddFlag(VIEW_STATE_PRESSED);
                RemoveFlag(VIEW_STATE_CANCELEVENT);
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
            BOOL isCancel = (VIEW_STATE_CANCELEVENT == (GetState() & VIEW_STATE_CANCELEVENT));
            BOOL isCancelPerform = IsCancelPerformClick((LONG)xPos, (LONG)yPos);
            BOOL isClickable = IsClickable();
            RemoveFlag(VIEW_STATE_PRESSED);
            RemoveFlag(VIEW_STATE_CANCELEVENT);
            if (isTouchOn && isPressed && isClickable && !isCancel && !isCancelPerform)
            {
                // Add this view to event chain, Click operation will finish after all events are processed.
                GetWindow()->PushEventView(this);
            }
        }
        break;

    case WM_MOUSEMOVE:
        {
            BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
            BOOL isCancelPerform = IsCancelPerformClick((LONG)xPos, (LONG)yPos);
            if (isPressed && isCancelPerform)
            {
                AddFlag(VIEW_STATE_CANCELEVENT);
            }
        }
        break;
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::PushClip(ID2D1RenderTarget *pRenderTarget)
{
    BOOL isShowRoundCorner = (VIEW_STATE_SHOWROUNDCORNER == (GetState() & VIEW_STATE_SHOWROUNDCORNER));
    BOOL isClipEnable = (VIEW_STATE_CLIPVIEW == (GetState() & VIEW_STATE_CLIPVIEW));
    // If clip is enable and showing round corner, using layer to clip bitmap.
    if (isShowRoundCorner && isClipEnable)
    {
        D2D1_RECT_F absRc = { 0, 0 };
        GetAbsoluteRect(absRc);

        ID2D1Factory *pD2DFactory = NULL;
        ID2D1RoundedRectangleGeometry *pRoundRcGeometry = NULL;
        m_pWindow->GetD2DDevices()->GetD2DFactory(&pD2DFactory);
        D2D1_ROUNDED_RECT roundRc =
        {
            absRc,
            m_pInternalData->m_fRoundCornerRadiusX,
            m_pInternalData->m_fRoundCornerRadiusY
        };
        pD2DFactory->CreateRoundedRectangleGeometry(roundRc, &pRoundRcGeometry);

        if (NULL == m_pInternalData->m_pClipLayer)
        {
            pRenderTarget->CreateLayer(&m_pInternalData->m_pClipLayer);
        }

        if (NULL != m_pInternalData->m_pClipLayer)
        {
            pRenderTarget->PushLayer(LayerParameters(D2D1::InfiniteRect(), pRoundRcGeometry), m_pInternalData->m_pClipLayer);
        }

        SAFE_RELEASE(pRoundRcGeometry);
        SAFE_RELEASE(pD2DFactory);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::PopClip(ID2D1RenderTarget *pRenderTarget)
{
    BOOL isShowRoundCorner = (VIEW_STATE_SHOWROUNDCORNER == (GetState() & VIEW_STATE_SHOWROUNDCORNER));
    BOOL isClipEnable = (VIEW_STATE_CLIPVIEW == (GetState() & VIEW_STATE_CLIPVIEW));
    // If clip is enable and showing round corner, using layer to clip bitmap.
    if (isShowRoundCorner && isClipEnable)
    {
        pRenderTarget->PopLayer();
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::PerformClick()
{
    if ( (NULL != m_pInternalData->m_pClickHandler) && IsClickable() && IsEnable() )
    {
        m_pInternalData->m_pClickHandler->OnClick(this);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::PerformLongClick()
{
    if ( (NULL != m_pInternalData->m_pLongClickHandler) && IsLongClickable() && IsEnable() )
    {
        return m_pInternalData->m_pLongClickHandler->OnLongClick(this);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::PerformOnMouseEvent(const LPMSG lpMsg, ViewElement *pSource)
{
    BOOL handled = FALSE;

    if (IsEnable() && IsVisible())
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

BOOL ViewElement::PerformOnTouchEvent(const LPMSG lpMsg, ViewElement *pSource)
{
    BOOL handled = FALSE;

    if (IsEnable() && IsVisible())
    {
        if (NULL != m_pInternalData->m_pViewTouchHandler)
        {
            handled = m_pInternalData->m_pViewTouchHandler->OnTouch(lpMsg, this, pSource);
        }

        if (!handled)
        {
            handled = OnTouchEvent(lpMsg->message, lpMsg->wParam, lpMsg->lParam);
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::PerformOnKeyEvent(const LPMSG lpMsg, ViewElement *pSource)
{
    BOOL handled = FALSE;

    if (IsEnable() && IsVisible())
    {
        if (NULL != m_pInternalData->m_pViewKeyHandler)
        {
            handled = m_pInternalData->m_pViewKeyHandler->OnKey(lpMsg, this, pSource);
        }

        if (!handled)
        {
            handled = OnKeyEvent(lpMsg->message, lpMsg->wParam, lpMsg->lParam);
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewElement::IsCancelPerformClick(LONG xPos, LONG yPos)
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

void ViewElement::AddFlag(VIEW_STATE state)
{
    m_pInternalData->m_nViewState |= state;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::RemoveFlag(VIEW_STATE state)
{
    m_pInternalData->m_nViewState &= ~state;
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::CombineTSRMatrix()
{
    if (m_pInternalData->m_hasTSRMatrix)
    {
        m_pInternalData->m_viewMatrix = GetTSRMatrix();
        m_pInternalData->m_hasTSRMatrix = FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewElement::ForceInvalidate()
{
    if (NULL != m_pWindow)
    {
        //m_pWindow->AddWindowState(WINDOW_STATE_INVALIDATE);
        m_pWindow->Invalidate(TRUE);
    }
}
