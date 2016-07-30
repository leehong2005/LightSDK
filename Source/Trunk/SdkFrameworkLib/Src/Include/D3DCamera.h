/*!
* @file D3DBasicAnimation.h 
* 
* @brief This file defines the base functions of D3DBasicAnimation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DCAMERA_H_
#define _D3DCAMERA_H_

#include <d3d9.h>
#include <d3dx9.h>

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_D3D

class D3DDevice;

/*!
* @brief The class process the eye and look position ,and device's viewMatrix and projectMatrix.
*/
class CLASS_DECLSPEC D3DCamera
{
public:
    /*!
    * @brief
    * the construction function.
    */
    D3DCamera();
    /*!
    * @brief
    * the destruction function.
    */
    virtual ~D3DCamera();

    /*!
    * @brief
    * set and return the viewMatrix of the device.
    * 
    * @param pDevice
    * pointer to a d3d device.
    *
    * @returns
    * return the viewMatrix.
    */
    //const D3DXMATRIX* setupView( const D3DXVECTOR3& vectEye, 
    //                             const D3DXVECTOR3& vectLookat, 
    //                             LPDIRECT3DDEVICE9 pDevice  );

    //const D3DXMATRIX* setupView( 
    //    const D3DXVECTOR3& vectEye, 
    //    const D3DXVECTOR3& vectLookat, 
    //    D3DDevice* pD3DDevice  );

    /*!
    * @brief
    * set and return the viewMatrix of the device.
    *
    * @param vectEye
    * pointer to an eye position.
    *
    * @param vectLookat
    * pointer to the look at position.
    *
    * @param vectUp
    * pointer to the up direction.
    *
    * @param pDevice
    * pointer to a d3d device.
    *
    * @returns
    * return the viewMatrix.
    */
    const D3DXMATRIX* setupView( 
        const D3DXVECTOR3& vectEye, 
        const D3DXVECTOR3& vectLookat,
        const D3DXVECTOR3& vectUp,
        D3DDevice* pD3DDevice  );

    ///*!
    //* @brief
    //* set and return the projMatrix of the device.
    //* 
    //* @param pDevice
    //* pointer to a d3d device.
    //*
    //* @returns
    //* return the projMatrix.
    //*/
    //const D3DXMATRIX* setupProj(float width, float height,  LPDIRECT3DDEVICE9 pDevice );

    //const D3DXMATRIX* setupProj(float fov, float aspect, float zn, float zf, LPDIRECT3DDEVICE9 pDevice );

    /*!
    * @brief
    * set and return the projMatrix of the device.
    *
    * @param fov
    * pointer to a d3d device.
    *
    * @param aspect
    * pointer to a d3d device.
    *
    * @param zn
    * pointer to a d3d device.
    *
    * @param zf
    * pointer to a d3d device.
    * 
    * @param pDevice
    * pointer to a d3d device.
    *
    * @returns
    * return the projMatrix.
    */
    const D3DXMATRIX* setupProj(float fov, float aspect, float zn, float zf, D3DDevice* pDevice );

    //const D3DXMATRIX* setupProj( D3DDevice* pDevice );

    /*!
    * @brief
    * get device's viewMatrix.
    * 
    * @returns
    * return viewMatrix.
    */
    const D3DXMATRIX* getViewMatrix()
    {
        return &m_MatView;
    }

    /*!
    * @brief
    * get device's projMatrix.
    * 
    * @returns
    * return projMatrix.
    */
    const D3DXMATRIX* getProjMatrix()
    {
        return &m_MatProj;
    }

    /*!
    * @brief
    * get Eye's position.
    * 
    * @returns
    * return the eye's position.
    */
    const D3DXVECTOR3* getEyeAt()
    {
        return &m_EyeAt;
    }

    /*!
    * @brief
    * get look's position.
    * 
    * @returns
    * return the look's position.
    */
    const D3DXVECTOR3* getLookAt()
    {
        return &m_LookAt;
    }

    /*!
    * @brief
    * get eye change matrix.
    * 
    * @returns
    * get change matrix.
    */
    const D3DXMATRIX* getEyaAtMatrix( OUT D3DXMATRIX* outEyeAt )
    {
        if ( outEyeAt != NULL )
        {
            *outEyeAt = m_MatEyeAt;
        }
        return &m_MatEyeAt;
    }

    /*!
    * @brief
    * get eye change matrix.
    * 
    * @returns
    * get change matrix.
    */
    const D3DXMATRIX* getLookAtMatrix( OUT D3DXMATRIX* outLookAt )
    {
        if ( outLookAt != NULL )
        {
            *outLookAt = m_MatLookAt;
        }
        return &m_MatLookAt;
    }

private:
    //! viewMatrix
    D3DXMATRIX m_MatView;
    //! projMatrix
    D3DXMATRIX m_MatProj;
    //! eye position
    D3DXVECTOR3 m_EyeAt;
    //! look position
    D3DXVECTOR3 m_LookAt;
    //! eye change matrix
    D3DXMATRIX m_MatEyeAt;
    //! look change matrix
    D3DXMATRIX m_MatLookAt;
};

END_NAMESPACE_D3D

#endif // _D3DCAMERA_H_
#endif // __cplusplus
