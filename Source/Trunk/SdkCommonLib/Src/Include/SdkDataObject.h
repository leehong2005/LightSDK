/*!
* @file SdkDataObject.h
* 
* @brief The file define the class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#ifdef __cplusplus
#ifndef _SDKDATAOBJECT_H_
#define _SDKDATAOBJECT_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief CSnrKrDataObject
*/
class CLASS_DECLSPEC SdkDataObject : public IDataObject
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkDataObject();

    /*!
    * @brief Determine whether the specified clip format is available or not.
    *
    * @param cfFormat       [I/ ] The data format.
    *
    * @return TRUE if data is available, FALSE is NOT available.
    */
    BOOL IsDataAvailable(CLIPFORMAT cfFormat);

    /*!
    * @brief Returns a pointer to a specified interface on an object to which a client currently holds an interface pointer.
    *
    * @param riid           [I/ ] Identifier of the interface being requested.
    * @param ppv            [ /O] Address of pointer variable that receives the interface pointer requested in riid.
    *
    * @return S_OK if the interface is supported, E_NOINTERFACE if not.
    */
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);

    /*!
    * @brief The IUnknown::AddRef method increments the reference count for an interface on an object.
    *
    * @return Returns an integer from 1 to n, the value of the new reference count.
    */
    IFACEMETHODIMP_(ULONG) AddRef();

    /*!
    * @brief Decrements the reference count for the calling interface on a object.
    *
    * @return Returns the resulting value of the reference count, which is used for diagnostic/testing purposes only.
    */
    IFACEMETHODIMP_(ULONG) Release();

    /*!
    * @brief Called by a data consumer to obtain data from a source data object.
    *
    * @param pformatetcIn   [I/ ] Pointer to the FORMATETC structure that defines the format, medium, and target device to use when passing the data.
    * @param pmedium        [I/ ] Pointer to the STGMEDIUM structure.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);

    /*!
    * @brief Called by an object containing a data source to transfer data to the object that implements this method.
    *
    * @param pformatetc     [I/ ] Pointer to the FORMATETC structure that defines the format, medium, and target device to use when passing the data.
    * @param pmedium        [I/ ] Pointer to the STGMEDIUM structure.
    * @param fRelease       [I/ ] Whether release the global memory.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP SetData(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease);

    /*!
    * @brief Called by a data consumer to obtain data from a source data object.
    *
    * @param pformatetc     [I/ ] Pointer to the FORMATETC structure.
    * @param pmedium        [I/ ] Pointer to the STGMEDIUM structure.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP GetDataHere(FORMATETC *pformatetc , STGMEDIUM *pmedium );

    /*!
    * @brief Determines whether the data object is capable of rendering the data described in the FORMATETC structure.
    *
    * @param pformatetc     [I/ ] Pointer to the FORMATETC structure.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP QueryGetData(FORMATETC *pformatetc);

    /*!
    * @brief Provides a standard FORMATETC structure that is logically equivalent to one that is more complex.
    *
    * @param pformatetcIn   [I/ ] Pointer to the FORMATETC structure.
    * @param pformatetcOut  [I/ ] Pointer to the FORMATETC structure.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP GetCanonicalFormatEtc(FORMATETC *pformatetcIn, FORMATETC *pformatetcOut);

    /*!
    * @brief Creates an object for enumerating the FORMATETC structures for a data object.
    *
    * @param dwDirection     [I/ ] Specifies a value from the enumeration.
    * @param ppenumFormatEtc [ /O] Address of IEnumFORMATETC* pointer.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc);

    /*!
    * @brief Called by an object supporting an advise sink to create a connection between a data object and the advise sink.
    *
    * @param pformatetc     [I/ ] Pointer to data of interest to the advise sink.
    * @param advf           [I/ ] Flags that specify how the notification takes place.
    * @param pAdvSnk        [I/ ] Pointer to the advise sink.
    * @param pdwConnection  [ /O] Pointer to a token that identifies this connection.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP DAdvise(FORMATETC *pformatetc , DWORD advf , IAdviseSink *pAdvSnk , DWORD *pdwConnection);

    /*!
    * @brief Destroys a notification connection that had been previously set up.
    *
    * @param dwConnection   [I/ ] Connection to remove.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP DUnadvise(DWORD dwConnection);

    /*!
    * @brief Creates an object that can be used to enumerate the current advisory connections.
    *
    * @param ppenumAdvise   [ /O] Address of output variable that receives the IEnumSTATDATA interface pointer.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    IFACEMETHODIMP EnumDAdvise(IEnumSTATDATA **ppenumAdvise);

private:

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkDataObject();

    /*!
    * @brief The copy constructor function.
    */
    SdkDataObject(const SdkDataObject&);

    /*!
    * @brief The equal operator function.
    */
    SdkDataObject& operator = (const SdkDataObject&);

    /*!
    * @brief Copy data about STGMEDIUM structure.
    *
    * @param pMedDest   [I/ ] Pointer to STGMEDIUM structure.
    * @param pMedSrc    [I/ ] Pointer to STGMEDIUM structure.
    * @param pFmtSrc    [I/ ] Pointer to FORMATETC structure.
    *
    * @return S_OK if succeeds, other COM error if fails.
    */
    HRESULT CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc);

    /*!
    * @brief Find the index of format that matches with specified format.
    *
    * @param pformatEtc [I/ ] The specified format
    *
    * @return The index of the format if matches, otherwise return -1.
    */
    int FindFormatEtc(FORMATETC *pformatEtc);

private:

    typedef struct _DATASTORAGE DATASTORAGE;

    volatile LONG           m_lRefCount;        // The reference of count
    vector<DATASTORAGE>     m_vctDataStorage;   // The collection of DATASTORAGE_t structure
};

END_NAMESPACE_COMMON

#endif // _SDKDATAOBJECT_H_
#endif // __cplusplus
