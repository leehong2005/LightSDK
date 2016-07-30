/*!
* @file D3DDevice.h 
* 
* @brief This file defines the base functions of D3DDevice.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DDEVICE_H_
#define _D3DDEVICE_H_

#include "CommonInclude.h"
#include <d3d9.h>
#include <d3dx9.h>

class CLASS_DECLSPEC D3DDevice
{
public:

    /*!
    * @brief
    * the construction function.
    */
    D3DDevice(void);

    /*!
    * @brief
    * the destruction function.
    */
    ~D3DDevice(void);

    /*!
    * @brief init D3D device.
    *
    * @param hWnd     [I/ ] the D3D window hwnd.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    BOOL InitDevice( IN HWND hWnd );

    void Resize( IN UINT32 w, IN UINT32 h);

    /*!
    * @brief return native D3D device.
    *
    * @return native D3D device.
    */
    LPDIRECT3DDEVICE9 GetDrawingDevice();

    /*!
    * @brief return D3D device width.
    *
    * @return device width.
    */
    UINT32 GetWidth();

    /*!
    * @brief return D3D device height.
    *
    * @return device height.
    */
    UINT32 GetHeight();

    /*!
    * @brief Reset the device.
    *
    * @return TRUE if succeed, otherwise return FALSE.
    */
    BOOL ResetDevice();

protected:

    void SetupDevice();

protected:

    HWND                    m_hWnd;             // D3D device window hwnd
    UINT32                  m_d3dHeight;        // D3D device height
    UINT32                  m_d3dWidth;         // D3D device width
    LPDIRECT3DDEVICE9       m_pd3dDevice;       // D3D device interface
    LPDIRECT3D9             m_pD3D;             // D3D9 interface
    D3DPRESENT_PARAMETERS   m_pParameters;      // D3D device parameters
};

#endif // _D3DDEVICE_H_
#endif // __cplusplus
