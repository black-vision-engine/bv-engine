#include "ShaderParamFactory.h"

#include <cassert>

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFloat.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec2.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec3.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec4.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat2.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat3.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat4.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamMVP.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMV.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamP.h"


namespace bv {

// ********************************
//
GenericShaderParam *    ShaderParamFactory::CreateGenericParameter  ( const IValue * value )
{
    assert( value != nullptr );

    std::string name    = value->GetName();
    ParamType type      = value->GetType();

    switch( type )
    {
        case ParamType::PT_FLOAT1:
            return new ShaderParamFloat( name, QueryTypedValue< ValueFloat >( value ) );
        case ParamType::PT_FLOAT2:
            return new ShaderParamVec2( name, QueryTypedValue< ValueVec2 >( value ) );
        case ParamType::PT_FLOAT3:
            return new ShaderParamVec3( name, QueryTypedValue< ValueVec3 >( value ) );
        case ParamType::PT_FLOAT4:
            return new ShaderParamVec4( name, QueryTypedValue< ValueVec4 >( value ) );
        case ParamType::PT_MAT2:
            return new ShaderParamMat2( name, QueryTypedValue< ValueMat2 >( value ) );
        case ParamType::PT_MAT3:
            return new ShaderParamMat3( name, QueryTypedValue< ValueMat3 >( value ) );
        case ParamType::PT_MAT4:
            return new ShaderParamMat4( name, QueryTypedValue< ValueMat4 >( value ) );
    
        default:
            assert( false );
    }

    return nullptr;
}

// ********************************
//
GenericShaderParam *     CreateGenericParameter  ( const std::string & name, ParamType paramType )
{
    switch( paramType )
    {
        case ParamType::PT_FLOAT1:
            return new ShaderParamFloat( name, nullptr );
        case ParamType::PT_FLOAT2:
            return new ShaderParamVec2( name, nullptr );
        case ParamType::PT_FLOAT3:
            return new ShaderParamVec3( name, nullptr );
        case ParamType::PT_FLOAT4:
            return new ShaderParamVec4( name, nullptr );
        case ParamType::PT_MAT2:
            return new ShaderParamMat2( name, nullptr );
        case ParamType::PT_MAT3:
            return new ShaderParamMat3( name, nullptr );
        case ParamType::PT_MAT4:
            return new ShaderParamMat4( name, nullptr );
    
        default:
            assert( false );
    }

    return nullptr;
}

// ********************************
//
GenericShaderParam *     ShaderParamFactory::CreateMVPParameter      ()
{
    return new ShaderParamMVP( "MVP" );
}

// ********************************
//
GenericShaderParam *     ShaderParamFactory::CreateMVParameter       ()
{
    return new ShaderParamMVP( "MV" );
}

// ********************************
//
GenericShaderParam *     ShaderParamFactory::CreatePParameter        ()
{
    return new ShaderParamMVP( "P" );
}

} //bv
