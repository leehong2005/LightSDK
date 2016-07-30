#ifdef __cplusplus
#ifndef _SAMPLEASSOCHANDLERITEM_H_
#define _SAMPLEASSOCHANDLERITEM_H_

#include "SdkAssocHandler.h"
#include "SdkImagesManager.h"

class SampleAssocHandlerItem : public CWnd
{
    DECLARE_DYNAMIC(SampleAssocHandlerItem)

public:

    BOOL    m_isIcon;

    SampleAssocHandlerItem(int nIndex = 0);
    virtual ~SampleAssocHandlerItem();

    BOOL Initialize(CWnd *pParent, int x, int y, int width, int height, SdkAssocHandler *pData);

protected:

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:

    int m_nIndex;
    SdkAssocHandler     *m_pAssocHandler;
};

#endif // _SAMPLEASSOCHANDLERITEM_H_
#endif // __cplusplus