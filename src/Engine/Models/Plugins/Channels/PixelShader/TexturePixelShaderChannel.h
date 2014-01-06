#pragma once

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv { namespace model
{

class TexturePixelShaderChannelPD : public BaseParametersDescriptor
{
public:
    static std::string          alphaParamName[];
    static std::string          txMatrixParamName[];
    static std::string          borderColorsParamName[];

    TexturePixelShaderChannelPD()
    {}
};


class TexturePixelShaderChannel : public model::PixelShaderChannelBase< TexturePixelShaderChannelPD >
{
    std::vector< model::ParamFloat >        m_alphaParams;
    std::vector< model::ValueFloat * >      m_alphaValues;

    std::vector< model::ParamTransform >    m_texTransformParams;
    std::vector< model::ValueMat4 * >       m_texTransformValues;

    std::vector< model::ParamVec4 >         m_borderColorParams;
    std::vector< model::ValueVec4 * >       m_borderColorValues;

public:

    virtual void                    Update( TimeType t );

    TexturePixelShaderChannel( const std::string & shaderFile, const std::vector< ParamFloat > & alphas, const std::vector< ParamTransform > & texTransforms, const std::vector< ParamVec4 > & borderColors );

};

} // model
} // bv
