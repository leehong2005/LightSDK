/*!
* @file ViewPreviewEx.h 
* 
* @brief This file defines the view preview layout.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2011/01/25
*/

#ifdef __cplusplus
#ifndef _VIEWPREVIEWEX_H_
#define _VIEWPREVIEWEX_H_

#include "ViewLayout.h"
#include "ISwitchEventHandler.h"
#include "TranslateAnimation.h"
#include "IAnimationListener.h"
#include "IAnimationTimerListener.h"
#include "ImageView.h"
#include "AlphaAnimation.h"

#define  DEFAULT_ELEMENT_WIDTH  200
#define  DEFAULT_ELEMENT_HEIGHT 200

#define  MIN_WIDTH  100
#define  MIN_HEIGHT 100

/*!
* @brief The animation type.
*/
typedef enum _ANI_TYPE
{
    ANIMATION_UNIFORM = 0,

} ANI_TYPE;

/*!
* @brief This class defined the view preview layout.
*/
class CLASS_DECLSPEC ViewPreviewEx : public ViewLayout
{
public:

    /*!
    * @brief The constructor function.
    */
    ViewPreviewEx();

    /*!
    * @brief The destructor function.
    */
    virtual ~ViewPreviewEx();

    /*!
    * @brief Set the element area.
    *
    * @param fWidth             [I/ ] The width.
    * @param fHeight            [I/ ] The height.
    */
    void SetEelementArea(FLOAT fWidth, FLOAT fHeight);
    
    /*!
    * @brief Remove all children view.
    *
    * @param isClearCache   [I/ ] Flag to indicate clear cache views or not.
    *
    * @return True: remove success.
    *               remove failure
    */
    virtual BOOL RemoveAllChildren(BOOL isClearCache = TRUE);

    /*!
    * @brief Set if fit child to window.
    *
    * @param bFit             [I/ ] The value if fit child to window.
    */
    void SetFitChildToWindow(BOOL bFit);

    /*!
    * @brief Add a view to the layout, the layout is the parent view of the added view.
    *
    * @param pChild      [I/ ] The child view.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    *
    * @remark You need not release the memory of the child view, the layout will
    *         release them when the layout view destroy.
    */
    virtual BOOL AddView(IN const ViewElement *pChild);

    /*!
    * @brief Set the start child to play.
    *
    * @param bFit             [I/ ] The value if fit child to window.
    */
    void SetStartChild(INT32 nChildIndex);

    /*!
    * @brief Called to Scroll to the last photo.
    *
    * @param bNeedInvalidate          [I/ ] If need to invalidate.
    */
    void ScrollUp(BOOL bNeedInvalidate = TRUE);

    /*!
    * @brief Called to Scroll to the next photo.
    *
    * @param bNeedInvalidate          [I/ ] If need to invalidate.
    */
    void ScrollDown(BOOL bNeedInvalidate = TRUE);

    /*!
    * @brief Set switch event handler.
    *
    * @param pEventHandler            [I/ ] The ISwitchEventHandler interface.
    */
    virtual void SetSwitchEventHandler(IN ISwitchEventHandler* pEventHandler);

    /*!
    * @brief Get current index.
    *
    * @return current index.
    */
    INT32 GetCurrentIndex();

    /*!
    * @brief Set the animation type.
    *
    * @param type                     [I/ ] The animation type.Include [ANIMATION_GENERAL,
    *                                       ANIMATION_FADE, ANIMATION_UNIFORM].
    */
    void SetAnimationType(IN ANI_TYPE type);

    /*!
    * @brief Set the time of the animation.
    *
    * @param nDSecond                 [I/ ] The time of the animation.
    */
    void SetAnimationTime(IN DOUBLE nDSecond);

    /*!
    * @brief Cancel the photo zoom.
    */
    void CancelZoom();

    /*!
    * @brief Judge if animation is end.
    *
    * @return TRUE if it ends, else return FALSE.
    */
    BOOL IsAnimationEnd();

    /*!
    * @brief Assign to zoom in or out.
    *
    * @param fRate                 [I/ ] The zoom rate.
    * @param pPointCenter          [ /O] The point center to zoom. Default value is NULL.
    */
    void ZoomTo(IN FLOAT fRate, OUT POINT *pPointCenter = NULL);

