#include "DefaultVertexShaderChannel.h"

//FIXME: REMOVE after reimplementing access to values from shader channel implementation
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

// ******************************
//
DefaultVertexShaderChannel::DefaultVertexShaderChannel  ( const std::string & shaderSource, const IValueSet * values )
    : ShaderChannel( shaderSource, values )
{
}

// ******************************
//
DefaultVertexShaderChannel * DefaultVertexShaderChannel::Create              ( const std::string & shaderFile, const IValueSet * values )
{
    const IValueSet * vs = values;

    if( !values )
    {
        vs = new DefaultParamValModel();
    }

    std::stringstream shaderSource;

    //FIXME: move reading file to superclass or utility code
    File::Open( shaderFile ) >> shaderSource;

    if( shaderSource.str() == "" )
    {
        return nullptr;
    }

    return new DefaultVertexShaderChannel( shaderSource.str(), vs );
}

} //model
} //bv
