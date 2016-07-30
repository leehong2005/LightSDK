/*!
* @file ImagePreviewLayout.h
* 
* @brief This file defines the view preview layout.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Qiao
* @date 2011/03/15
*/

#ifdef __cplusplus
#ifndef _IMAGEPREVIEWLAYOUT_H_
#define _IMAGEPREVIEWLAYOUT_H_

#include "ViewLayout.h"
#include "AnimatedGifView.h"
#include "TranslateAnimation.h"
#include "ImagePreviewUpdateHandler.h"

class CLASS_DECLSPEC ImagePreviewLayout : public ViewLayout
{
public:

    /*!
    * @brief The constructor function.
    */
    ImagePreviewLayout();

    /*!
    * @brief The destructor function.
    */
    virtual ~ImagePreviewLayout();

    /*!
    * @brief Called to Scroll to the last photo.
    */
    void ScrollUp(BOOL bAutoplay);

    /*!
    * @brief Called to Scroll to the next photo.
    */
    void ScrollDown(BOOL bAutoplay);

    /*!
    * @brief Reset state.
    */
    void Reset();

    /*!
    * @brief Called to get the image from files, which will be set as the background.
    *
    * @param lpFile               [I/ ] The path of the image.
    * @param nWidth               [I/ ] The width of image.
    * @param nHeight              [I/ ] The height of image.
    */
    void SetLastImage(IN LPCWSTR lpFile, INT32 nWidth, INT32 nHeight);

    /*!
    * @brief Called to get the image from files, which will be set as the background.
    *
    * @param lpFile               [I/ ] The path of the image.
    * @param nWidth               [I/ ] The width of image.
    * @param nHeight              [I/ ] The height of image.
    */
    void SetCurImage(IN LPCWSTR lpFile, INT32 nWidth, INT32 nHeight);

    /*!
    * @brief Called to get the image from files, which will be set as the background.
    *
    * @param lpFile               [I/ ] The path of the image.
    * @param nWidth               [I/ ] The width of image.
    * @param nHeight              [I/ ] The height of image.
    */
    void SetNextImage(IN LPCWSTR lpFile, INT32 nWidth, INT32 nHeight);

    /*!
    * @brief Get current file path.
    *
    * @param lpFilePath               [I/ ] The file path.
    * @param nBufferLength            [I/ ] The buffer length.
    */
    void GetCurFilePath(LPWSTR lpFilePath, INT32 nBufferLength);

    /*!
    * @brief Set the time of the animation.
    *
    * @param nDSecond                 [I/ ] The time of the animation.
    */
    void SetAnimationTime(IN DOUBLE nDSecond);

    /*!
    * @brief Set the update image handler.
    *
    * @param pHandler                 [I/ ] The update image handler.
    */
    void SetUpdateImageHandler(ImagePreviewUpdateHandler *pHandler);

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

    /*!
    * @brief Set move index.
    *
    * @param nMoveIndex [I/ ] The move index.
    */
    void SetMoveIndex(INT32 nMoveIndex);

    /*!
    * @brief Set auto play type.
    *
    * @param isAutoPlay [I/ ] The auto play flag.
    */
    void SetAutoPlayType(BOOL isAutoPlay);

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
    VOID OnAnimationTimerUpdate(OUT Animation *pAnimation);

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

private:

    /*!
    * @brief Initialize the children view.
    */
    void Initialize();

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
    * @brief The method is called when the mouse wheel is rotated.
    *
    * @param wParam             [I/ ] Indicates whether various virtual keys are down.
    * @param lParam             [I/ ] The cursor position.
    */
    void OnMouseWheel(WPARAM wParam, LPARAM lParam);

    /*!
    * @brief The image preview layout.
    *
    * @param pLayout            [I/ ] The view layout.
    * @param pImageView         [I/ ] The image view.
    * @param fX                 [I/ ] The x position.
    * @param fY                 [I/ ] The y position.
    * @param fWidth             [I/ ] The width of view.
    * @param fHeight            [I/ ] The height of view.
    */
    void LayoutView(ViewLayout *pLayout, AnimatedGifView *pImageView, FLOAT fX, FLOAT fY, FLOAT fWidth, FLOAT fHeight);

