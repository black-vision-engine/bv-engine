#pragma once

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/TypedValues.h"


namespace bv{ namespace model {

class SolidColorShaderChannelPD
{
public:

    static const std::string            solidColor;

};

class SolidColorShaderChannel : public model::PixelShaderChannelBase
{
private:

    ParamVec4                       m_solidColor;
    ValueVec4Ptr                    m_solidColorVal;

public:

    explicit                        SolidColorShaderChannel ( const std::string & shaderFile, const ParamVec4 & color );
    virtual                         ~SolidColorShaderChannel();

    virtual void                    Update                  ( TimeType t );

};

} // model
} // bv
