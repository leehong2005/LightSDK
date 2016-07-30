/*!
* @file SdkBase64Util.h
* 
* @brief Encode binary data using printable characters.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/22
*/

#ifdef __cplusplus
#ifndef _SDKBASE64_H_
#define _SDKBASE64_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

#define BASE64_LENGTH(inlen) ((((inlen) + 2) / 3) * 4)

/*!
* @brief Encode binary data using printable characters
*/
class CLASS_DECLSPEC SdkBase64Util
{
public:

    /*! 
    * @brief Base64 encode IN array of size INLEN into OUT array of size OUTLEN
    *
    * @param in     [I/ ] IN array
    * @param inlen  [I/ ] IN array of size
    * @param out    [ /O] OUT array
    * @param outlen [I/ ] OUT array of size
    */
    static void Encode(IN const char* in, size_t inlen, OUT char* out, size_t outlen);

    /*! 
    * @brief Base64 decode IN array of size INLEN into OUT array of size OUTLEN
    *
    * @param in     [I/ ] IN array
    * @param inlen  [I/ ] IN array of size
    * @param out    [ /O] OUT array
    * @param outlen [I/ ] OUT array of size
    */
    static BOOL Decode (IN const char* in, size_t inlen, OUT char* out, size_t* outlen);

private:

    /*! 
    * @brief Return true if ch is a character from the Base64 alphabet, and FALSE otherwise.
    * 
    * @param ch    [I/ ] a character
    *
    * @retval TRUE is a character from the Base64 alphabet.
    *         FALSE is not a character from the Base64 alphabet.
    */
    static BOOL Isbase64 (char ch);

private:

    static const char m_b64[];      // With this approach this file works independent of the charset used
    static const char m_b64str[];   // A const string of this approach this file works 
                                    // independent of the charset used
};

END_NAMESPACE_UTILITIES

#endif // _SDKBASE64_H_
#endif // __cplusplus
