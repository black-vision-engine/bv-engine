#pragma once

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
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

class SolidColorShaderChannel : public model::PixelShaderChannelBase< SolidColorShaderChannelPD >
{
private:

    Vec4Interpolator                m_solidColor;
    model::ValueVec4*               m_solidColorVal;

public:

    explicit                        SolidColorShaderChannel ( const std::string & shaderFile, const Vec4Interpolator & color );
    virtual                         ~SolidColorShaderChannel();

    virtual void                    Update                  ( TimeType t );

};


} // model
} // bv