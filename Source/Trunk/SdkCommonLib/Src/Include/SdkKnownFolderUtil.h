/*!
* @file SdkKnownFolderUtil.h
* 
* @brief This file defines the special folder manager.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/16 12:00
*/

#ifdef __cplusplus
#ifndef _SDKKNOWNFOLDERMANAGER_H_
#define _SDKKNOWNFOLDERMANAGER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*
* @brief The data structure for special folder item.
*/
typedef struct _KNOWNITEM
{
    TCHAR   szFilePath[MAX_PATH];     // The path of item.
    TCHAR   szDisplayName[MAX_PATH];  // The display name of item.
    HICON   hIcon;                    // The icon of item.
    HBITMAP hBitmap;                  // The bitmap of item.

} KNOWNITEM, *LPKNOWNITEM;


typedef vector<LPKNOWNITEM>           KNOWNITEMLIST;    // Defines the list of KNOWNITEM* vector
typedef vector<LPKNOWNITEM>::iterator KNOWNITEMITER;    // Defines the iterator.


class CLASS_DECLSPEC SdkKnownFolderUtil
{
public:

    /*
    * @brief Show browse the special folder dialog box.
    *
    * @param hwnd           [I/ ] A handle of a window that receives the dialog box.
    * @param nCSIDL         [I/ ] The special folder id, such as CSIDL_RECENT.
    * @param pszDisplayName [I/ ] The pointer to the display name.
    *
    * @return The pointer to LPITEMIDLIST which is selected by user.
    */
    static LPITEMIDLIST BrowseSpecialFolder(HWND hWnd, UINT nCSIDL, LPWSTR pszDisplayName);

    /*
    * @brief The default constructor function.
    */
    SdkKnownFolderUtil();

    /*
    * @brief The default destructor function.
    */
    ~SdkKnownFolderUtil();

    /*
    * @brief Initialize the special folder information.
    *
    * @param ncsidlFolder [I/ ] The special folder id, such as CSIDL_RECENT.
    * @param nBmpWidth    [I/ ] The width of bitmap to be extracted.
    * @param nBmpHeight   [I/ ] The height of bitmap to be extracted.
    *
    * @return The return value is TRUE when succeed, otherwise is FALSE.
    */
    BOOL InitFolderInfo(IN UINT ncsidlFolder, UINT nBmpWidth = 256, UINT nBmpHeight = 256);

    /*
    * @brief Retrieve the count of the item list.
    *
    * @return The count of special folder list.
    */
    UINT GetItemCount();

    /*
    * @brief Get the known item at the specified index.
    *
    * @param nIndex     [I/ ] The index of the item want to get.
    * @param pKnownItem [ /O] The pointer to KNOWNITEM structure to fill the structure.
    *
    * @return TRUE if succeed, otherwise is FALSE.
    */
    BOOL GetItem(UINT nIndex, OUT KNOWNITEM* pKnownItem);

    /*
    * @brief Save all icons to specified path
    *
    * @param pszFolderPath [I/ ] he folder path to which these icons are saved.
    *
    * @return The return value is TRUE when succeed, otherwise return value is FALSE.
    */
    BOOL SaveAllIcons(IN LPCTSTR pszFolderPath);

    /*
    * @brief Get the special folder path.
    *
    * @param pszFolderPath  [ /O] The folder path.
    * @param dwSize         [I/ ] The size of the folder path array.
    */
    void GetFolderPath(OUT LPTSTR pszFolderPath, IN DWORD dwSize);

private:

    /*!
    * @brief Clear all the items information.
    */
    void ClearAllItems();

    /*
    * @brief Get the shell item information including displayed name, icon etc.
    *
    * @param pshWorkDir     [I/ ] The pointer to IShellFolder interface.
    * @param pidl           [I/ ] The pidl of corresponding item.
    * @param pknownItem     [ /O] The pointer to pointer to KNOWNITEM structure which save the got information.
    *
    * @return The return value is TRUE when succeed, otherwise is FALSE.
    */
    BOOL GetShellItemInfo(IN IShellFolder* pshWorkDir, IN LPCITEMIDLIST pidl, OUT KNOWNITEM** pknownItem);

    /*
    * @brief Enumerate the shell item in the specified pidl namespace.
    *        This function will fill the m_KnownItemList with shell item information.
    *
    * @param IpshWorkDir    [I/ ] The pointer to IShellFolder interface.
    */
    HRESULT EnumShellItemInfo(IN IShellFolder* pshWorkDir);

private:

    SIZE            m_bmpSize;                  // The size of bitmap to be extracted.
    BOOL            m_isFolder;                 // The flag indicates the item is folder or not
    TCHAR           m_szFolderPath[MAX_PATH];   // The special folder path
    KNOWNITEMLIST*  m_pKnownItemList;           // The item list.
    IShellFolder*   m_pshDesktop;               // Desktop folder.
};

END_NAMESPACE_UTILITIES

#endif // _SDKKNOWNFOLDERMANAGER_H_
#endif // __cplusplus
