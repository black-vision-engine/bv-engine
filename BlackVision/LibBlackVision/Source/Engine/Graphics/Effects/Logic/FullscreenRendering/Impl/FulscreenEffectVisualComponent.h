#pragma once

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/Impl/FulscreenEffectVisualComponentDesc.h"

#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"


namespace bv { 

class TriangleStrip;
class Shader;
class Renderer;



class FullscreenEffectVisualComponent
{
private:

    TriangleStrip *         m_quad;
    Shader *                m_pixelShader;

    unsigned int            m_numRenderTargetTextures;
    unsigned int            m_numStaticTextures;

    IValuePtrVec            m_values;

private:

                            FullscreenEffectVisualComponent    ( TriangleStrip * quad, unsigned int numRendenderTargetTextures, unsigned int numStaticTextures, const IValuePtrVec & values );

public:

                            ~FullscreenEffectVisualComponent   ();

    void                    Render                              ( Renderer * renderer );

    void                    SyncRenderTargets                   ( const RenderTargetVec & renderTargets ); // FIXME: nrl - possibly add an API to change sampler params as well (wrapX, wrapY, filteringMode) - not only the input render target
    void                    SyncTextures                        ( const Texture2DEntryVec & textures );
    void                    SyncRenderState                     ( const FullscreenEffectRendererState & state );
    void                    SyncIValues                         ( const IValuePtrVec & vals );

    const IValuePtrVec &    GetIValues                          () const;
    void                    AppendRenderPasses_DIRTY_HACK       ( std::set< const RenderablePass * > * passes ) const;

public:

    static FullscreenEffectVisualComponent *   Create  ( const FullscreenEffectVisualComponentDesc & desc );

private:

    void            UpdateShaderParams                  ();

    unsigned int    FirstRenderTargetTextureIdx         () const;
    unsigned int    FirstStaticTextureIdx               () const;

    void            SyncTexture                         ( unsigned int i, Texture2DPtr tex );
    void            SyncSampler                         ( unsigned int i, const std::string & name, TextureFilteringMode tfm, TextureWrappingMode wrapX, TextureWrappingMode wrapY );

    void            SyncStaticTexture                   ( unsigned int i, const Texture2DEntry & tex );

};


} //bv
