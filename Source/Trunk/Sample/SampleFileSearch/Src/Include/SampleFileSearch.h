
#ifdef __cplusplus
#ifndef _SAMPLEFILESEARCH_H_
#define _SAMPLEFILESEARCH_H_

#include "stdafx.h"
#include "SdkCommonInclude.h"

/*!
* @brief The file property.
*/
typedef struct _FILEPROPERT
{
    wstring fileName;
    wstring fileDate;
    wstring fileType;
    WCHAR   szBuffer[100];
    wstring filePath;

} FILEPROPERTY;

/*!
* @brief The SampleFileSearch class.
*/
class SampleFileSearch : public IFileSearcherNotify
{
public:

    /*!
    * @brief The constructor function.
    */
    SampleFileSearch();

    /*!
    * @brief The destructor function.
    */
    ~SampleFileSearch();

    /*!
    * @brief Create the Dialog.
    */
    HRESULT Initialize(HINSTANCE hInstance);

    /*!
    * @brief Run message.
    */
    void RunMessageLoop();

protected:

    /*!
    * @brief Insert the file property to list view.
    *
    * @param fileProperty   [I/ ] The file property.
    */
    void InserToList(FILEPROPERTY fileProperty);

    /*!
    * @brief Create controls on the dialog.
    *
    * @param hWndDlg         [I/ ] The dialog.
    */
    void OnCreate(HWND hWndDlg);

    /*!
    * @brief The message procedure.
    *
    * @param hWnd       [I/ ] The handle to the window which occurs the message.
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return If you process the message, return 0, otherwise call DefWindowProc.
    */
    INT_PTR OnDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /*
    * @brief The message procedure.
    *
    * @param hWnd       [I/ ] The handle to the window which occurs the message.
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return If you process the message, return 0, otherwise call DefWindowProc.
    */
    static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Called when find a result.
    *
    * @param pfileSearcher  [I/ ] The SdkFileSearcher instance pointer.
    * @param lpFilePath     [I/ ] The full path of the found file.
    *                             lpFilePath maybe points a block of temporary memory, If called want
    *                             to duplicate the content of the memory, should copy the memory, not
    *                             points to lpFilePath directly.
    */
    void OnSearchOneResult(IN SdkFileSearcher *pfileSearcher, IN LPCTSTR lpFilePath);

    /*!
    * @brief Called when find a result.
    *
    * @param pfileSearcher  [I/ ] The SdkFileSearcher instance pointer.
    */
    void OnSearchFinish(IN SdkFileSearcher *pfileSearcher);

    /*!
    * @brief Start search files.
    */
    void SetSearchData();

    void OpenFileFolder(LPCTSTR lpFilePath);

    void OpenFile(LPCTSTR lpFilePath);

private:

    TCHAR            m_szSelFilePath[MAX_PATH];
    HINSTANCE        m_hInst;
    HWND             m_dialog;
    HWND             m_combobox;
    HWND             m_listview;
    FILEPROPERTY     m_filePropert;
    SdkFileSearcher *m_pFileSearch;
    vector<wstring>  m_vctFilePath;
};
#endif // _SAMPLEFILESEARCH_H_
#endif // __cplusplus
