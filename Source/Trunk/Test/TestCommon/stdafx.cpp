// stdafx.cpp : source file that includes just the standard includes
// TestCommon.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void Test()
{
    int i = 0;

    int b = (0 == i) ? 10 : 100;
}

void Test2()
{
    int b = 0;
    if (1)
    {
        b = 10;
    }
    else
    {
        b = 100;
    }
}