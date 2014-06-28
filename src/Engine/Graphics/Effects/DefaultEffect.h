#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Models/Interfaces/ITexturesData.h"


namespace bv {

class IShaderDataSource;
class ITextureParams;
class ITextureDescriptor;
class IAnimationDescriptor;

class Texture2DImpl;
class Texture2DSequenceImpl;


class DefaultEffect : public RenderableEffect
{
public:

        DefaultEffect   ( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
        ~DefaultEffect  ();

protected:

    ShaderParameters *  CreateDefaultParamsPS   ( const IShaderDataSource * ds ) const;
    ShaderParameters *  CreateDefaultParamsVS   ( const IShaderDataSource * ds ) const;
    ShaderParameters *  CreateDefaultParamsGS   ( const IShaderDataSource * ds ) const;

    void                AddTextures             ( Shader * shader, ITexturesDataConstPtr txData );

private:

    ShaderParameters *      CreateDefaultParamsImpl ( const IShaderDataSource * ds ) const;

    TextureSampler *        CreateSampler           ( const ITextureParams * txParams, unsigned int samplerNum ) const;

    Texture2DImpl *         CreateTexture           ( const ITextureDescriptor * txParams ) const;
    Texture2DSequenceImpl * CreateSequence          ( const IAnimationDescriptor * animParams ) const;

};

} //bv
