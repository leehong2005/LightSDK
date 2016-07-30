/*!
* @file IViewEventHandler.h 
* 
* @brief This file defines the event for view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _ID3DVIEWEVENTHANDLER_H_
#define _ID3DVIEWEVENTHANDLER_H_

class D3DViewElement;

/*!
* @brief The IViewEventHandler defines the event for view.
*/
class CLASS_DECLSPEC ID3DViewEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ID3DViewEventHandler() {};

    /*!
    * @brief The destructor function.
    *
    * @param view   [I/ ] The pointer ViewElement, you should NOT delete the pointer.
    */
    virtual BOOL OnMouseMessage( D3DViewElement* pTarget, D3DViewElement* pSender, UINT message, WPARAM wParam, LPARAM lParam ) 
    {
		UNREFERENCED_PARAMETER(pTarget);
		UNREFERENCED_PARAMETER(pSender);
		UNREFERENCED_PARAMETER(message);
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

        return FALSE;
    };
};

#endif // _IVIEWEVENTHANDLER_H_
#endif // __cplusplus