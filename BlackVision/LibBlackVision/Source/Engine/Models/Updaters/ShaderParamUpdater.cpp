#include "stdafx.h"

#include "ShaderParamUpdater.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFloat.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec2.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec3.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec4.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat2.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat3.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat4.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamBool.h"


namespace bv {

namespace {

// *******************************
//
template< typename ValType, typename ShaderParamType >
inline void	UpdateTypedShaderParam   ( const IValue * source, GenericShaderParam * dest )
{
    static_cast< ShaderParamType * >( dest )->SetValue( QueryTypedValue< ValType >( source )->GetValue() );
}

} // anonymous

// *****************************
//
void            UpdateGenericShaderParam        ( const IValue * source, GenericShaderParam * dest )
{
    assert( source && source->GetType() == dest->Type() );
    
    switch( source->GetType() )
    {
    case ParamType::PT_FLOAT1:
        UpdateTypedShaderParam< ValueFloat, ShaderParamFloat >( source, dest );
        break;
    case ParamType::PT_INT:
        UpdateTypedShaderParam< ValueInt, ShaderParamInt >( source, dest );
        break;
    case ParamType::PT_FLOAT2:
        UpdateTypedShaderParam< ValueVec2, ShaderParamVec2 >( source, dest );
        break;
    case ParamType::PT_FLOAT3:
        UpdateTypedShaderParam< ValueVec3, ShaderParamVec3 >( source, dest );
        break;
    case ParamType::PT_FLOAT4:
        UpdateTypedShaderParam< ValueVec4, ShaderParamVec4 >( source, dest );
        break;
    case ParamType::PT_MAT2:
        UpdateTypedShaderParam< ValueMat2, ShaderParamMat2 >( source, dest );
        break;
    case ParamType::PT_MAT3:
        UpdateTypedShaderParam< ValueMat3, ShaderParamMat3 >( source, dest );
        break;
    case ParamType::PT_MAT4:
        UpdateTypedShaderParam< ValueMat4, ShaderParamMat4 >( source, dest );
        break;
	case ParamType::PT_BOOL:
		UpdateTypedShaderParam< ValueBool, ShaderParamBool >( source, dest );
		break;
	default:
        assert( false );
    }
}

} //bv
