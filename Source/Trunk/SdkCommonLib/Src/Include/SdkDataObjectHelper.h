/*!
* @file SdkDataObjectHelper.h
* 
* @brief The file define the class to create SdkDataObject object.
*
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#ifdef __cplusplus
#ifndef _SDKDATAOBJECTFACTORY_H_
#define _SDKDATAOBJECTFACTORY_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

class CLASS_DECLSPEC SdkDataObjectHelper
{
public:

    /*!
    * @brief Create the instance of SdkDataObject object.
    *
    * @param riid   [I/ ] The id of interface.
    * @param ppv    [/ O] The output instance.
    *
    * @return S_OK if succeed, otherwise other value.
    */
    static HRESULT WINAPI CreateDataObject(REFIID riid, void **ppv);

    /*!
    * @brief Create the instance of SdkDataObject from specified IDataObject instance.
    *
    * @param pDataObjectSrc     [I/ ] The source data object.
    * @param pDataObjectDest    [I/O] The destination data object.
    *
    * @return S_OK if succeeds, otherwise E_FAIL.
    */
    static HRESULT WINAPI CopyDataObject(IDataObject *pDataObjectSrc, IDataObject *pDataObjectDest);

    /*!
    * @brief Set the tip of drag data.
    *
    * @param pDataObject    [I/ ] The data object.
    * @param pDropDesc      [I/ ] The message, text such as "Move to %1.
    *
    * @return S_OK if succeeds, otherwise E_FAIL.
    */
    static HRESULT WINAPI SetDropTip(IDataObject *pDataObject, const DROPDESCRIPTION *pDropDesc);

    /*!
    * @brief Get the tip of drag data.
    *
    * @param pDataObject    [I/ ] The data object.
    * @param pDropDesc      [ /O] The message, text such as "Move to %1.
    *
    * @return S_OK if succeeds, otherwise E_FAIL.
    */
    static HRESULT WINAPI GetDropTip(IDataObject *pDataObject, DROPDESCRIPTION *pDropDesc);

    /*!
    * @brief Set the data blob. The data type is TYMED_HGLOBAL.
    *
    * @param pDataObject    [I/ ] The data object.
    * @param cf             [I/ ] The clip format.
    * @param pvBlob         [I/ ] Pointer to blob.
    * @param cbBlob         [I/ ] The bytes of the blob.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    static HRESULT WINAPI SetDataBlob(IDataObject *pDataObject, CLIPFORMAT cf, const void *pvBlob, UINT cbBlob);

    /*!
    * @brief Get the data blob. The data type is TYMED_HGLOBAL.
    *
    * @param pDataObject    [I/ ] The data object.
    * @param cf             [I/ ] The clip format.
    * @param ppvBlob        [ /O] Pointer to blob.
    * @param pcbBlob        [ /O] The bytes of the blob.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    static HRESULT WINAPI GetDataBlob(IDataObject *pDataObject, CLIPFORMAT cf, HGLOBAL *pGlobal);
};

END_NAMESPACE_COMMON

#endif // _SDKDATAOBJECTFACTORY_H_
#endif // __cplusplus
