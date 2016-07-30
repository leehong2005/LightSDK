/*!
* @file D2DResource.h 
* 
* @brief This file defines class D2DResource, which is the base class of all D2D resources class,
*        such as Bitmap, Brush, Layer, ,etc.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/27
*/

#ifdef __cplusplus
#ifndef _D2DRESOURCE_H_
#define _D2DRESOURCE_H_

#include "Common.h"

class D2DDevice;

/*!
* @brief The base class of D2D resources class.
*/
class CLASS_DECLSPEC D2DResource
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DResource();

    /*!
    * @brief The destructor function.
    */
    virtual ~D2DResource();

    /*!
    * @brief Set D2DDevice pointer.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    */
    virtual void SetD2DDevice(IN D2DDevice *pD2DDevice);

    /*!
    * @brief Get D2DDevice pointer.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    *
    * @return D2DDevice pointer, you should NOT delete this pointer.
    */
    virtual D2DDevice* GetD2DDevice()  const;

protected:

    D2DDevice       *m_pD2DDevice;          // The D2DDevice pointer.
};

#endif // _D2DRESOURCE_H_
#endif // __cplusplus