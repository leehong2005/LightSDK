/*!
* @file ProcessBar.h 
* 
* @brief This file defines the class ProcessBar, it's a control for display process.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/24
*/

#ifdef  __cplusplus
#ifndef _SDKPROGRESSBAR_H_
#define _SDKPROGRESSBAR_H_

#include "SdkViewElement.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief This file defines the class ProcessBar, it's a control for display process.
*/
class CLASS_DECLSPEC SdkProgressBar : public SdkViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkProgressBar();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkProgressBar();

    /*!
    * @brief Set the percent of the SeekBar.
    *
    * @param fPercent  [I/ ] The percent, from 0 to 100.
    */
    virtual void SetPercent(FLOAT fPercent);

    /*!
    * @brief Get the current percent of seek bar.
    *
    * @return The value of percent.
    */
    virtual FLOAT GetPercent() const;

    /*!
    * @brief Set event handler of seek bar.
    *
    * @param pProgressBarHandler   [I/ ] The pointer to IProgressBarEventHandler.
    */
    virtual void SetProgressBarEventHandler(IProgressBarEventHandler *pProgressBarHandler);

protected:

    /*!
    * @brief Called when get the absolute rectangle of progress bar track.
    */
    virtual D2D1_RECT_F OnCalcTrackRect();

    /*!
    * @brief Called when get the width of progress bar progress.
    *
    * @return The width of the progress.
    */
    virtual FLOAT OnGetProgressWidth();

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief The framework calls this method when the percent is changed.
    *
    * @param fCurPercent    [I/ ] The current percent.
    */
    virtual void OnPercentChanged(FLOAT fCurPercent);

private:

    /*!
    * @brief The internal data of seek bar.
    */
    struct _PROGRESSBAR_INTERNALDATA;

    _PROGRESSBAR_INTERNALDATA   *m_pProgressBarData;        // The progress bar data.
};

END_NAMESPACE_VIEWS

#endif // _SDKPROGRESSBAR_H_
#endif // __cplusplus
