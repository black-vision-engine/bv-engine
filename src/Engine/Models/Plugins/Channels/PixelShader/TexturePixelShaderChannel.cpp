#include "TexturePixelShaderChannel.h"

#include <string>
#include <vector>


namespace bv { namespace model
{

//FIXME: this constant should be stored in other place - some file responsible for storing static engine configuration
const unsigned int MAX_NUM_TEXTURES = 16;

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
    //ShaderChannel::Update( t );

    //assert( m_alphaParams.size() == m_alphaValues.size() );

    //assert( m_texTransformParams.NumTransforms() == m_texTransformValues.size() );

    //for( unsigned int i = 0; i < m_alphaParams.size(); ++i )
    //{
    //    m_alphaValues[ i ]->SetValue( m_alphaParams[ i ].Evaluate( t ) );
    //}

    //for( unsigned int i = 0; i < m_texTransformParams.NumTransforms(); ++i )
    //{
    //    m_texTransformValues[ i ]->SetValue( m_texTransformParams.Evaluate( i , t ) );
    //}
}

// ******************************
//
TexturePixelShaderChannel::TexturePixelShaderChannel( const std::string & shaderFile
                                                    , const std::vector< ParamFloat > & alphas
                                                    , const ParamTransformVec & texTransforms
                                                    , const std::vector< ParamVec4 > & borderColors )
    : DefaultPixelShaderChannel( shaderFile, nullptr )
    , m_texTransformParams( texTransforms )
{
//    assert( alphas.size() <= MAX_NUM_TEXTURES );
//    assert( texTransforms.NumTransforms() <= MAX_NUM_TEXTURES );
//
//    m_alphaParams = alphas;
//    for( unsigned int i = 0; i < alphas.size(); ++i )
//    {    
//        m_alphaValues.push_back( ValueFloatPtr( new ValueFloat( ParamDesc::alphaParamName[ i ] ) ) );
//        RegisterValue( m_alphaValues[ i ].get() );
//    }
//
//
//    m_texTransformParams = texTransforms;
//    for( unsigned int i = 0; i < texTransforms.NumTransforms(); ++i )
//    {
//        m_texTransformValues.push_back( ValueMat4Ptr( new ValueMat4( ParamDesc::txMatrixParamName[ i ] ) ) );
//        RegisterValue( m_texTransformValues[ i ].get() );
//    }
//
//    m_borderColorParams = borderColors;
//    for( unsigned int i = 0; i < borderColors.size(); ++i )
//    {
//        m_borderColorValues.push_back( ValueVec4Ptr( new ValueVec4( ParamDesc::borderColorsParamName[ i ] ) ) );
//        RegisterValue( m_borderColorValues[ i ].get() );
//    }
}

// ******************************
//
TexturePixelShaderChannel::~TexturePixelShaderChannel  ()
{
}

} // model
} // bv
