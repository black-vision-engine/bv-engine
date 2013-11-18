#pragma once

#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugins/Parameter.h"


namespace bv{ namespace model {

class ExtrudeGeometryShaderChannelPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;// FIXME: To jest tu niepotrzebne
    static const std::string            scaleParamName;

    explicit ExtrudeGeometryShaderChannelPD()
        : BaseParametersDescriptor( pluginName ) 
    {
        m_params[ scaleParamName ] = ParamType::PT_FLOAT;
    }
};

class ExtrudeGeometryShaderChannel : public model::ShaderChannel< IGeometryShaderChannel, ExtrudeGeometryShaderChannelPD >
{
    FloatInterpolator                m_scale;
    model::ValueFloat*               m_scaleVal;

public:

    virtual void                    Update( TimeType t );

    explicit                        ExtrudeGeometryShaderChannel(const std::string& shaderFile, const FloatInterpolator& scale);

    virtual                         ~ExtrudeGeometryShaderChannel() {}
};


} // model
} // bv