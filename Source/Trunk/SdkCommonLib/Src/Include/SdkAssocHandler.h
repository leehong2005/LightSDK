/*!
* @file SdkAssocHandler.h
* 
* @brief The SdkAssocHandler class is a wrapper of IAssocHandler interface.
*        User can use API SHAssocEnumHandlers to get instance of IAssocHandler.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/26
*/

#ifdef __cplusplus
#ifndef _SDKASSOCHANDLER_H_
#define _SDKASSOCHANDLER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

class CLASS_DECLSPEC SdkAssocHandler
{
public:

    typedef vector<SdkAssocHandler*>    AssocHandlerList;

    /*!
    * @brief The SdkAssocHandler list of specified file by lpfileExt parameter. 
    *        you should call CoInitialize() function before calling this function.
    *
    * @param assocHandlers  [I/O] The reference to vector<SdkAssocHandler*> to store SdkAssocHandler pointers.
    * @param lpfileExt      [I/ ] The pointer to null-terminated, Unicode string to contains file extension.
    * @param isRecommended  [I/ ] The flag indicates get recommended associated handler or not, default value is TRUE.
    *
    * @return TRUE if successful, otherwise return FALSE.
    */
    static BOOL GetAssocHandlers(IN OUT AssocHandlerList& assocHandlers, IN LPCWSTR lpfileExt, BOOL isRecommended = TRUE);

    /*!
    * @brief The default constructor function.
    */
    SdkAssocHandler();

    /*!
    * @brief The constructor function. you can call Release function to release passocHandler.
    *
    * @param passocHandler  [I/ ] The pointer to IAssocHandler interface.
    */
    SdkAssocHandler(IAssocHandler *passocHandler);

    /*!
    * @brief The default constructor function.
    */
    ~SdkAssocHandler();

    /*!
    * @brief Attach pointer to IAssocHandler interface to SdkAssocHandler class.
    *        After attach to this class, you can call Release function to release passocHandler.
    *
    * @param passocHandler  [I/ ] The pointer to IAssocHandler interface.
    *
    * @return TRUE if successful, otherwise return FALSE.
    */
    BOOL Attach(IAssocHandler *passocHandler);

    /*!
    * @brief Get the display name of current associated handler.
    *
    * @return The display name.
    */
    wstring GetUIName();

    /*!
    * @brief Get the full path of current associated handler.
    *
    * @return The file full path.
    */
    wstring GetName();

    /*!
    * @brief The GetIcon function retrieves a handle to an icon from current associated handler.
    *
    * @return The handle to HICON. 
    *
    * @remark The size of HICON usually is 32*32. You can use DrawIconEx to draw icon on DC 
    *         of your application.
    */
    HICON GetIcon();

    /*!
    * @brief The GetBitmap function retrieves a handle to HBITMAP.
    *
    * @param nWidth     [I/ ] The width of icon, default value is 256.
    * @param nHeight    [I/ ] The width of icon, default value is 256.
    *
    * @return The handle to HBITMAP. 
    *
    * @remark This function will extract icon from specified file, It only get icon of file.
    */
    HBITMAP GetBitmap(int nWidth = 256, int nHeight = 256);

    /*!
    * @brief Indicates whether the handler is registered as a recommended handler for the queried file type.
    *
    * @return TRUE if the handler is recommended, otherwise return FALSE.
    */
    BOOL IsRecommended();

    /*!
    * @brief Set current handler as the default application for this file type.
    *
    * @ pszDescription  [I/ ] A pointer to a null-terminated, Unicode string that
    *                         contains the display name of the application.
    *
    * @return TRUE if successful, otherwise return FALSE.
    */
    BOOL MakeDefault(IN LPCWSTR pszDescription);

    /*!
    * @brief Directly open specified file by pszfilePath parameter with current handler.
    *
    * @ pszfilePath     [I/ ] A pointer to a null-terminated, Unicode string that
    *                         contains the full path of file to be opened.
    *
    * @return TRUE if successful, otherwise return FALSE.
    */
    BOOL Invoke(IN LPCWSTR pszfilePath);

    /*!
    * @brief Save icon to specified file.
    *
    * @ pszfilePath     [I/ ] A pointer to a null-terminated, Unicode string that
    *                         contains the full path to be saved.
    *
    * @return TRUE if successful, otherwise return FALSE.
    */
    BOOL SaveToFile(IN LPCWSTR pszfilePath);

private:

    int              m_nBmpWidth;               // The width of the bitmap
    int              m_nBmpHeight;              // The height of the bitmap
    HICON            m_hAssocIcon;              // The handle to HICON to store icon
    HBITMAP          m_hAssocBitmap;            // The handle to HBITMAP to store icon
    IAssocHandler   *m_pAssocHandler;           // The handle to IAssocHandler interface
};

END_NAMESPACE_UTILITIES

#endif // _SDKASSOCHANDLER_H_
#endif // __cplusplus
