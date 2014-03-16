#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class IShaderDataSource;
class ShaderParameters;


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

    ShaderParameters * DefaultParamsImpl( const IShaderDataSource * ds ) const;

};

} //bv
