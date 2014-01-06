#pragma once

#include <string>

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/Parameters/TypedParameters.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"
#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model
{

class TextPixelShaderChannelPD
{
public:

    static const std::string            colorParam;

};

class TextPixelShaderChannel : public model::PixelShaderChannelBase< TextPixelShaderChannelPD >
{
    ParamVec4                       m_color;
    model::ValueVec4 *              m_colorVal;

public:

    virtual void                    Update( TimeType t )
    {
        m_colorVal->SetValue( m_color.Evaluate( t ) );
        ShaderChannel::Update( t );
    }

    TextPixelShaderChannel( const std::string& shaderFile, const ParamVec4& color )
        : PixelShaderChannelBase( shaderFile )
        , m_color(color)
    {
        m_colorVal = new model::ValueVec4( ParamDesc::colorParam );
        RegisterValue(m_colorVal);
    }

};

} // model
} // bv