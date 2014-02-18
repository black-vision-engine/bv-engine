#pragma once

#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/TypedValues.h"


namespace bv{ namespace model {

class ExtrudeGeometryShaderChannelPD
{
public:

    static const std::string            scaleParamName;

};

class ExtrudeGeometryShaderChannel : public ShaderChannel< IGeometryShaderChannel >
{
private:

    ParamFloat                      m_scale;
    ValueFloatPtr                   m_scaleVal;

public:

    virtual void                    Update                          ( TimeType t );

    explicit                        ExtrudeGeometryShaderChannel    ( const std::string & shaderFile, const ParamFloat & scale );
    virtual                         ~ExtrudeGeometryShaderChannel   ();

};


} // model
} // bv
