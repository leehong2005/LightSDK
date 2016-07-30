/*!
* @file CommonMacro.h 
* 
* @brief This file includes some common macro.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/04/20
*/

#ifdef __cplusplus
#ifndef _SDKCOMMONMACRO_H_
#define _SDKCOMMONMACRO_H_



//
// Namespace names
//

#define NAME_COMPANY                LZT
#define NAME_COMMONLIB              CommonLib
#define NAME_UTILITIES              Utilities
#define NAME_UILIB                  UILib
#define NAME_VIEWS                  Views
#define NAME_WINDOW                 Window
#define NAME_ANIMATION              Animations
#define NAME_D2D                    D2D
#define NAME_D3D                    D3D
#define NAME_THEME                  UITheme
#define NAME_CALLBACK               Callback


//
// Namespace full names
//

#define NAMESPACE_COMMONLIB         NAME_COMPANY::NAME_COMMONLIB
#define NAMESPACE_UTILITIES         NAME_COMPANY::NAME_UTILITIES
#define NAMESPACE_UILIB             NAME_COMPANY::NAME_UILIB
#define NAMESPACE_VIEWS             NAME_COMPANY::NAME_VIEWS
#define NAMESPACE_WINDOW            NAME_COMPANY::NAME_WINDOW
#define NAMESPACE_ANIMATION         NAME_COMPANY::NAME_ANIMATION
#define NAMESPACE_D2D               NAME_COMPANY::NAME_D2D
#define NAMESPACE_D3D               NAME_COMPANY::NAME_D3D
#define NAMESPACE_THEME             NAME_COMPANY::NAME_THEME
#define NAMESPACE_CALLBACK          NAME_COMPANY::NAME_CALLBACK



#define BEGIN_NAMESPACE(name)       namespace NAME_COMPANY { namespace name {
#define END_NAMESPACE               }}
#define USING_NAMESPACE(name)       using namespace name;


//
// Begin of namespaces
//

#define BEGIN_NAMESPACE_COMMON      BEGIN_NAMESPACE(NAME_COMMONLIB)
#define BEGIN_NAMESPACE_UTILITIES   BEGIN_NAMESPACE(NAME_UTILITIES)
#define BEGIN_NAMESPACE_UILIB       BEGIN_NAMESPACE(NAME_UILIB)
#define BEGIN_NAMESPACE_VIEWS       BEGIN_NAMESPACE(NAME_VIEWS)
#define BEGIN_NAMESPACE_WINDOW      BEGIN_NAMESPACE(NAME_WINDOW)
#define BEGIN_NAMESPACE_ANIMATION   BEGIN_NAMESPACE(NAME_ANIMATION)
#define BEGIN_NAMESPACE_D2D         BEGIN_NAMESPACE(NAME_D2D)
#define BEGIN_NAMESPACE_D3D         BEGIN_NAMESPACE(NAME_D3D)
#define BEGIN_NAMESPACE_THEME       BEGIN_NAMESPACE(NAME_THEME)
#define BEGIN_NAMESPACE_CALLBACK    BEGIN_NAMESPACE(NAME_CALLBACK)


//
// End of namespaces
//

#define END_NAMESPACE_COMMON        END_NAMESPACE
#define END_NAMESPACE_UTILITIES     END_NAMESPACE
#define END_NAMESPACE_UILIB         END_NAMESPACE
#define END_NAMESPACE_VIEWS         END_NAMESPACE
#define END_NAMESPACE_WINDOW        END_NAMESPACE
#define END_NAMESPACE_ANIMATION     END_NAMESPACE
#define END_NAMESPACE_D2D           END_NAMESPACE
#define END_NAMESPACE_D3D           END_NAMESPACE
#define END_NAMESPACE_THEME         END_NAMESPACE
#define END_NAMESPACE_CALLBACK      END_NAMESPACE


//
// Using of namespaces
//

