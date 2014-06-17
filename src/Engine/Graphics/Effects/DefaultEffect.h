#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Models/Interfaces/IOverrideState.h"
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

        DefaultEffect   ( const model::IOverrideState * state, const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
        ~DefaultEffect  ();

protected:

    ShaderParameters *  CreateDefaultParamsPS   ( const model::IOverrideState * state, const IShaderDataSource * ds ) const;
    ShaderParameters *  CreateDefaultParamsVS   ( const model::IOverrideState * state, const IShaderDataSource * ds ) const;
    ShaderParameters *  CreateDefaultParamsGS   ( const model::IOverrideState * state, const IShaderDataSource * ds ) const;

    void                AddTextures             ( Shader * shader, ITexturesDataConstPtr txData );

private:

    void                    AddOverrideParamsPS ( ShaderParameters * paramsPS, const model::IOverrideState * state ) const;
    void                    AddOverrideParamsVS ( ShaderParameters * paramsVS, const model::IOverrideState * state ) const;
    void                    AddOverrideParamsGS ( ShaderParameters * paramsGS, const model::IOverrideState * state ) const;

    ShaderParameters *      CreateDefaultParamsImpl ( const IShaderDataSource * ds ) const;

    TextureSampler *        CreateSampler           ( const ITextureParams * txParams, unsigned int samplerNum ) const;

    Texture2DImpl *         CreateTexture           ( const ITextureDescriptor * txParams ) const;
    Texture2DSequenceImpl * CreateSequence          ( const IAnimationDescriptor * animParams ) const;

};

} //bv
