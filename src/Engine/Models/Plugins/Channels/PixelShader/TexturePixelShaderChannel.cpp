#include "TexturePixelShaderChannel.h"

#include <string>
#include <vector>


namespace bv { namespace model
{

const std::string TexturePixelShaderChannelPD::pluginName( "SimpleTexturePlugin" );
TexturePixelShaderChannelPD::StringsVector TexturePixelShaderChannelPD::alphaParamName;
TexturePixelShaderChannelPD::StringsVector TexturePixelShaderChannelPD::txMatrixParamName;

// ******************************
//
void TexturePixelShaderChannel::Update( TimeType t )
{
    ShaderChannel::Update( t );

    assert( m_alphaParams.size() == m_alphaValues.size() );

    assert( m_texTransformParams.size() == m_texTransformValues.size() );

    for( unsigned int i = 0; i < m_alphaParams.size(); ++i )
    {
        m_alphaValues[ i ]->SetValue( m_alphaParams[ i ]->Evaluate( t ) );
        m_texTransformValues[ i ]->SetValue( m_texTransformParams[ i ]->Evaluate( t ) );
    }

    for( unsigned int i = 0; i < m_texTransformParams.size(); ++i )
    {
        m_texTransformValues[ i ]->SetValue( m_texTransformParams[ i ]->Evaluate( t ) );
    }

}

// ******************************
//
TexturePixelShaderChannel::TexturePixelShaderChannel( const std::string & shaderFile
                                                    , const std::vector< FloatInterpolator > & alphas
                                                    , const std::vector< TransformF > & texTransforms )
    : PixelShaderChannelBase( shaderFile )
{
    ParamDesc::alphaParamName.resize( alphas.size() );
    for( unsigned int i = 0; i < alphas.size(); ++i )
    {    
        m_alphaParams.push_back( new model::ParamFloat( ParamDesc::alphaParamName[ i ], alphas[ i ] ) );
        m_alphaValues.push_back( new model::ValueFloat( ParamDesc::alphaParamName[ i ] ) );
        RegisterValue( m_alphaValues[ i ] );
    }


    ParamDesc::txMatrixParamName.resize( texTransforms.size() );
    for( unsigned int i = 0; i < texTransforms.size(); ++i )
    {
        m_texTransformParams.push_back( new model::ParamTransform( ParamDesc::txMatrixParamName[ i ], texTransforms[ i ] ) );
        m_texTransformValues.push_back( new model::ValueMat4( ParamDesc::txMatrixParamName[ i ] ) );
        RegisterValue( m_texTransformValues[ i ] );
    }
}

} // model
} // bv
