/*!
* @file CustomTextRenderer.h 
* 
* @brief This file defines the class CustomTextRenderer, is used to Generation text renderer.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/01/19
*/

#ifdef __cplusplus
#ifndef _CUSTOMTEXTRENDERER_H_
#define _CUSTOMTEXTRENDERER_H_

#include "CommonInclude.h"

/*!
* @brief This class implements the IDWriteTextRenderer.
*/
class CLASS_DECLSPEC CustomTextRenderer : public IDWriteTextRenderer
{
public:

    /*!
    * @brief The constructor function.
    * 
    * @param pD2DFactory        [I/ ] The Direct2D factory.
    * @param pRT                [I/ ] The render target.
    * @param pOutlineBrush      [I/ ] The solid brush.
    */
    CustomTextRenderer(ID2D1Factory* pD2DFactory, 
                       ID2D1RenderTarget* pRT, 
                       ID2D1Brush* pOutlineBrush);

    /*!
    * @brief Call this method determines whether pixel snapping is disabled. The recommended 
    *                           default is FALSE, unless doing animation 
    *                           that requires subPixel vertical placement.
    * 
    * @param clientDrawingContext       [I/ ] The client Draw context.It's can be NULL.
    * @param isDisabled                 [ /O] Whether pixel snapping is disabled.
    */
    IFACEMETHOD(IsPixelSnappingDisabled)(__maybenull void* clientDrawingContext,
                                         __out BOOL* isDisabled);

    /*!
    * @brief Call this method Returns the current transform applied to the render target.
    * 
    * @param clientDrawingContext       [I/ ] The client Draw context.It can be NULL.
    * @param transform                  [ /O] The transform matrix.
    */
    IFACEMETHOD(GetCurrentTransform)(__maybenull void* clientDrawingContext,
                                     __out DWRITE_MATRIX* transform);

    /*!
    * @brief Call this method returns the number of pixels per DIP.
    * 
    * @param clientDrawingContext       [I/ ] 
    * @param pixelsPerDip               [ /O] 
    */
    IFACEMETHOD(GetPixelsPerDip)(__maybenull void* clientDrawingContext,
                                 __out FLOAT* pixelsPerDip);

    /*!
    * @brief Call this method to draw GlyphRun.
    * 
    * @param clientDrawingContext       [I/ ] The client Draw context.It's can be NULL.
    * @param baselineOriginX            [I/ ] The origin position x coordinates of baseline.
    * @param baselineOriginY            [I/ ] The origin position y coordinates of baseline.
    * @param measuringMode              [I/ ] The measuring method used for text layout
    * @param glyphRun                   [I/ ] The glyphRun enumeration.
    * @param glyphRunDescription        [I/ ] The glyphRun description.
    * @param clientDrawingEffect        [ / ] The drawing effect of client want to set.It can be NULL.
    */
    IFACEMETHOD(DrawGlyphRun)(__maybenull void* clientDrawingContext,
                              FLOAT baselineOriginX,
                              FLOAT baselineOriginY,
                              DWRITE_MEASURING_MODE measuringMode,
                              __in DWRITE_GLYPH_RUN const* glyphRun,
                              __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
                              IUnknown* clientDrawingEffect);

    /*!
    * @brief Call this method to draw underLine below the text using a Direct2D rectangle.
    * 
    * @param clientDrawingContext       [I/ ] The client Draw context.It's can be NULL.
    * @param baselineOriginX            [I/ ] The origin position x coordinates of baseline.
    * @param baselineOriginY            [I/ ] The origin position y coordinates of baseline.
    * @param underline                  [I/ ] The underline object of draw on textLayout.
    * @param clientDrawingEffect        [I/ ] The drawing effect of client want to set.It can be NULL.
    */
    IFACEMETHOD(DrawUnderline)(__maybenull void* clientDrawingContext,
                               FLOAT baselineOriginX,
                               FLOAT baselineOriginY,
                               __in DWRITE_UNDERLINE const* underline,
                               IUnknown* clientDrawingEffect);

    /*!
    * @brief Call this method to draw strike through line below the text using a Direct2D.
    * 
    * @param clientDrawingContext       [ / ] The client Draw context.It's can be NULL.
    * @param baselineOriginX            [I/ ] The origin position x coordinates of baseline.
    * @param baselineOriginY            [I/ ] The origin position y coordinates of baseline.
    * @param strikethrough              [I/ ] The strikethrough enumeration.
    * @param clientDrawingEffect        [ / ] The drawing effect of client want to set.It can be NULL.
    */
    IFACEMETHOD(DrawStrikethrough)(__maybenull void* clientDrawingContext,
                                   FLOAT baselineOriginX,
                                   FLOAT baselineOriginY,
                                   __in DWRITE_STRIKETHROUGH const* strikethrough,
                                   IUnknown* clientDrawingEffect);

    /*!
    * @brief Call this method to draw inline object.
    * 
    * @param clientDrawingContext       [I/ ] The client Draw context.It's can be NULL.
    * @param originX                    [I/ ] The origin position x coordinates.
    * @param originY                    [I/ ] The origin position y coordinates.
    * @param inlineObject               [I/ ] The DwriteInlineObject.
    * @param isSideways                 [I/ ] Whether inline object is sideways.
    * @param isRightToLeft              [I/ ] Whether inline object from right to left.
    * @param clientDrawingEffect        [I/ ] The drawing effect of client want to set.It can be NULL.
    */
    IFACEMETHOD(DrawInlineObject)(__maybenull void* clientDrawingContext,
                                  FLOAT originX,
                                  FLOAT originY,
                                  IDWriteInlineObject* inlineObject,
                                  BOOL isSideways,
                                  BOOL isRightToLeft,
                                  IUnknown* clientDrawingEffect);

public:

    /*!
    * @brief Call this method to increments the ref count
    */
    IFACEMETHOD_(unsigned long, AddRef) ();

    /*!
    * @brief Call this method to decrements the ref count and deletes the instance 
    *                            if the ref count becomes 0.
    */
    IFACEMETHOD_(unsigned long, Release) ();

    /*!
    * @brief Call this method to query interface implementation
    * 
    * @param riid           
    * @param ppvObject      
    */
    IFACEMETHOD(QueryInterface) (IID const& riid, void** ppvObject);

private:

    /*!
    * @brief The destructor function.
    */
    virtual ~CustomTextRenderer();

private:

    unsigned long           n_cRefCount_;           // The reference count.
    ID2D1Factory           *m_pD2DFactory;          // The D2D factory.
    ID2D1RenderTarget      *m_pRenderTarget;        // The D2D renderTarget.
    ID2D1Brush             *m_pD2DBrush;            // The D2D brush.
};

#endif // _CUSTOMTEXTRENDERER_H_
#endif // __cplusplus