    /*!
    * @brief The set image information.
    *
    * @param pImageView         [I/ ] The image view.
    * @param lpFile             [I/ ] The path of the image.
    * @param fWidth             [I/ ] The width of view.
    * @param fHeight            [I/ ] The height of view.
    */
    void SetImageInfo(AnimatedGifView *pImageView, LPCWSTR lpFile, FLOAT fWidth, FLOAT fHeight);

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

    /*!
    * @brief Get new show index.
    * 
    * @param bNext                  [I/ ] Next or last.
    *
    * @return the new show index.
    */
    INT32 GetNewShowIndex(BOOL bNext);

    /*!
    * @brief Calculate Layout Information.
    * 
    * @param layoutInfoContent      [I/O] The layout information content.
    * @param fWidth                 [I/ ] The width.
    * @param fHeight                [I/ ] The height.
    *
    * @return the new show index.
    */
    void CalcLayoutInfo(IN OUT LAYOUTINFO &layoutInfoContent, FLOAT fWidth, FLOAT fHeight);

    /*!
    * @brief Delete imageView.
    */
    void DeleteImageView();

    /*!
    * @brief Continuous dramatic.
    */
    void ContinuousDramatic();

    /*!
    * @brief Judge gif zoom.
    *
    * @return the new show index.
    */
    BOOL IsGifZoom();

private:

    INT32                       m_nCurIndex;                // The current show index.
    INT32                       m_nNewCurIndex;             // The new current show index.
    INT32                       m_nMoveIndex;               // The move index.
    FLOAT                       m_fZoomRate;                // The current zoom rate
    FLOAT                       m_fOffsetX;                 // The offset X.
    DWORD                       m_dwZoomArguments;          // The zoom argument.
    DOUBLE                      m_dAnimationTime;           // The animation time.    
    BOOL                        m_bLbuttonDown;             // The Left button down flag.
    BOOL                        m_bUpOrDown;                // The up or down flag.
    BOOL                        m_bMove;                    // The flag whether moved or not.
    BOOL                        m_bContinue;                // The flag whether continue or not.
    BOOL                        m_bCanZoom;                 // The flag whether can zoom or not.
    BOOL                        m_bZoom;                    // The flag whether zoom or not.
    BOOL                        m_bCanPanning;              // The flag whether can pan or not.
    BOOL                        m_isAnimationEnd;           // The flag whether animation is end or not.
    BOOL                        m_isAutoplay;               // The flag whether auto play is end or not.
    BOOL                        m_isKeyLastOrUp;            // The flag whether last or up.
    BOOL                        m_isBottomIn;               // The flag whether bottom out or in.

    AnimatedGifView             *m_pLastImageView;          // The last image view.
    AnimatedGifView             *m_pCurImageView;           // The current image view.
    AnimatedGifView             *m_pNextImageView;          // The current image view.
    ViewLayout                  *m_pLastViewLayout;         // The Last View layout
    ViewLayout                  *m_pCurViewLayout;          // The current layout.
    ViewLayout                  *m_pNextViewLayout;         // The next view layout.
    ViewElement                 *m_pCurrentShowView;        // The current show view.
    ImagePreviewUpdateHandler   *m_pUpdateHandler;          // The update image handler.
    TranslateAnimation          *m_pLeftTranslateAnimation; // The left/right translate animation
    TranslateAnimation          *m_pRightTranslateAnimation;// The left/right translate animation
    TranslateAnimation          *m_pDynamicTranslate;       // The dynamic translate animation(left button up)
    
    TCHAR                       m_tcCurFilePath[MAX_PATH];  // The current file path.
    TCHAR                       m_tcLastFilePath[MAX_PATH]; // The last file path.
    TCHAR                       m_tcNextFilePath[MAX_PATH]; // The next file path.

    POINT                       m_lastPoint;                // The last point.
    POINT                       m_downPoint;                // The down point.
    FLOAT                       m_fDisplayWidth;            // The display width.
    FLOAT                       m_fDisplayHeight;           // The display height.
    LAYOUTINFO                  m_curChildLayoutInfo;       // The current show child layout info
};

#endif // _IMAGEPREVIEWLAYOUT_H_
#endif // __cplusplus
