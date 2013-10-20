#pragma once

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugins/Parameter.h"

namespace bv { namespace model
{

class TexturePixelShaderChannelPD : public BaseParametersDescriptor
{
public:

    typedef std::vector< std::string > StringsVector;

    static const std::string            pluginName;

    static StringsVector                alphaParamName;
    static StringsVector                txMatrixParamName;

    TexturePixelShaderChannelPD()
        : BaseParametersDescriptor( pluginName )
    {
        alphaParamName.resize( 15 );
        alphaParamName[ 0 ] = "alpha0";
        alphaParamName[ 1 ] = "alpha1";
        alphaParamName[ 2 ] = "alpha2";
        alphaParamName[ 3 ] = "alpha3";
        alphaParamName[ 4 ] = "alpha4";
        alphaParamName[ 5 ] = "alpha5";
        alphaParamName[ 6 ] = "alpha6";
        alphaParamName[ 7 ] = "alpha7";
        alphaParamName[ 8 ] = "alpha8";
        alphaParamName[ 9 ] = "alpha9";
        alphaParamName[ 10 ] = "alpha10";
        alphaParamName[ 11 ] = "alpha11";
        alphaParamName[ 12 ] = "alpha12";
        alphaParamName[ 13 ] = "alpha13";
        alphaParamName[ 14 ] = "alpha14";

        txMatrixParamName.resize( 16 );
        txMatrixParamName[ 0 ] = "txMat0";
        txMatrixParamName[ 1 ] = "txMat1";
        txMatrixParamName[ 2 ] = "txMat2";
        txMatrixParamName[ 3 ] = "txMat3";
        txMatrixParamName[ 4 ] = "txMat4";
        txMatrixParamName[ 5 ] = "txMat5";
        txMatrixParamName[ 6 ] = "txMat6";
        txMatrixParamName[ 7 ] = "txMat7";
        txMatrixParamName[ 8 ] = "txMat8";
        txMatrixParamName[ 9 ] = "txMat9";
        txMatrixParamName[ 10 ] = "txMat10";
        txMatrixParamName[ 11 ] = "txMat11";
        txMatrixParamName[ 12 ] = "txMat12";
        txMatrixParamName[ 13 ] = "txMat13";
        txMatrixParamName[ 14 ] = "txMat14";
        txMatrixParamName[ 15 ] = "txMat14";

        m_params[ "alpha0" ] = ParamType::PT_FLOAT;
        m_params[ "alpha1" ] = ParamType::PT_FLOAT;
        m_params[ "alpha2" ] = ParamType::PT_FLOAT;
        m_params[ "alpha3" ] = ParamType::PT_FLOAT;
        m_params[ "alpha4" ] = ParamType::PT_FLOAT;
        m_params[ "alpha5" ] = ParamType::PT_FLOAT;
        m_params[ "alpha6" ] = ParamType::PT_FLOAT;
        m_params[ "alpha7" ] = ParamType::PT_FLOAT;
        m_params[ "alpha8" ] = ParamType::PT_FLOAT;
        m_params[ "alpha9" ] = ParamType::PT_FLOAT;
        m_params[ "alpha10" ] = ParamType::PT_FLOAT;
        m_params[ "alpha11" ] = ParamType::PT_FLOAT;
        m_params[ "alpha12" ] = ParamType::PT_FLOAT;
        m_params[ "alpha13" ] = ParamType::PT_FLOAT;
        m_params[ "alpha14" ] = ParamType::PT_FLOAT;

        m_params[ "txMat0" ] = ParamType::PT_MAT4;
        m_params[ "txMat1" ] = ParamType::PT_MAT4;
        m_params[ "txMat2" ] = ParamType::PT_MAT4;
        m_params[ "txMat3" ] = ParamType::PT_MAT4;
        m_params[ "txMat4" ] = ParamType::PT_MAT4;
        m_params[ "txMat5" ] = ParamType::PT_MAT4;
        m_params[ "txMat6" ] = ParamType::PT_MAT4;
        m_params[ "txMat7" ] = ParamType::PT_MAT4;
        m_params[ "txMat8" ] = ParamType::PT_MAT4;
        m_params[ "txMat9" ] = ParamType::PT_MAT4;
        m_params[ "txMat10" ] = ParamType::PT_MAT4;
        m_params[ "txMat11" ] = ParamType::PT_MAT4;
        m_params[ "txMat12" ] = ParamType::PT_MAT4;
        m_params[ "txMat13" ] = ParamType::PT_MAT4;
        m_params[ "txMat14" ] = ParamType::PT_MAT4;
        m_params[ "txMat15" ] = ParamType::PT_MAT4;
    }
};


class TexturePixelShaderChannel : public model::ShaderChannel< model::IPixelShaderChannel, TexturePixelShaderChannelPD >
{
    std::vector< model::ParamFloat * >      m_alphaParams;
    std::vector< model::ValueFloat * >      m_alphaValues;

    std::vector< model::ParamTransform * >  m_texTransformParams;
    std::vector< model::ValueMat4 * >       m_texTransformValues;

public:
    virtual void                    Update( float t );

    TexturePixelShaderChannel( const std::string& shaderFile, const std::vector< FloatInterpolator >& alphas, const std::vector< TransformF >& texTransforms );
};

} // model
} // bv
