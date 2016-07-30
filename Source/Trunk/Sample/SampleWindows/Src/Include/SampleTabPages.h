#pragma once

#include "SdkUICommonInclude.h"
#include "SampleDef.h"
#include "SampleFunParser.h"

class SampleTabPages
{
public:

    SampleTabPages(CONTROLTYPE eCtrlType, SdkTabView* pTabView);
    ~SampleTabPages();

    CONTROLTYPE GetControlType() const;
    SdkViewElement* GetView();
    LPCTSTR GetCtrlName();
    void GetFunAndParams(vector<FUNCTIONINFO*>& vctFuns);
    BOOL PerformFuns(FUNCTIONINFO *pFunInfo);

private:

    SdkViewElement* CreateView(CONTROLTYPE eCtrlType);
    SdkViewElement* CreateButton();
    SdkViewElement* CreateCheckBox();
    SdkViewElement* CreateComboBox();
    SdkViewElement* CreateEditBox();
    SdkViewElement* CreateListBox();
    SdkViewElement* CreateGifView();
    SdkViewElement* CreateGroupBox();
    SdkViewElement* CreateImageView();
    SdkViewElement* CreateOverlapView();
    SdkViewElement* CreateProgress();
    SdkViewElement* CreateRadioButton();
    SdkViewElement* CreateRatingView();
    SdkViewElement* CreateScrollBar();
    SdkViewElement* CreateSeekBar();
    SdkViewElement* CreateTabHeader();
    SdkViewElement* CreateTabView();
    SdkViewElement* CreateTextView();

private:

    FLOAT               m_nTabWidth;
    FLOAT               m_nTabHeight;
    SdkViewElement     *m_pView;
    CONTROLTYPE         m_eCtrlType;
    SampleFunParser    *m_pFunParser;
};
