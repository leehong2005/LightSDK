// TestCommon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommonInclude.h"


//////////////////////////////////////////////////////////////////////////

void TestMediaContent()
{
    FilePropertyHelper media(L"D:\\a.mp3");

    PROPERTYKEYNAME szKeys[] = 
    {
        SYSTEM_CORE_AUTHOR,                                 // String Author
        SYSTEM_CORE_COMMENT,                                // String Comment
        SYSTEM_CORE_COMPANY,                                // String Company
        SYSTEM_CORE_COPYRIGHT,                              // String Copyright
        SYSTEM_CORE_DATECREATED,                            // DateTime
        SYSTEM_CORE_DATEMODIFIED,                           // DateTime
        SYSTEM_CORE_DATEACCESSED,                           // DateTime
        SYSTEM_CORE_ITEMTYPE,                               // String
        SYSTEM_CORE_ITEMTYPETEXT,                           // String
        SYSTEM_CORE_RATING,                                 // UInt32
        SYSTEM_CORE_RATINGTEXT,                             // String
        SYSTEM_CORE_TITLE,                                  // String
        SYSTEM_CORE_TOTALFILESIZE,                          // UInt64
        SYSTEM_CORE_FILEALLOCATIONSIZE,                     // UInt64
        SYSTEM_CORE_SIZE,                                   // UInt64
        SYSTEM_CORE_DISPLAYNAME,                            // String
        SYSTEM_CORE_FILENAME,                               // String
        SYSTEM_CORE_ITEMPATHDISPLAY,                        // String
        SYSTEM_CORE_PARSINGPATH,                            // String
        SYSTEM_CORE_DESCRIPTION,                            // String
        SYSTEM_CORE_KEYWORDS,                               // String Tags
        SYSTEM_CORE_FOLDERPATH,                             // String Folder Path.
    };

    WCHAR szBuffer[MAX_PATH] = { 0 };
    for (int i = 0; i < ARRAYSIZE(szKeys); ++i)
    {
        media.FormatForDisplay(szKeys[i], szBuffer, MAX_PATH);
        ::OutputDebugString(szBuffer);
    }

    ::OutputDebugString(L"================================================ \n");

    media.SetFileName(L"D:\\blue_turkey.mp3");

    for (int i = 0; i < ARRAYSIZE(szKeys); ++i)
    {
        media.FormatForDisplay(szKeys[i], szBuffer, MAX_PATH);
        ::OutputDebugString(szBuffer);
    }
}

//////////////////////////////////////////////////////////////////////////

void TestMediaInfoProvider()
{
}

//////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    CoInitialize(NULL);

    TestMediaContent();
    TestMediaInfoProvider();

    CoUninitialize();

	return 0;
}

