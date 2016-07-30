/*!
* @file D3DDevice.cpp 
* 
* @brief This file defines the base functions of D3DDevice.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#include "D3DDevice.h"

//////////////////////////////////////////////////////////////////////////

D3DDevice::D3DDevice():m_pd3dDevice(NULL),m_pD3D(NULL),m_hWnd(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

D3DDevice::~D3DDevice()
{
    SAFE_RELEASE(m_pd3dDevice);
    SAFE_RELEASE(m_pD3D);
    m_hWnd = NULL;
}

//////////////////////////////////////////////////////////////////////////

BOOL D3DDevice::InitDevice( HWND hWnd )
{
    m_hWnd = hWnd;
    RECT rect = {0};
    if (!IsWindow( hWnd ))
    {
        CommonHelper::PrintDebugString(L"Error: not valid window hwnd!");
        return FALSE;
    }
    // create directX 9.0 version
    m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
    if (!(m_pD3D)) 
    {
        CommonHelper::PrintDebugString(L"Error: Can not create Direct9!");
        exit(0);
    }

    ::GetWindowRect(hWnd, &rect);
    

    // get default display mode
    D3DDISPLAYMODE displayMode;
    ZeroMemory( &displayMode, sizeof(displayMode));
    m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

    // setting the create device parameter.
    ZeroMemory(&m_pParameters, sizeof (m_pParameters));
    m_pParameters.Windowed = TRUE;                                                    // d3d shall be embedded in window
    m_pParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;                                // discard mode for swap buffer
    m_pParameters.EnableAutoDepthStencil = TRUE;                                    // using auto-mode depth stencil
    m_pParameters.AutoDepthStencilFormat = D3DFMT_D24S8;                            // 32bit per pixel for depth stencil
    m_pParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;                // immediate present mode
    m_pParameters.MultiSampleType = D3DMULTISAMPLE_NONE;//D3DMULTISAMPLE_4_SAMPLES;                        // 4 times multi-sample
    m_pParameters.MultiSampleQuality = 0;//2;                                            // 2 quality
    m_pParameters.BackBufferFormat = displayMode.Format;                            // use the supported format
    m_pParameters.BackBufferWidth = m_d3dWidth = rect.right - rect.left;                            // setting width of back buffer
    m_pParameters.BackBufferHeight = m_d3dHeight = rect.bottom - rect.top;                        // setting height of back buffer
    m_pParameters.BackBufferCount = 1;                                                // setting back buffer count

    HRESULT hr = S_OK;

    // using hardware to create device
    if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
        D3DDEVTYPE_HAL, 
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &m_pParameters, 
        &m_pd3dDevice))) 
    {
        // if we create device by software processing, 
        // we shall remove multi-sample and quality setting
        m_pParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        m_pParameters.MultiSampleQuality = 0;

        // create device in software process mode
        if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
            D3DDEVTYPE_HAL, 
            hWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &m_pParameters, 
            &m_pd3dDevice))) 
        {
            // create device in software process mode
            if (FAILED(hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
                D3DDEVTYPE_HAL, 
                hWnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                &m_pParameters, 
                &m_pd3dDevice))) 
            {
                CommonHelper::PrintDebugString(L"Can not create 3D device %x\n", hr);
                SetLastErrorEx(hr,0);
                return FALSE;
            }
        }
    }
    SetupDevice();
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void D3DDevice::SetupDevice()
{
    m_pd3dDevice->LightEnable(0, FALSE);
    //m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    //m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    //m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    //m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,         FALSE );                // set light mode
    //m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,     TRUE );                // enable dithering
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );                // open Z-buffer
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,         D3DCULL_CW );            // set cull mode
    //m_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );                // set normal auto-calculate mode
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );                    // enable write depth buffer

    //m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
    //m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
    //m_pd3dDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x00000000);

    m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    // set blending factors so that alpha component determines transparency
    m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//////////////////////////////////////////////////////////////////////////

UINT32 D3DDevice::GetWidth()
{
    return m_d3dWidth;
}

//////////////////////////////////////////////////////////////////////////

UINT32 D3DDevice::GetHeight()
{
    return m_d3dHeight;
}

//////////////////////////////////////////////////////////////////////////

LPDIRECT3DDEVICE9 D3DDevice::GetDrawingDevice()
{
    return m_pd3dDevice;
}

//////////////////////////////////////////////////////////////////////////

BOOL D3DDevice::ResetDevice()
{
    if ( m_pd3dDevice != NULL )
    {
        HRESULT hr = m_pd3dDevice->Reset( &m_pParameters );
        if ( SUCCEEDED(hr) )
        {
            SetupDevice();
            return TRUE;
        }
        CommonHelper::PrintDebugString(L"d3device reset 0x%x\n", hr);
    }
    return FALSE;
}

VOID D3DDevice::Resize(IN UINT32 w, IN UINT32 h)
{
    m_pParameters.BackBufferWidth = m_d3dWidth = w;
    m_pParameters.BackBufferHeight = m_d3dHeight = h;
}