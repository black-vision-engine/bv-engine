#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class IShaderDataSource;
class ShaderParameters;
class Texture2DImpl;
class Texture2DSequenceImpl;

class DefaultEffect : public RenderableEffect
{
public:

        DefaultEffect   ( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
        ~DefaultEffect  ();

protected:

    ShaderParameters *  DefaultParamsPS ( const IShaderDataSource * ds ) const;
    ShaderParameters *  DefaultParamsVS ( const IShaderDataSource * ds ) const;
    ShaderParameters *  DefaultParamsGS ( const IShaderDataSource * ds ) const;

    void                AddTextures     ( Shader * shader, const ITexturesData * txData );

private:

    ShaderParameters *      DefaultParamsImpl   ( const IShaderDataSource * ds ) const;

    TextureSampler *        CreateSampler       ( const ITextureParams * txParams, unsigned int samplerNum ) const;

    Texture2DImpl *         CreateTexture       ( const ITextureDescriptor * txParams ) const;
    Texture2DSequenceImpl * CreateSequence      ( const IAnimationDescriptor * animParams ) const;

};

} //bv
