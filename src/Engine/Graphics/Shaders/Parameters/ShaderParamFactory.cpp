#include "ShaderParamFactory.h"

#include <cassert>

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec4.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat4.h"


namespace bv {

// ********************************
//
GenericShaderParam *    ShaderParamFactory::CreateParameter( const IValue * value )
{
    assert( value != nullptr );

    std::string name    = value->GetName();
    ParamType type      = value->GetType();

    switch( type )
    {
        case ParamType::PT_FLOAT4:
            return new ShaderParamVec4( name, QueryTypedValue< ValueVec4 >( value ) );
        case ParamType::PT_MAT4:
            return new ShaderParamMat4( name, QueryTypedValue< ValueMat4 >( value ) );
    
        default:
            assert( false );
    }

    return nullptr;
}

} //bv