#define USING_NAMESPACE_COMMON      USING_NAMESPACE(NAMESPACE_COMMONLIB)
#define USING_NAMESPACE_UTILITIES   USING_NAMESPACE(NAMESPACE_UTILITIES)
#define USING_NAMESPACE_UILIB       USING_NAMESPACE(NAMESPACE_UILIB)
#define USING_NAMESPACE_VIEWS       USING_NAMESPACE(NAMESPACE_VIEWS)
#define USING_NAMESPACE_WINDOW      USING_NAMESPACE(NAMESPACE_WINDOW)
#define USING_NAMESPACE_ANIMATION   USING_NAMESPACE(NAMESPACE_ANIMATION)
#define USING_NAMESPACE_D2D         USING_NAMESPACE(NAMESPACE_D2D)
#define USING_NAMESPACE_D3D         USING_NAMESPACE(NAMESPACE_D3D)
#define USING_NAMESPACE_THEME       USING_NAMESPACE(NAMESPACE_THEME)
#define USING_NAMESPACE_CALLBACK    USING_NAMESPACE(NAMESPACE_CALLBACK)



// This macro is used to declare a class with namespace.
#ifndef DECLARE_CLASS
#define DECLARE_CLASS(name, nspace) \
        namespace NAME_COMPANY      \
        {                           \
            namespace nspace        \
            {                       \
                class name;         \
            }                       \
        }
#endif // DECLARE_CLASS





// This macro is used for COM interface to add the reference count.
// typically used when user want to equal two COM interfaces.
#ifndef SAFE_ADDREF
#define SAFE_ADDREF(p)              \
        {                           \
            if ((p) != NULL)        \
            {                       \
                (p)->AddRef();      \
            }                       \
        }
#endif // SAFE_ADDREF


// Release a COM interface, it is no necessary to check NULL of the interface.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)             \
        {                           \
            if ((p) != NULL)        \
            {                       \
                (p)->Release();     \
                (p) = NULL;         \
            }                       \
        }
#endif // SAFE_RELEASE



// Delete the memory allocated with C++ operator new.
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)              \
        {                           \
            if ((p) != NULL)        \
            {                       \
                delete (p);         \
                (p) = NULL;         \
            }                       \
        }
#endif // SAFE_DELETE


// Delete the memory array allocated with C++ operator new[].
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)        \
        {                           \
            if ((p) != NULL)        \
            {                       \
                delete [] (p);      \
                (p) = NULL;         \
            }                       \
        }
#endif // SAFE_DELETE_ARRAY



// Used to check a handle is valid or invalid, usually the handle is file handle.
#ifndef ISVALIDHANDLE
#define ISVALIDHANDLE(h)        ( (h) != NULL && (h) != INVALID_HANDLE_VALUE )
#endif // ISVALIDHANDLE


// Safe close the handle to a file handle. This macro call ::CloseHandle function.
#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE(h)                    \
        {                                       \
            if (ISVALIDHANDLE((h)))             \
            {                                   \
                ::CloseHandle((h));             \
                (h) = NULL;                     \
            }                                   \
        }
#endif // SAFE_CLOSE_HANDLE



// This macro call ::DeleteObject function to delete GDI object, such as HBITMAP.
#ifndef SAFE_DELETE_OBJECT
#define SAFE_DELETE_OBJECT(h)                   \
        {                                       \
            if (NULL != (h))                    \
            {                                   \
                ::DeleteObject((h));            \
                (h) = NULL;                     \
            }                                   \
        }
#endif // SAFE_DELETE_OBJECT


// The macro call ::DestroyIcon function to destroy an icon from LoadIcon, CreateIcon, ect.
#ifndef SAFE_DELETE_ICON
#define SAFE_DELETE_ICON(h)                     \
        {                                       \
            if (NULL != (h))                    \
            {                                   \
                ::DestroyIcon((h));             \
                (h) = NULL;                     \
            }                                   \
        }
#endif // SAFE_DELETE_ICON



// This macro call ::DeleteDC function to delete the device context from CreateDC, ect.
#ifndef SAFE_DELETE_DC
#define SAFE_DELETE_DC(h)                       \
        {                                       \
            if (NULL != (h))                    \
            {                                   \
                ::DeleteDC((h));                \
                (h) = NULL;                     \
            }                                   \
        }
