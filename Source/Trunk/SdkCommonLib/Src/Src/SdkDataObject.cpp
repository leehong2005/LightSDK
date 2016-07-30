/*!
* @file SdkDataObject.cpp
* 
* @brief The file define the class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#include "stdafx.h"
#include "SdkDataObject.h"
#include "SdkCommonHelper.h"

USING_NAMESPACE_COMMON

/*!
* @brief _DATASTORAGE struct
*/
struct NAMESPACE_COMMONLIB::_DATASTORAGE
{
    FORMATETC *formatEtc;
    STGMEDIUM *stgMedium;
};

//////////////////////////////////////////////////////////////////////////

SdkDataObject::SdkDataObject() : m_lRefCount(1)
{
    // The reference count must be initialized 1.
}

//////////////////////////////////////////////////////////////////////////

SdkDataObject::~SdkDataObject()
{
    int nSize = (int)m_vctDataStorage.size();
    for (int i = 0; i < nSize; ++i)
    {
        DATASTORAGE &dataEntry = m_vctDataStorage.at(i);
        ReleaseStgMedium(dataEntry.stgMedium);

        SAFE_DELETE(dataEntry.stgMedium);
        SAFE_DELETE(dataEntry.formatEtc);
    }

    m_vctDataStorage.clear();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkDataObject::IsDataAvailable(CLIPFORMAT cfFormat)
{
    FORMATETC fmtEtc = { cfFormat, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

    return (S_OK == QueryGetData(&fmtEtc)) ? TRUE : FALSE ;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(SdkDataObject, IDataObject),
        { 0 },
    };

    return QISearch(this, qit, riid, ppv);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkDataObject::AddRef()
{
    return InterlockedIncrement(&m_lRefCount);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkDataObject::Release()
{
    ULONG lRef = InterlockedDecrement(&m_lRefCount);
    if (0 == lRef)
    {
        delete this;
    }

    return m_lRefCount;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
{
    if ( (NULL == pformatetcIn) || (NULL == pmedium) )
    {
        return E_INVALIDARG;
    }

    int nIndex = FindFormatEtc(pformatetcIn);
    if (nIndex >= 0)
    {
        return CopyMedium(
            pmedium,
            m_vctDataStorage[nIndex].stgMedium,
            m_vctDataStorage[nIndex].formatEtc);
    }

    return DV_E_FORMATETC;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::SetData(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease)
{
    if ( pformatetc->tymed != pmedium->tymed )
    {
        return E_INVALIDARG;
    }

    HRESULT hr = ((NULL != pformatetc) && (NULL != pmedium)) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        // The format already exist, we should replace the data of this media.
        int nIndex = FindFormatEtc(pformatetc);
        if (nIndex >= 0)
        {
            DATASTORAGE &dataEntry = m_vctDataStorage[nIndex];
            *(dataEntry.formatEtc) = *pformatetc;

            if (fRelease)
            {
                *(dataEntry.stgMedium) = *pmedium;
            }
            else
            {
                ReleaseStgMedium(dataEntry.stgMedium);
                hr = CopyMedium(dataEntry.stgMedium, pmedium, pformatetc);
            }
        }
        else
        {
            FORMATETC* pfetc = new FORMATETC;
            STGMEDIUM* pStgMed = new STGMEDIUM;
            *pfetc = *pformatetc;

            if (fRelease)
            {
                *pStgMed = *pmedium;
            }
            else
            {
                hr = CopyMedium(pStgMed, pmedium, pformatetc);
            }

            // Add the format and storage to vector.
            DATASTORAGE dataEntry = { pfetc, pStgMed };
            m_vctDataStorage.push_back(dataEntry);
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::GetDataHere(FORMATETC *pformatetc , STGMEDIUM *pmedium)
{
    UNREFERENCED_PARAMETER(pformatetc);
    UNREFERENCED_PARAMETER(pmedium);

    // Not implement

    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::QueryGetData(FORMATETC *pformatetc)
{
    if ( NULL == pformatetc )
    {
        return E_INVALIDARG;
    }

    return (-1 == FindFormatEtc(pformatetc)) ? DV_E_FORMATETC : S_OK;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::GetCanonicalFormatEtc(FORMATETC *pformatetcIn, FORMATETC *pformatetcOut)
{
    UNREFERENCED_PARAMETER(pformatetcIn);

    return (NULL == pformatetcOut) ? E_INVALIDARG : DATA_S_SAMEFORMATETC;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc)
{
    if ( NULL == ppenumFormatEtc )
    {
        return E_INVALIDARG;
    }

    if ( DATADIR_GET == dwDirection )
    {
        FORMATETC rgfmtetc[] =
        {
            { CF_UNICODETEXT, NULL, DVASPECT_CONTENT, 0, TYMED_HGLOBAL },
        };

        *ppenumFormatEtc = NULL;
        return SHCreateStdEnumFmtEtc(ARRAYSIZE(rgfmtetc), rgfmtetc, ppenumFormatEtc);
    }

    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::DAdvise(FORMATETC *pformatetc , DWORD advf , IAdviseSink *pAdvSnk , DWORD *pdwConnection)
{
    UNREFERENCED_PARAMETER(pformatetc);
    UNREFERENCED_PARAMETER(advf);
    UNREFERENCED_PARAMETER(pAdvSnk);
    UNREFERENCED_PARAMETER(pdwConnection);

    // Not implement.

    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::DUnadvise(DWORD dwConnection)
{
    UNREFERENCED_PARAMETER(dwConnection);

    // Not implement.

    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDataObject::EnumDAdvise(IEnumSTATDATA **ppenumAdvise)
{
    UNREFERENCED_PARAMETER(ppenumAdvise);

    // Not implement.

    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkDataObject::CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc)
{
    if ( (NULL == pMedDest) || (NULL == pMedSrc) || (NULL == pFmtSrc) )
    {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;
    *pMedDest = *pMedSrc;
    SAFE_ADDREF(pMedDest->pUnkForRelease);

    switch(pMedSrc->tymed)
    {
    case TYMED_HGLOBAL:
        pMedDest->hGlobal = (HGLOBAL)OleDuplicateData(
            pMedSrc->hGlobal, pFmtSrc->cfFormat, NULL);
        break;

    case TYMED_GDI:
        pMedDest->hBitmap = (HBITMAP)OleDuplicateData(
            pMedSrc->hBitmap, pFmtSrc->cfFormat, NULL);
        break;

    case TYMED_MFPICT:
        pMedDest->hMetaFilePict = (HMETAFILEPICT)OleDuplicateData(
            pMedSrc->hMetaFilePict, pFmtSrc->cfFormat, NULL);
        break;

    case TYMED_ENHMF:
        pMedDest->hEnhMetaFile = (HENHMETAFILE)OleDuplicateData(
            pMedSrc->hEnhMetaFile, pFmtSrc->cfFormat, NULL);
        break;

    case TYMED_FILE:
        pMedSrc->lpszFileName = (LPOLESTR)OleDuplicateData(
            pMedSrc->lpszFileName, pFmtSrc->cfFormat, NULL);
        break;

    case TYMED_ISTREAM:
        pMedDest->pstm = pMedSrc->pstm;
        SAFE_ADDREF(pMedDest->pstm);
        break;

    case TYMED_ISTORAGE:
        pMedDest->pstg = pMedSrc->pstg;
        SAFE_ADDREF(pMedDest->pstg);
        break;

    case TYMED_NULL:
        hr = S_FALSE;
        break;
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

int SdkDataObject::FindFormatEtc(FORMATETC *pformatEtc)
{
    if (NULL != pformatEtc)
    {
        int nSize = (int)m_vctDataStorage.size();
        for (int i = 0; i < nSize; ++i)
        {
            FORMATETC *formatEtc = m_vctDataStorage[i].formatEtc;

            if ( (formatEtc->cfFormat == pformatEtc->cfFormat) &&
                 (formatEtc->tymed == pformatEtc->tymed) &&
                 (formatEtc->dwAspect == pformatEtc->dwAspect) )
            {
                return i;
            }
        }
    }

    return -1;
}
