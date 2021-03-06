/*! 
* @file MouseSpeedTracker.h 
* 
* @brief This file defines the functions to track mouse speed.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _MOUSESPEEDTRACKER_H_
#define _MOUSESPEEDTRACKER_H_

#include "Common.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The class defines the functions to track mouse speed.
*/
class CLASS_DECLSPEC MouseSpeedTracker
{
public:

    /*!
    * @brief The constructor function.
    */
    MouseSpeedTracker();

    /*
    * @brief The default destructor function
    */
    ~MouseSpeedTracker();

    /*!
    * @brief Get the X-speed.
    */
    DOUBLE GetSpeedX();

    /*!
    * @brief Get the Y-speed.
    */
    DOUBLE GetSpeedY();

    /*!
    * @brief Get the speed sample time.
    */
    DWORD GetSpeedSampleTime();

    /*
    * @brief The mouse event procedure.
    *
    * @param hWnd       [I/ ] The handle to the window which occurs the message.
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    */
    void OnMouseEvent(HWND hWnd, UINT32 message, WPARAM wParam, LPARAM lParam);

private:

    DWORD       m_dwElapseTime;     // The elapse timer.
    POINT       m_deltaDist;        // The delta between first point and second point.
    POINT       m_firstPT;          // The first point of mouse.
    POINT       m_secondPT;         // The second point of mouse.
    BOOL        m_isLBDown;         // Indicates whether left button pressed down.
    DOUBLE      m_dSpeedX;          // The X-Speed.
    DOUBLE      m_dSpeedY;          // The Y-Speed.
};

END_NAMESPACE

#endif // _MOUSESPEEDTRACKER_H_
#endif // __cplusplus