#endif // SAFE_DELETE_DC


#ifndef FILTER_BITVALUE
#define FILTER_BITVALUE(val, bit, vct)          \
        {                                       \
            if (val & bit)                      \
            {                                   \
                vct.push_back(bit);             \
            }                                   \
        }
#endif // FILTER_BITVALUE



// Convert GDI RECT to D2DRECt.
#ifndef RECT_TO_D2DRECT
#define RECT_TO_D2DRECT(rc)                     \
        {                                       \
            (FLOAT)((rc).left),                 \
            (FLOAT)((rc).top),                  \
            (FLOAT)((rc).right),                \
            (FLOAT)((rc).bottom)                \
        }
#endif // RECT_TO_D2DRECT


// Convert D2DRECT to GDI RECT.
#ifndef D2DRECT_TO_RECT
#define D2DRECT_TO_RECT(rc)                     \
        {                                       \
            (LONG)((rc).left),                  \
            (LONG)((rc).top),                   \
            (LONG)((rc).right),                 \
            (LONG)((rc).bottom)                 \
        }
#endif // D2DRECT_TO_RECT


// Get the HRESULT value from last error.
#ifndef RESULT_FROM_KNOWN_LASTERROR
#define RESULT_FROM_KNOWN_LASTERROR             \
        {                                       \
            const DWORD err = ::GetLastError(); \
            if (ERROR_SUCCESS == err)           \
            {                                   \
                return S_OK;                    \
            }                                   \
            else                                \
            {                                   \
                return HRESULT_FROM_WIN32(err); \
            }                                   \
        }
#endif // RESULT_FROM_KNOWN_LASTERROR


// This macro function calls the C runtime's _beginthreadex function. 
// The C runtime library doesn't want to have any reliance on Windows' data 
// types such as HANDLE. This means that a Windows programmer needs to cast
// values when using _beginthreadex. Since this is terribly inconvenient, 
// I created this macro to perform the casting.
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#ifndef chBEGINTHREADEX
#define chBEGINTHREADEX(psa, cbStackSize, pfnStartAddr, \
   pvParam, dwCreateFlags, pdwThreadId)                 \
      ((HANDLE)_beginthreadex(                          \
         (void *)        (psa),                         \
         (unsigned)      (cbStackSize),                 \
         (PTHREAD_START) (pfnStartAddr),                \
         (void *)        (pvParam),                     \
         (unsigned)      (dwCreateFlags),               \
         (unsigned *)    (pdwThreadId)))
#endif // chBEGINTHREADEX


#ifndef ARRAYSIZE
#define ARRAYSIZE(a)       ( sizeof((a)) / sizeof((a)[0]) )
#endif // ARRAYSIZE


#ifndef GET_RIGHT_VALUE
#define GET_RIGHT_VALUE(val, max, min)    (val > max) ? max : ((val < min) ? min : val)
#endif // GET_RIGHT_VALUE


#ifndef MAX
#define MAX(x, y)           ( ((x) > (y) ) ? (x) : (y) )
#endif // MAX


#ifndef MIN
#define MIN(x, y)           ( ((x) < (y) ) ? (x) : (y) )
#endif // MIN


#ifndef UPCASE
#define UPCASE(c)           ( ((c)>='a' && (c)<='z') ? ((c)-0x20) : (c) )
#endif // UPCASE


#ifndef RECT_HEIGHT
#define RECT_HEIGHT(rc)     ( (rc).bottom - (rc).top )
#endif // RECT_HEIGHT


#ifndef RECT_WIDTH
#define RECT_WIDTH(rc)      ( (rc).right - (rc).left )
#endif // RECT_WIDTH


#endif // _SDKCOMMONMACRO_H_
#endif // __cplusplus


//////////////////////////////////////////////////////////////////////////
//
// END of file
//
//////////////////////////////////////////////////////////////////////////
