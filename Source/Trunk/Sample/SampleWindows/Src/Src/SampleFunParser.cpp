#include "StdAfx.h"
#include "SampleFunParser.h"
#include "SdkCommonMacro.h"

SampleFunParser::SampleFunParser(void)
{
    m_vctBtnFunInfos.clear();
}

//////////////////////////////////////////////////////////////////////////

SampleFunParser::~SampleFunParser(void)
{
    int nSize = (int)m_vctBtnFunInfos.size();
    for (int i = 0; i < nSize; ++i)
    {
        SAFE_DELETE(m_vctBtnFunInfos[i]);
    }

    m_vctBtnFunInfos.clear();
}

//////////////////////////////////////////////////////////////////////////

void SampleFunParser::ParserButtonCtrl(vector<FUNCTIONINFO*>& vctFuns)
{
    if ( 0 == m_vctBtnFunInfos.size() )
    {
        static FUNCTIONINFO szBtnFunInfos[] = 
        {
            { _SetViewPos,  0, _T(""), TRUE, 2, { {PARAMTYPE_INT, _T("x"), _T("0")}, {PARAMTYPE_INT, _T("y"), _T("0")}, } },
            { _SetViewSize, 0, _T(""), TRUE, 2, { {PARAMTYPE_INT, _T("width"), _T("100")}, {PARAMTYPE_INT, _T("height"), _T("32")}, } },
            { _SetVisible,  0, _T(""), TRUE, 1, { {PARAMTYPE_BOOL, _T("isVisible"), _T("1")}, } },
            { _SetEnable,   0, _T(""), TRUE, 1, { {PARAMTYPE_BOOL, _T("isEnable"),  _T("1")}, } },
            { _SetToggleStyle, 0, _T(""), TRUE, 1, { {PARAMTYPE_BOOL, _T("isToggle"),  _T("1")}, } },
            { _ShowUACIcon,    0, _T(""), TRUE, 1, { {PARAMTYPE_BOOL, _T("fShow"),  _T("1")}, } },
            { _SetText,        0, _T(""), TRUE, 1, { {PARAMTYPE_BOOL, _T("lpText"), _T("")}, } },
            { _SetTextAlign,   0, _T(""), TRUE, 1, { {PARAMTYPE_BOOL, _T("align"),  _T("0")}, } },
            { _SetTextSize,    0, _T(""), TRUE, 1, { {PARAMTYPE_BOOL, _T("size"),   _T("18")}, } },
        };

        int nSize = ARRAYSIZE(szBtnFunInfos);
        for (int i = 0; i < nSize; ++i)
        {
            FUNCTIONINFO *pBtnFunInfo = new FUNCTIONINFO();
            *pBtnFunInfo = szBtnFunInfos[i];
            pBtnFunInfo->strFunName = FunTypeToString(pBtnFunInfo->nFunId);
            m_vctBtnFunInfos.push_back(pBtnFunInfo);
        }
    }

    vctFuns.clear();
    vctFuns.assign(m_vctBtnFunInfos.begin(), m_vctBtnFunInfos.end());
}
