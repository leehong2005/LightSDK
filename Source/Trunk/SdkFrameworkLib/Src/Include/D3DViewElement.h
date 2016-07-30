/*!
* @file D3DViewElement.h 
* 
* @brief This file defines the base functions of D3DViewElement.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DVIEWELEMENT_H_
#define _D3DVIEWELEMENT_H_

#include "D3DDevice.h"
#include "SdkAnimation.h"
#include "SdkUICommon.h"

USING_NAMESPACE_ANIMATION

BEGIN_NAMESPACE_D3D

class ID3DViewEventHandler;
class D3DCamera;

class CLASS_DECLSPEC D3DViewElement
{
public:

    friend class SdkWindow;

    /*!
    * @brief The constructor function.
    */
    D3DViewElement(void);

    /*!
    * @brief The destructor function.
    */
    virtual ~D3DViewElement(void);

    /*!
    * @brief paint view element.
    */
    virtual void OnPaint();

    /*!
    * @brief Invalidate view element.
    */
    virtual void Invalidate();

    /*!
    * @brief return the window
    *
    * @return window
    */
    virtual SdkWindow* GetWindow();
    
    /*!
    * @brief bring element to top
    *
    */
    virtual void BringViewToTop();

    /*!
    * @brief set animation
    *
    * @param pAnimation         [I/ ] set animation
    */
    virtual void SetAnimation(IN SdkAnimation *pAnimation);

    /*!
    * @brief start animation
    */
    virtual void StartAnimation();

    /*!
    * @brief clear animation
    */
    virtual void ClearAnimation();

    /*!
    * @brief on animation start event
    */
    virtual void OnAnimationStart();

    /*!
    * @brief on animation end event
    */
    virtual void OnAnimationEnd();

    /*!
    * @brief set world matrix
    *
    * @param pMatValue            set world matrix
    */
    virtual void SetWorldMatrix( IN const D3DXMATRIX* pMatValue );

    /*!
    * @brief get world matrix
    *
    * @param pMatOut            set world matrix
    *
    * @return return world matrix
    */
    virtual D3DXMATRIX* GetWorldMatrix( OUT D3DXMATRIX* pMatOut );

    /*!
    * @brief Touch event, such as windows 7 multi-touch.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnTouchEvent( IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent( IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

    /*!
    * @brief Keyboard event.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Get Points.
    *
    * @param pOutVectors        [ /O] the points
    *
    */
    virtual void GetPoints( OUT vector<D3DXVECTOR3>& pOutVectors );

    /*!
    * @brief Get D3D device.
    *
    * @param pViewElement        the D3D view element
    *
    * @return return D3DDevice
    */
    static  D3DDevice* GetD3DDevice( IN D3DViewElement* pViewElement ); 

    /*!
    * @brief SetWindow
    *
    * @param pWindow             set window to the view element
    *
    */
    virtual void SetWindow(SdkWindow *pWindow);

    virtual void CalcWorldMatrix(D3DXMATRIX* outMatrix);

    virtual void SetShowView( BOOL val ) 
    { 
        m_isShown = val; 
    };

    virtual BOOL GetShowView() 
    { 
        return m_isShown ;
    };

    virtual void SetParent( D3DViewElement* pParent )
    {
        m_pParent = pParent;
    };

    virtual D3DViewElement* GetParent()
    {
        return m_pParent;
    };

    virtual void SetEventHandler( ID3DViewEventHandler* pHandler )
    {
        m_pEventHandler = pHandler;
    }

    virtual ID3DViewEventHandler* GetEventHandler()
    {
        return m_pEventHandler;
    }

    virtual void SetCamera( D3DCamera* pCamera )
    {
        m_pCamera = pCamera;
    }

    virtual D3DCamera* GetCamera()
    {
        return m_pCamera;
    }


protected:
    // window
    SdkWindow                    *m_pWindow;
    //    world matrix
    D3DXMATRIX                 m_matWorld;
    //    diffuse color
    D3DCOLOR                 m_dwDiffuseColor;
    //  animation
    SdkAnimation                *m_pAnimation;

    BOOL                     m_isShown;

    D3DViewElement*          m_pParent;

    ID3DViewEventHandler    *m_pEventHandler;

    D3DCamera*              m_pCamera;
};

END_NAMESPACE_D3D

#endif // _D3DVIEWELEMENT_H_
#endif // __cplusplus
