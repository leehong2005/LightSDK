
#ifdef __cplusplus
#ifndef _SAMPLEASSOCHANDLERDLG_H_
#define _SAMPLEASSOCHANDLERDLG_H_

#include "afxwin.h"
#include "SdkAssocHandler.h"
#include "SampleAssocHandlerItem.h"

class CSampleAssocHandlerDlg : public CDialog
{
public:

    CSampleAssocHandlerDlg(CWnd* pParent = NULL);

    enum { IDD = IDD_SAMPLEASSOCHANDLER_DIALOG };

protected:

    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedButtonApply();

    DECLARE_MESSAGE_MAP()

protected:

    void ClearList();
    void ResetHandlerItems();

private:

    BOOL                            m_isBitmapChecked;
    HICON                           m_hIcon;
    CComboBox                       m_comboBoxExt;
    vector<SampleAssocHandlerItem*> m_vctItemList;
    SdkAssocHandler::AssocHandlerList m_assocHandlerList;
};

#endif // _SAMPLEASSOCHANDLERDLG_H_
#endif // __cplusplus
