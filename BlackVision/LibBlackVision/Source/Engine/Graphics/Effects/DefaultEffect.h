#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Models/Interfaces/ITexturesData.h"

#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"


namespace bv {

class IShaderDataSource;
class ITextureParams;
class ITextureDescriptor;
class IAnimationDescriptor;
class IMultipassShaderDataSource;

class DefaultEffect : public RenderableEffect
{
public:

        //FIXME: this is a HACK 
        DefaultEffect   ( const IMultipassShaderDataSource * msds );

        DefaultEffect   ( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
        ~DefaultEffect  ();

protected:			///FIXME changed from private

    RenderablePass *    CreateRenderablePass    ( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds ) const;

protected:

    ShaderParameters *  CreateDefaultParamsPS   ( const IShaderDataSource * ds ) const;
    ShaderParameters *  CreateDefaultParamsVS   ( const IShaderDataSource * ds ) const;
    ShaderParameters *  CreateDefaultParamsGS   ( const IShaderDataSource * ds ) const;

    void                AddTextures             ( Shader * shader, ITexturesDataConstPtr txData ) const;

private:

    ShaderParameters *      CreateDefaultParamsImpl ( const IShaderDataSource * ds ) const;

    TextureSampler *        CreateSampler           ( const ITextureParams * txParams, unsigned int samplerNum ) const;

    Texture2DPtr            GetTexture              ( const ITextureDescriptor * txParams ) const;
    Texture2DPtr            GetSequence             ( const IAnimationDescriptor * animParams ) const;

};

} //bv
