#include "TexturePixelShaderChannel.h"

namespace bv { namespace model
{


void TexturePixelShaderChannel::Update( float t )
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

TexturePixelShaderChannel::TexturePixelShaderChannel( const std::string& shaderFile
                                                    , const std::vector< FloatInterpolator >& alphas
                                                    , const std::vector< TransformF >& texTransforms )
    : ShaderChannel( shaderFile )
{
    ParamDesc::alphaParamName.resize( alphas.size() );
    for( unsigned int i = 0; i < alphas.size(); ++i )
    {
        ParamDesc::alphaParamName[ i ] = "alpha" + std::to_string( i );
        m_alphaParams.push_back( new model::ParamFloat( ParamDesc::alphaParamName[ i ], alphas[ i ] ) );
        m_alphaValues.push_back( new model::ValueFloat( ParamDesc::alphaParamName[ i ] ) );
        RegisterValue( m_alphaValues[ i ] );
    }


    ParamDesc::txMatrixParamName.resize( texTransforms.size() );
    for( unsigned int i = 0; i < texTransforms.size(); ++i )
    {
        ParamDesc::txMatrixParamName[ i ] = "txMat" + std::to_string( i );
        m_texTransformParams.push_back( new model::ParamTransform( ParamDesc::txMatrixParamName[ i ], texTransforms[ i ] ) ) );
        m_texTransformValues.push_back( new model::ValueMat4( ParamDesc::txMatrixParamName[ i ] ) );
        RegisterValue( m_texTransformValues[ i ] );
    }
}

} // model
} // bv
