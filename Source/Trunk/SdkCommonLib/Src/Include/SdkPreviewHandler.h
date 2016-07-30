/*!
* @file SdkPreviewHandler.h
* 
* @brief SdkPreviewHandler class defines methods to get the preview of a specified file.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2009/12/03 12:00
* @version 1.0.0
*/

#ifdef __cplusplus
#ifndef _SDKPREVIEWHANDLER_H_
#define _SDKPREVIEWHANDLER_H_

#include "SdkCommonHelper.h"

BEGIN_NAMESPACE_UTILITIES

/*!
/ @brief The handler CLSID and name pair.
*/
typedef struct _PREVIEWHANDLERINFO
{
    WCHAR szClsid[100];     // The CLSID of preview handler.
    WCHAR szName[100];      // The name of preview handler.

} PREVIEWHANDLERINFO, *LPPREVIEWHANDLERINFO;

/*!
* @brief SdkPreviewHandler class.
*/
class CLASS_DECLSPEC SdkPreviewHandler
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkPreviewHandler();

    /*!
    * @brief The destructor function.
    */
    ~SdkPreviewHandler();

    /*!
    * @brief Attach preview to a specified window.
    *
    * @param hWnd       [I/ ] The handle to the window to be display preview.
    * @param lpRect     [I/ ] The rectangle to hold the preview.
    *                         If NULL, the entire region of window will be as preview region.
    * @param lpfileName [I/ ] The file to preview.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL AttachPreview(IN HWND hWnd, IN const RECT* lpRect, IN LPCWSTR lpfileName);

    /*!
    * @brief Detach preview action.
    */
    void DetachPreview();

    /*
    * @brief Enumerate the installed preview handler in current machine.
    */
    void EnumInstalledHandler();

    /*!
    * @brief Get the installed handler count, before calling this function,
    *        EnumInstalledHandler must be called first.
    *
    * @return The count of installed handler.
    */
    DWORD GetInstalledHandlerCount();

    /*!
    * @brief Get the handler information at specified index.
    *
    * @param nIndex             [I/ ] The index of handler to be retrieved.
    * @param previewHandleInfo  [I/ ] The pointer to PREVIEWHANDLERINFO structure.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetInstalledHandlerItem(IN DWORD nIndex, OUT LPPREVIEWHANDLERINFO previewHandleInfo);

protected:

    /*!
    * @brief Get CLSID of handler from extension of specified file.
    *
    * @param lpfileExt  [I/ ] The file extension.
    * @param pszClsid   [I/ ] The buffer to receive CLSID.
    * @param dwSize     [I/ ] The size of buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetClsidFromExtension(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize);

    /*!
    * @brief Create preview handler from CLSID.
    *
    * @param ppHandler  [I/ ] The output IPreviewHandler instance.
    * @param pClsid     [I/ ] The CLSID buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL CreatePreviewHandlerFromClsid(OUT IPreviewHandler** ppHandler, IN LPCTSTR pClsid);

    /*!
    * @brief Initialize the specified handler with specified file.
    *
    * @param pHandler   [I/ ] The instance of IPreviewHandler to be initialized.
    * @param lpfileName [I/ ] The path of file.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL InitializeHandler(IN IPreviewHandler* pHandler, IN LPCWSTR lpfileName);

    /*!
    * @brief Initialize the specified handler with specified file.
    *
    * @param pHandler   [I/ ] The instance of IPreviewHandler to be initialized.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL InitializeHandlerWithFile(IN IPreviewHandler* pHandler);

    /*!
    * @brief Initialize the specified handler with file stream.
    *
    * @param pHandler   [I/ ] The instance of IPreviewHandler to be initialized.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL InitializeHandlerWithStream(IN IPreviewHandler* pHandler);

    /*!
    * @brief Initialize the specified handler with shell item.
    *
    * @param pHandler   [I/ ] The instance of IPreviewHandler to be initialized.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL InitializeHandlerWithItem(IN IPreviewHandler* pHandler);

    /*!
    * @brief Get CLSID from map.
    *
    * @param lpfileExt  [I/ ] The file extension.
    * @param pszClsid   [I/ ] The buffer to receive CLSID.
    * @param dwSize     [I/ ] The size of buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetClsidFromMap(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize);

    /*!
    * @brief Get CLSID from default type.
    *
    * @param lpfileExt  [I/ ] The file extension.
    * @param pszClsid   [I/ ] The buffer to receive CLSID.
    * @param dwSize     [I/ ] The size of buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetClsidFromDefaultType(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize);

    /*!
    * @brief Get CLSID from shellex registry.
    *
    * @param lpfileExt  [I/ ] The file extension.
    * @param pszClsid   [I/ ] The buffer to receive CLSID.
    * @param dwSize     [I/ ] The size of buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetClsidFromExtShellEx(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize);

    /*!
    * @brief Get CLSID from file associate.
    *
    * @param lpfileExt  [I/ ] The file extension.
    * @param pszClsid   [I/ ] The buffer to receive CLSID.
    * @param dwSize     [I/ ] The size of buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetClsidFromSystemFileAssoc(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize);

    /*!
    * @brief Get CLSID from file associate extension.
    *
    * @param lpfileExt  [I/ ] The file extension.
    * @param pszClsid   [I/ ] The buffer to receive CLSID.
    * @param dwSize     [I/ ] The size of buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetClsidFromSystemFileAssocExt(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize);

    /*!
    * @brief Get CLSID from open with prog ids.
    *
    * @param lpfileExt  [I/ ] The file extension.
    * @param pszClsid   [I/ ] The buffer to receive CLSID.
    * @param dwSize     [I/ ] The size of buffer.
    *
    * @return TRUE if succeed to preview, otherwise FALSE.
    */
    BOOL GetClsidFromOpenWithProgIds(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize);

    /*!
    * @brief Clear installed handler list.
    */
    void ClearInstalledHandlerList();

protected:

    WCHAR                           m_szFileName[MAX_PATH];         // The preview file name.
    IPreviewHandler                *m_pHandler;                     // The preview handler.
    IInitializeWithFile            *m_pInitWithFile;                // The IInitializeWithFile pointer.
    IInitializeWithItem            *m_pInitWithItem;                // The IInitializeWithItem pointer.
    IInitializeWithStream          *m_pInitWithStream;              // The IInitializeWithStream pointer.
    map<wstring, wstring>           m_mapExtensionToCLSID;          // CLSID and name pair.
    vector<LPPREVIEWHANDLERINFO>    m_vtHandlerList;                // The installed handler list.
};

END_NAMESPACE_UTILITIES

#endif // _SDKPREVIEWHANDLER_H_
#endif // __cplusplus