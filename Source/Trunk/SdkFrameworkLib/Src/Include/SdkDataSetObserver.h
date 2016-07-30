/*!
* @file SdkDataSetObserver.h
* 
* @brief This file defines class SdkDataSetObserver, Receives call backs when a data set has been changed.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#ifdef __cplusplus
#ifndef _DATASETOBSERVER_H_
#define _DATASETOBSERVER_H_

#include "SdkCommonInclude.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief Receives call backs when a data set has been changed.
*/
class CLASS_DECLSPEC SdkDataSetObserver
{
public:

    /*!
    * @brief The construct function.
    */
    SdkDataSetObserver();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkDataSetObserver();

    /*!
    * @brief This method is called when the entire data set has changed.
    */
    virtual void OnDataChanged();
};

END_NAMESPACE_VIEWS

#endif // _DATASETOBSERVER_H_
#endif // __cplusplus
