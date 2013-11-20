#pragma once

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugins/Parameter.h"


namespace bv { namespace model
{

class TexturePixelShaderChannelPD : public BaseParametersDescriptor
{
public:

    static const std::string    pluginName;

    static std::string          alphaParamName[];
    static std::string          txMatrixParamName[];

    TexturePixelShaderChannelPD()
        : BaseParametersDescriptor( pluginName )
    {
        //FIXME: this is a static calss which means that initializing anything in constructor makes no sense
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


class TexturePixelShaderChannel : public model::PixelShaderChannelBase< TexturePixelShaderChannelPD >
{
    std::vector< model::ParamFloat >        m_alphaParams;
    std::vector< model::ValueFloat * >      m_alphaValues;

    std::vector< model::ParamTransform >    m_texTransformParams;
    std::vector< model::ValueMat4 * >       m_texTransformValues;

public:

    virtual void                    Update( TimeType t );

    TexturePixelShaderChannel( const std::string & shaderFile, const std::vector< ParamFloat > & alphas, const std::vector< ParamTransform > & texTransforms );

};

} // model
} // bv
