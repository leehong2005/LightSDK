#pragma once

#include "SampleDef.h"

class SampleFunParser
{
public:
    SampleFunParser();
    ~SampleFunParser();

    void ParserButtonCtrl(vector<FUNCTIONINFO*>& vctFuns);

private:

    vector<FUNCTIONINFO*>   m_vctBtnFunInfos;
};
