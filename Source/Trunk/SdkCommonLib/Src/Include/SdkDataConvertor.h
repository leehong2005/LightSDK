/*!
* @file SdkDataConvertor.h 
* 
* @brief Defines some functions to convert one data type to another data type.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/21
*/

#ifdef __cplusplus
#ifndef _SDKDATACONVERTOR_H_
#define _SDKDATACONVERTOR_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief Supply functions to convert data type.
*/
class CLASS_DECLSPEC SdkDataConvertor
{
public:

    /*!
    * @brief Convert boolean value to int.
    *
    * @param bValue     [I/ ] Value to be converted.
    * @param outValue   [ /O] Value to be converted.
    *
    * @return Converted data. returned value is 0 if the input cannot be converted.
    */
    static BOOL ToInt32(IN BOOL bValue, OUT INT32& outValue);

    /*!
    * @brief Convert string value to int.
    *
    * @param lpValue    [I/ ] Value to be converted.
    * @param outValue   [ /O] Value to be converted.
    *
    * @return TRUE if succeed to convert, FALSE otherwise.
    */
    static BOOL ToInt32(IN LPCWSTR lpValue, OUT INT32& outValue);

    /*!
    * @brief Convert int value to boolean.
    *
    * @param nValue     [I/ ] Value to be converted.
    * @param outValue   [ /O] Value to be converted.
    *
    * @return TRUE if succeed to convert, FALSE otherwise.
    */
    static BOOL ToBoolean(IN INT32 nValue, OUT BOOL& outValue);

    /*!
    * @brief Convert string value to boolean.
    *
    * @param lpValue    [I/ ] Value to be converted.
    * @param outValue   [ /O] Value to be converted.
    *
    * @return TRUE if succeed to convert, FALSE otherwise.
    */
    static BOOL ToBoolean(IN LPCWSTR lpValue, OUT BOOL& outValue);

    /*!
    * @brief Convert int value to string.
    *
    * @param nValue     [I/ ] Value to be converted.
    * @param outValue   [ /O] Value to be converted.
    *
    * @return TRUE if succeed to convert, FALSE otherwise.
    */
    static BOOL ToString(IN INT32 nValue, OUT wstring& outValue);

    /*!
    * @brief Convert int value to string.
    *
    * @param nValue     [I/ ] Value to be converted.
    * @param outValue   [ /O] Value to be converted.
    *
    * @return TRUE if succeed to convert, FALSE otherwise.
    */
    static BOOL ToString(IN bool bValue, OUT wstring& outValue);
};

END_NAMESPACE_UTILITIES

#endif // _SDKDATACONVERTOR_H_
#endif // __cplusplus
