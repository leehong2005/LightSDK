/*!
* @file SdkDataObjectHelper.cpp
* 
* @brief The file define the class to create SdkDataObject object.
*
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#include "stdafx.h"
#include "SdkDataObjectHelper.h"
#include "SdkDataObject.h"

USING_NAMESPACE_COMMON

HRESULT WINAPI SdkDataObjectHelper::CreateDataObject(REFIID riid, void **ppv)
{
    SdkDataObject *pDataObject = new (std::nothrow) SdkDataObject();

    HRESULT hr = (NULL != pDataObject) ? S_OK : E_OUTOFMEMORY;
    if ( SUCCEEDED(hr) )
    {
        hr = pDataObject->QueryInterface(riid, ppv);
    }

    SAFE_RELEASE(pDataObject);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI SdkDataObjectHelper::CopyDataObject(IDataObject *pDataObjectSrc, IDataObject *pDataObjectDest)
{
    HRESULT hr = ((NULL != pDataObjectDest) && (NULL != pDataObjectSrc)) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        IEnumFORMATETC *pEnumFormatetc = NULL;
        hr = pDataObjectSrc->EnumFormatEtc(DATADIR_GET, &pEnumFormatetc);

        if ( SUCCEEDED(hr) )
        {
            ULONG ufetched = 0;
            FORMATETC formatEtc = { 0 };
            STGMEDIUM stgMedium = { 0 };

            while (SUCCEEDED(pEnumFormatetc->Next(1, &formatEtc, &ufetched)) && (1 == ufetched))
            {
                stgMedium.tymed = formatEtc.tymed;
                hr = pDataObjectSrc->GetData(&formatEtc, &stgMedium);
                if (SUCCEEDED(hr))
                {
                    hr = pDataObjectDest->SetData(&formatEtc, &stgMedium, TRUE);
                }
            }
        }

        SAFE_RELEASE(pEnumFormatetc);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI SdkDataObjectHelper::SetDropTip(IDataObject *pDataObject, const DROPDESCRIPTION *pDropDesc)
{
    HRESULT hr = ( (NULL != pDataObject) && (NULL != pDropDesc) ) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        static CLIPFORMAT s_cfDropDescription = 0;
        if ( 0 == s_cfDropDescription )
        {
            s_cfDropDescription = (CLIPFORMAT)RegisterClipboardFormat(CFSTR_DROPDESCRIPTION);
        }

        hr = SetDataBlob(pDataObject, s_cfDropDescription, (void*)pDropDesc, sizeof(DROPDESCRIPTION));
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI SdkDataObjectHelper::GetDropTip(IDataObject *pDataObject, DROPDESCRIPTION *pDropDesc)
{
    HRESULT hr = ( (NULL != pDataObject) && (NULL != pDropDesc) ) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        static CLIPFORMAT s_cfDropDescription = 0;
        if ( 0 == s_cfDropDescription )
        {
            s_cfDropDescription = (CLIPFORMAT)RegisterClipboardFormat(CFSTR_DROPDESCRIPTION);
        }

        HGLOBAL hGlobal = NULL;
        hr = GetDataBlob(pDataObject, s_cfDropDescription, &hGlobal);
        if ( SUCCEEDED(hr) )
        {
            void *pBlob = GlobalLock(hGlobal);
            memcpy(pDropDesc, pBlob, sizeof(DROPDESCRIPTION));
            GlobalFree(hGlobal);
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI SdkDataObjectHelper::SetDataBlob(IDataObject *pDataObject, CLIPFORMAT cf, const void *pvBlob, UINT cbBlob)
{
    HRESULT hr = ( (NULL != pDataObject) && (NULL != pvBlob) ) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        HGLOBAL hGlobal = GlobalAlloc(GPTR, cbBlob);
        hr = (NULL != hGlobal) ? S_OK : E_OUTOFMEMORY;

        if ( SUCCEEDED(hr) )
        {
            void *pv = GlobalLock(hGlobal);
            CopyMemory(pv, pvBlob, cbBlob);
            GlobalUnlock(hGlobal);
            FORMATETC fmte = {cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};

            // The STGMEDIUM structure is used to define how to handle a global memory transfer.
            // This structure includes a flag, tymed, which indicates the medium
            // to be used, and a union comprising pointers and a handle for getting whichever
            // medium is specified in tymed.

            STGMEDIUM medium = { 0 };
            medium.tymed = TYMED_HGLOBAL;
            medium.hGlobal = hGlobal;

            hr = pDataObject->SetData(&fmte, &medium, TRUE);
            if ( FAILED(hr) )
            {
                GlobalFree(hGlobal);
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI SdkDataObjectHelper::GetDataBlob(IDataObject *pDataObject, CLIPFORMAT cf, HGLOBAL *pGlobal)
{
    HRESULT hr = ( (NULL != pDataObject) && (NULL != pGlobal) ) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        FORMATETC fmte = { cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM medium = { 0 };
        medium.tymed = TYMED_HGLOBAL;

        hr = pDataObject->GetData(&fmte, &medium);
        if ( SUCCEEDED(hr) )
        {
            (*pGlobal) = medium.hGlobal;
        }
    }

    return hr;
}