    /*!
    * @brief Set if photos can be zoomed.
    *
    * @param bCanZoom              [I/ ] The time of the animation.
    */
    void SetCanZoom(BOOL bCanZoom);

    /*!
    * @brief Set gesture.
    *
    * @param bInteria              [I/ ] The gesture type. Default value is FALSE.
    */
    void SetGesture(BOOL bInteria = FALSE);

    /*!
    * @brief Stop the animation.
    */
    void StopAnimation();

protected:

    /*!
    * @brief This method can be called when the animation is about to start.
    * 
    * @param pAnimation            [ /O] The animation.
    */
    void OnAnimationStart(OUT Animation *pAnimation);

    /*!
    * @brief This method can be called when the animation is about to play.
    * 
    * @param pAnimation            [ /O] The animation.
    */
    void OnAnimationPlaying(OUT Animation *pAnimation);

    /*!
    * @brief This method can be called when the animation has ended.
    * 
    * @param pAnimation            [ /O] The animation.
    */
    void OnAnimationEnd(OUT Animation *pAnimation);

    /*!
    * @brief This method can be called when the timer update.
    * 
    * @param pAnimation            [ /O] The animation.
    */
    void OnAnimationTimerUpdate(OUT Animation *pAnimation);

    /*!
    * @brief The method is called when the mouse event occurs.
    *
    * @param message            [I/ ] The mouse message.
    * @param wParam             [I/ ].Indicates whether various virtual keys are down.
    * @param lParam             [I/ ].The cursor position.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Touch event, such as windows 7 multi-touch.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
     */
    virtual BOOL OnTouchEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Called when the layout of view is changed.
    *
    * @param bChanged           [I/ ] Indicates whether is changed.
    * @param left               [I/ ] The left value.
    * @param top                [I/ ] The top value.
    * @param width              [I/ ] The width of view.
    * @param height             [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL bChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

private:

    /*!
    * @brief The method is called when the button press.
    *
    * @param wParam             [I/ ] Indicates whether various virtual keys are down.
    * @param lParam             [I/ ] The cursor position.
    */
    void OnLButtonDown(WPARAM wParam, LPARAM lParam);

    /*!
    * @brief The method is called when the button up.
    *
    * @param wParam             [I/ ] Indicates whether various virtual keys are down.
    * @param lParam             [I/ ] The cursor position.
    */
    void OnLButtonUp(WPARAM wParam, LPARAM lParam);

    /*!
    * @brief The method is called when the mouse move.
    *
    * @param wParam             [I/ ] Indicates whether various virtual keys are down.
    * @param lParam             [I/ ] The cursor position.
    */
    void OnMouseMove(WPARAM wParam, LPARAM lParam);

    /*!
    * @brief The method is called when the mouse leave.
    *
    * @param wParam             [I/ ] Indicates whether various virtual keys are down.
    * @param lParam             [I/ ] The cursor position.
    */
    void OnMouseLeave(WPARAM wParam, LPARAM lParam);

    /*!
    * @brief The method is called when the mouse wheel is rotated.
    *
    * @param wParam             [I/ ] Indicates whether various virtual keys are down.
    * @param lParam             [I/ ] The cursor position.
    */
    void OnMouseWheel(WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Calculate the offset.
    *
    * @return The offset.
    */
    FLOAT CalcOffset();

    /*!
    * @brief Calculate the offset.
    *
    * @param fStartOffset       [ /O] The start offset.
    * @param fEndOffset         [ /O] The end offset.
    */
    void CalcOffset(OUT FLOAT &fStartOffset, OUT FLOAT &fEndOffset);

    /*!
    * @brief To correct the offset.
    *
    * @param bNeedInvalidate          [I/ ] If need to invalidate.
    */
    void CorrectOffset(BOOL bNeedInvalidate);

    /*!
    * @brief Perform the switch event handler.
    */
    void PerformSwitch();

    /*!
    * @brief Resize the frame.
    *
    * @param fWidth                   [I/ ] The width of frame.
    * @param fHeight                  [I/ ] The width of frame.
    */
    void ResizeFrame(IN FLOAT fWidth, IN FLOAT fHeight);

    /*!
    * @brief Called when photo zoom out.
    *
    * @param fRate                    [I/ ] The zoom rate. Default value is 1.0f.
    * @param pPointCenter             [ /O] The point center to zoom. Default value is NULL.
    */
    void ZoomOut(IN FLOAT fRate = 1.0f, OUT POINT *pPointCenter = NULL);

    /*!
    * @brief Called when photo zoom in.
    *
    * @param fRate                    [I/ ] The zoom rate. Default value is 1.0f.
    * @param pPointCenter             [ /O] The point center to zoom. Default value is NULL.
    */
    void ZoomIn(IN FLOAT fRate = 1.0f, OUT POINT *pPointCenter = NULL);

    /*!
    * @brief Zoom photo.
    *
    * @param fRate                    [I/ ] The zoom rate.
    * @param pPointCenter             [ /O] The point center to zoom. Default value is NULL.
    */
    void Zoom(IN FLOAT fRate, OUT POINT *pPointCenter = NULL);

    /*!
    * @brief Panning the photo.
    *
    * @param pointLast                [ /O] The last point.
    * @param pointCur                 [ /O] The new point.
    */
    void DoPanning(OUT const POINT &pointLast, OUT const POINT &pointCur);

    /*!
    * @brief Update photo and Index.
    */
    void UpdateCurShowIndex();

    /*!
    * @brief Stop the animation.
    * 
    * @param pAnimation               [ /O] The animation.
    */
    void StopAnimation(OUT Animation *pAnimation);

    /*!
    * @brief Start the animation.
    * 
    * @param isStart                  [I/ ] A flag if is started.
    * @param bLeft                    [I/ ] A flag if is left translate animation.
    */
    void StartAnimation(IN BOOL isStart, IN BOOL bLeft);

protected:

    FLOAT                   m_fElementHeight;           // The element height.
    FLOAT                   m_fElementWidth;            // The element width.
    FLOAT                   m_fRealWidth;               // The Real width.
    FLOAT                   m_fOffsetX;                 // The offset x.
    FLOAT                   m_fBackOffsetX;             // The offset x.
    FLOAT                   m_fZoomRate;                // The current zoom rate
    DOUBLE                  m_dAnimationTime;           // The animation time
    ANI_TYPE                m_animationType;            // The animation type
    POINT                   m_lastPoint;                // The last point.
    BOOL                    m_bLbuttonDown;             // The Left button down flag.
    BOOL                    m_bUpOrDown;                // The up or down flag.
    BOOL                    m_bMove;                    // The flag whether moved or not.
    BOOL                    m_isAnimationEnd;           // The flag whether animation is end or not.
    BOOL                    m_bFitChildToWindow;        // The fit child to window flag.
    BOOL                    m_bZoom;                    // The flag whether zoom or not.
    BOOL                    m_bCanPanning;              // The flag whether can pan or not.
    BOOL                    m_bCanZoom;                 // The flag whether can zoom or not.
    BOOL                    m_bFront;                   // The flag whether front or not.
    BOOL                    m_bButtonAnimationIsEnd;    // The flag is button animation whether end or not.
    INT32                   m_nCurShowIndex;            // The start photo index.
    DWORD                   m_dwZoomArguments;          // The zoom argument.
    LAYOUTINFO              m_curChildLayoutInfo;       // The current show child layout info
    ISwitchEventHandler    *m_pSwitchEventHandler;      // The switch event handler.
    TranslateAnimation     *m_pLeftTranslateAnimation;  // The left/right translate animation
    TranslateAnimation     *m_pRightTranslateAnimation; // The left/right translate animation
    TranslateAnimation     *m_pDynamicTranslate;        // The dynamic translate animation(left button up)

    vector<ViewElement*>    m_vecFrame;                 // The frame array.
    vector<ViewElement*>    m_vecContent;               // The photos array.
};

#endif // _VIEWPREVIEWEX_H_
#endif // __cplusplus
