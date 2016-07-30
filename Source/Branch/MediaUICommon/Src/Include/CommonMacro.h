/*!
* @file CommonMacro.h 
* 
* @brief This file includes some common macro.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/04/20
*/

#ifdef __cplusplus
#ifndef _COMMONMACRO_H_
#define _COMMONMACRO_H_


//////////////////////////////////////////////////////////////////////////
//
// Macros definition section
//


#ifndef BEGIN_NAMESPACE
#define BEGIN_NAMESPACE(name)       namespace name {
#endif // BEGIN_NAMESPACE


#ifndef END_NAMESPACE
#define END_NAMESPACE               }
#endif // END_NAMESPACE


#ifndef USING_NAMESPACE
#define USING_NAMESPACE(name)       using namespace name;
#endif // USING_NAMESPACE


#define BEGIN_NAMESPACE_COMMON      BEGIN_NAMESPACE(CommonLib)
#define USING_NAMESPACE_COMMON      USING_NAMESPACE(CommonLib)
#define BEGIN_NAMESPACE_UI          BEGIN_NAMESPACE(UILib)
#define USING_NAMESPACE_UI          USING_NAMESPACE(UILib)


#ifndef SAFE_ADDREF
#define SAFE_ADDREF(p)              \
        {                           \
            if ((p) != NULL)        \
            {                       \
                (p)->AddRef();      \
            }                       \
        }
#endif // SAFE_ADDREF


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


#ifndef ISVALIDHANDLE
#define ISVALIDHANDLE(h)        ( (h) != NULL && (h) != INVALID_HANDLE_VALUE )
#endif // ISVALIDHANDLE


#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE(h)                    \
        {                                       \
            if (ISVALIDHANDLE((h)))             \
            {                                   \
                CloseHandle((h));               \
                (h) = NULL;                     \
            }                                   \
        }
#endif // SAFE_CLOSE_HANDLE


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


#ifndef RECT_TO_D2DRECT
#define RECT_TO_D2DRECT(rc)                     \
        {                                       \
            (FLOAT)((rc).left),                 \
            (FLOAT)((rc).top),                  \
            (FLOAT)((rc).right),                \
            (FLOAT)((rc).bottom)                \
        }
#endif // RECT_TO_D2DRECT


#ifndef D2DRECT_TO_RECT
#define D2DRECT_TO_RECT(rc)                     \
        {                                       \
            (LONG)((rc).left),                  \
            (LONG)((rc).top),                   \
            (LONG)((rc).right),                 \
            (LONG)((rc).bottom)                 \
        }
#endif // D2DRECT_TO_RECT


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)       ( sizeof((a)) / sizeof((a)[0]) )
#endif // ARRAY_SIZE


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


#ifndef LOWERCASE
#define LOWERCASE(str)      CharLower(const_cast<TCHAR*>(str.c_str()))
#endif // LOWERCASE


#ifndef UPPERCASE
#define UPPERCASE(str)      CharUpper(const_cast<TCHAR*>(str.c_str()))
#endif // UPPERCASE


#ifndef RECT_HEIGHT
#define RECT_HEIGHT(rc)     ( (rc).bottom - (rc).top )
#endif // RECT_HEIGHT


#ifndef RECT_WIDTH
#define RECT_WIDTH(rc)      ( (rc).right - (rc).left )
#endif // RECT_WIDTH


#endif // _COMMONMACRO_H_
#endif // __cplusplus


//////////////////////////////////////////////////////////////////////////
//
// END of file
//
//////////////////////////////////////////////////////////////////////////
