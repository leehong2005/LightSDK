/*!
* @file DataSetObserver.h
* 
* @brief This file defines class DataSetObserver, Receives call backs when a data set has been changed.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#ifdef __cplusplus
#ifndef _DATASETOBSERVER_H_
#define _DATASETOBSERVER_H_

#include "CommonInclude.h"

/*!
* @brief Receives call backs when a data set has been changed.
*/
class CLASS_DECLSPEC DataSetObserver
{
public:

    /*!
    * @brief The construct function.
    */
    DataSetObserver();

    /*!
    * @brief The destructor function.
    */
    virtual ~DataSetObserver();

    /*!
    * @brief This method is called when the entire data set has changed.
    */
    virtual void OnDataChanged();
};

#endif // _DATASETOBSERVER_H_
#endif // __cplusplus
