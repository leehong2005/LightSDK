/*!
* @file D3DViewElement.h 
* 
* @brief This file defines the base functions of D3DViewLayout.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DVIEWLAYOUT_H_
#define _D3DVIEWLAYOUT_H_

#include "SdkCommonInclude.h"
#include "D3DViewElement.h"
#include "SdkUICommon.h"

BEGIN_NAMESPACE_D3D

class CLASS_DECLSPEC D3DViewLayout : public D3DViewElement
{
public:

    friend class SdkWindow;

    /*!
    * @brief The constructor function.
    */
    D3DViewLayout(void);

    /*!
    * @brief The destructor function.
    */
    virtual ~D3DViewLayout(void);

    /*!
    * @brief paint view element.
    */
    virtual void OnPaint();
    
    /*!
    * @brief layout children elements.
    */
    virtual void OnLayout();

    /*!
    * @brief add element to children.
    *
    * @param pViewElement            element to be added
    */
    virtual void AddChild( IN D3DViewElement* pViewElement );

    /*!
    * @brief get child element by index
    *
    * @param index                    index
    * @param ppOutViewElement        return index element
    */
    virtual void GetChild( IN UINT32 index, IN D3DViewElement** ppOutViewElement );

    virtual void SetWindow(SdkWindow *pWindow);

    virtual BOOL OnMouseEvent( IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

    virtual void SetCamera( D3DCamera* pCamera );

    virtual void RemoveChild( D3DViewElement* pElement );

    virtual void SetShowView( BOOL val );

    /*!
    * @brief bring element to top
    *
    */
    virtual void BringChildViewToTop( D3DViewElement* pViewElement );

protected:
    // children list
    vector<D3DViewElement*>  m_vctChildren;
};

END_NAMESPACE_D3D

#endif // _D3DVIEWELEMENT_H_
#endif // __cplusplus
