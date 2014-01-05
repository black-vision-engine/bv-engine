#include "TexturePixelShaderChannel.h"

#include <string>
#include <vector>


namespace bv { namespace model
{

//FIXME: this constant should be stored in other place - some file responsible for storing static engine configuration
const unsigned int MAX_NUM_TEXTURES = 16;

const std::string TexturePixelShaderChannelPD::pluginName( "SimpleTexturePlugin" );

//FIXME: should be initialized after GL initialization
std::string TexturePixelShaderChannelPD::alphaParamName[ MAX_NUM_TEXTURES ]     = { "Alpha0", "Alpha1", "Alpha2", "Alpha3", "Alpha4", "Alpha5", "Alpha6", "Alpha7", "Alpha8"
                                                                                    "Alpha9", "Alpha10", "Alpha11", "Alpha12", "Alpha13", "Alpha14", "Alpha15" };

std::string TexturePixelShaderChannelPD::txMatrixParamName[ MAX_NUM_TEXTURES ]  = { "txMat0", "txMat1", "txMat2", "txMat3", "txMat4", "txMat5", "txMat6", "txMat7"
                                                                                    "txMat8", "txMat9", "txMat10", "txMat11", "txMat12", "txMat13", "txMat14", "txMat15" };

std::string TexturePixelShaderChannelPD::borderColorsParamName[ MAX_NUM_TEXTURES ] = { "borderColor0", "borderColor1", "borderColor2", "borderColor3", "borderColor4", "borderColor5", "borderColor6", "borderColor7"
                                                                                    "borderColor8", "borderColor9", "borderColor10", "borderColor11", "borderColor12", "borderColor13", "borderColor14", "borderColor15" };

// ******************************
//
void TexturePixelShaderChannel::Update( TimeType t )
{
    ShaderChannel::Update( t );

    assert( m_alphaParams.size() == m_alphaValues.size() );

    assert( m_texTransformParams.size() == m_texTransformValues.size() );

    for( unsigned int i = 0; i < m_alphaParams.size(); ++i )
    {
        m_alphaValues[ i ]->SetValue( m_alphaParams[ i ].Evaluate( t ) );
    }

    for( unsigned int i = 0; i < m_texTransformParams.size(); ++i )
    {
        m_texTransformValues[ i ]->SetValue( m_texTransformParams[ i ].Evaluate( t ) );
    }
}

// ******************************
//
TexturePixelShaderChannel::TexturePixelShaderChannel( const std::string & shaderFile
                                                    , const std::vector< ParamFloat > & alphas
                                                    , const std::vector< ParamTransform > & texTransforms
                                                    , const std::vector< ParamVec4 > & borderColors )
    : PixelShaderChannelBase( shaderFile )
{
    assert( alphas.size() <= MAX_NUM_TEXTURES );
    assert( texTransforms.size() <= MAX_NUM_TEXTURES );

    m_alphaParams = alphas;
    for( unsigned int i = 0; i < alphas.size(); ++i )
    {    
        m_alphaValues.push_back( new model::ValueFloat( ParamDesc::alphaParamName[ i ] ) );
        RegisterValue( m_alphaValues[ i ] );
    }


    m_texTransformParams = texTransforms;
    for( unsigned int i = 0; i < texTransforms.size(); ++i )
    {
        m_texTransformValues.push_back( new model::ValueMat4( ParamDesc::txMatrixParamName[ i ] ) );
        RegisterValue( m_texTransformValues[ i ] );
    }

    m_borderColorParams = borderColors;
    for( unsigned int i = 0; i < borderColors.size(); ++i )
    {
        m_borderColorValues.push_back( new model::ValueVec4( ParamDesc::borderColorsParamName[ i ] ) );
        RegisterValue( m_borderColorValues[ i ] );
    }
}

} // model
} // bv
