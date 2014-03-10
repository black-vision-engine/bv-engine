#pragma once

#include <string>

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model
{

class TextPixelShaderChannelPD
{
public:

    static const std::string            colorParam;

};

class TextPixelShaderChannel : public DefaultPixelShaderChannel
{
    ParamVec4                       m_color;
    ValueVec4Ptr                    m_colorVal;

public:

    virtual void                    Update( TimeType t )
    {
        m_colorVal->SetValue( m_color.Evaluate( t ) );
        //ShaderChannel::Update( t );
    }

    TextPixelShaderChannel( const std::string& shaderFile, const ParamVec4& color )
        : DefaultPixelShaderChannel( shaderFile, nullptr )
        , m_color(color)
    {
        //m_colorVal = ValueVec4Ptr( new model::ValueVec4( ParamDesc::colorParam ) );
        //RegisterValue(m_colorVal.get());
    }

};

} // model
} // bv