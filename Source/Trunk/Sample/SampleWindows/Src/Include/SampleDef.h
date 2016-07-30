#ifdef __cplusplus
#ifndef _SAMPLEDEF_H_
#define _SAMPLEDEF_H_

#include "SdkCommon.h"

//
// Function define.
//
#define _SetViewPos             0
#define _SetViewSize            1
#define _SetVisible             2
#define _SetEnable              3
#define _SetToggleStyle         4
#define _ShowUACIcon            5
#define _SetText                6
#define _SetTextAlign           7
#define _SetTextSize            8


#define PARAMTYPE_INT           0
#define PARAMTYPE_UINT32        1
#define PARAMTYPE_STRING        2
#define PARAMTYPE_BOOL          3


inline LPTSTR FunTypeToString(int funType)
{
#define FUNTYPETOSTRING(x)  if ((x) == funType) return _T(#x)

    FUNTYPETOSTRING(_SetViewPos);
    FUNTYPETOSTRING(_SetViewSize);
    FUNTYPETOSTRING(_SetEnable);
    FUNTYPETOSTRING(_SetVisible);
    FUNTYPETOSTRING(_SetToggleStyle);
    FUNTYPETOSTRING(_ShowUACIcon);
    FUNTYPETOSTRING(_SetText);
    FUNTYPETOSTRING(_SetTextAlign);
    FUNTYPETOSTRING(_SetTextSize);

    return _T("Unknown");
}


inline LPTSTR ParamTypeToString(int type)
{
    LPTSTR lpType = NULL;

    switch (type)
    {
    case PARAMTYPE_INT:
        lpType = _T("int");
        break;

    case PARAMTYPE_UINT32:
        lpType = _T("UINT32");
        break;

    case PARAMTYPE_STRING:
        lpType = _T("LPCTSTR");
        break;

    case PARAMTYPE_BOOL:
        lpType = _T("BOOL");
        break;
    }

    return lpType;
}


typedef struct _PARAMINFO
{
    int nType;
    LPTSTR strName;
    wstring strValue;

} PARAMINFO, *LPPARAMINFO;


typedef struct _FUNCTIONINFO
{
    int nFunId;
    LPTSTR    strFunName;
    LPTSTR    strRetVal;
    BOOL isSet;
    int nNumParam;
    PARAMINFO szParams[5];

} FUNCTIONINFO, *LPFUNCTIONINFO;





typedef enum _CONTROLTYPE
{
    TYPE_BUTTON         = 0,
    TYPE_CHECKBOX       = 1,
    TYPE_COMBOBOX       = 2,
    TYPE_EDITBOX        = 3,
    TYPE_GIFVIEW        = 4,
    TYPE_GROUPBOX       = 5,
    TYPE_IMAGEVIEW      = 6,
    TYPE_OVERLAP        = 7,
    TYPE_PROGRESS       = 8,
    TYPE_RADIOBUTTON    = 9,
    TYPE_RATINGVIEW     = 10,
    TYPE_SCROLLBAR      = 11,
    TYPE_SEEKBAR        = 12,
    TYPE_TABHEADER      = 13,
    TYPE_TABVIEW        = 14,
    TYPE_TEXTVIEW       = 15,
    TYPE_LISTBOX        = 16,

} CONTROLTYPE;


#define NAME_BUTTON         _T("Button")
#define NAME_CHECKBOX       _T("CheckBox")
#define NAME_COMBOBOX       _T("ComboBox")
#define NAME_EDITBOX        _T("EditBox")
#define NAME_GIFVIEW        _T("GifView")
#define NAME_GROUPBOX       _T("GroupBox")
#define NAME_IMAGEVIEW      _T("ImageView")
#define NAME_OVERLAP        _T("OverlapView")
#define NAME_PROGRESS       _T("Progress")
#define NAME_RADIOBUTTON    _T("RadioButton")
#define NAME_RATINGVIEW     _T("Rating")
#define NAME_SCROLLBAR      _T("ScrollBar")
#define NAME_SEEKBAR        _T("SeekBar")
#define NAME_TABHEADER      _T("TabHeader")
#define NAME_TABVIEW        _T("TabView")
#define NAME_TEXTVIEW       _T("TextView")
#define NAME_LISTBOX        _T("ListBox")


inline LPCTSTR CtrlTypeToName(CONTROLTYPE eCtrlType)
{
    LPCTSTR lpName = NULL;

    switch (eCtrlType)
    {
    case TYPE_BUTTON     :
        lpName = NAME_BUTTON;
        break;

    case TYPE_CHECKBOX   :
        lpName = NAME_CHECKBOX;
        break;

    case TYPE_COMBOBOX   :
        lpName = NAME_COMBOBOX;
        break;

    case TYPE_EDITBOX    :
        lpName = NAME_EDITBOX;
        break;

    case TYPE_GIFVIEW    :
        lpName = NAME_GIFVIEW;
        break;

    case TYPE_GROUPBOX   :
        lpName = NAME_GROUPBOX;
        break;

    case TYPE_IMAGEVIEW  :
        lpName = NAME_IMAGEVIEW;
        break;

    case TYPE_OVERLAP    :
        lpName = NAME_OVERLAP;
        break;

    case TYPE_PROGRESS   :
        lpName = NAME_PROGRESS;
        break;

    case TYPE_RADIOBUTTON:
        lpName = NAME_RADIOBUTTON;
        break;

    case TYPE_RATINGVIEW :
        lpName = NAME_RATINGVIEW;
        break;

    case TYPE_SCROLLBAR  :
        lpName = NAME_SCROLLBAR;
        break;

    case TYPE_SEEKBAR    :
        lpName = NAME_SEEKBAR;
        break;

    case TYPE_TABHEADER  :
        lpName = NAME_TABHEADER;
        break;

    case TYPE_TABVIEW    :
        lpName = NAME_TABVIEW;
        break;

    case TYPE_TEXTVIEW   :
        lpName = NAME_TEXTVIEW;
        break;

    case TYPE_LISTBOX:
        lpName = NAME_LISTBOX;
        break;
    }

    return lpName;
}


#endif // _SAMPLEDEF_H_
#endif // __cplusplus
