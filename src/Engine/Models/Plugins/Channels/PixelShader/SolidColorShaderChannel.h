#pragma once

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugins/Parameter.h"

namespace bv{ namespace model {

class SolidColorShaderChannelPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;// FIXME: To jest tu niepotrzebne
    static const std::string            solidColor;

    explicit SolidColorShaderChannelPD()
        : BaseParametersDescriptor( pluginName ) 
    {
        m_params[ solidColor ] = ParamType::PT_FLOAT4;
    }
};

class SolidColorShaderChannel : public model::ShaderChannel< IPixelShaderChannel, SolidColorShaderChannelPD >
{
    Vec4Interpolator                m_solidColor;
    model::ValueVec4*               m_solidColorVal;

public:

    virtual void                    Update( float t );

    explicit                        SolidColorShaderChannel(const std::string& shaderFile, const Vec4Interpolator& color);

    virtual                         ~SolidColorShaderChannel();
};


} // model
} // bv