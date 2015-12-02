#include "ShiftReplicationModifier.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

namespace bv { namespace model
{

// *******************************
//
ShiftReplicationModifier::ShiftReplicationModifier()
{}

// *******************************
//
void                            ShiftReplicationModifier::AddParamShift( const std::string & pluginName, const std::string & paramName, ParamValDelta & shift )
{
    auto p = std::make_pair( pluginName, paramName );
    m_paramsShifts[ p ] = shift;
}


// *******************************
//
namespace
{

template< class ParamTypePtr, ParamType paramType >
void ApplyParamDelta( IParameterPtr param, ParamValDelta delta )
{
    typedef std::shared_ptr< ValueImpl< ParamTypePtr::element_type::ValType, paramType > > ValType;

    if( auto p = QueryTypedParam< ParamTypePtr >( param ) )
    {

        auto keys = p->AccessInterpolator().GetKeys();

        bool valueSet = false;

        for( auto & k : keys )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                k.val += QueryTypedValue< ValType >( delta.delta )->GetValue();
                valueSet = true;
            }
        }

        if( !valueSet )
        {
            auto val = p->AccessInterpolator().Evaluate( delta.startTime );
            p->SetVal( val + QueryTypedValue< ValType >( delta.delta )->GetValue(), delta.startTime + delta.deltaTime );
        }
    }
}

} // anonymous

// *******************************
//
void                            ShiftReplicationModifier::Apply( const BasicNodeConstPtr &, const BasicNodePtr & next ) const
{
    for( auto it : m_paramsShifts )
    {
        if( auto p = next->GetPlugin( it.first.first ) )
        {
            if( auto param = p->GetParameter( it.first.second ) )
            {
                switch( it.second.delta->GetType() )
                {
                case ParamType::PT_FLOAT1:
                    ApplyParamDelta< ParamFloatPtr, ParamType::PT_FLOAT1 >( param, it.second );                        
                    break;
                case ParamType::PT_FLOAT2:
                    ApplyParamDelta< ParamVec2Ptr, ParamType::PT_FLOAT2 >( param, it.second );
                    break;
                case ParamType::PT_FLOAT3:
                    ApplyParamDelta< ParamVec3Ptr, ParamType::PT_FLOAT3 >( param, it.second );
                    break;
                case ParamType::PT_FLOAT4:
                    ApplyParamDelta< ParamVec4Ptr, ParamType::PT_FLOAT4 >( param, it.second );
                    break;
                case ParamType::PT_MAT2:
                    assert( false ); // TODO: Implement
                    //ApplyParamDelta< ParamMat2Ptr, ParamType::PT_MAT2 >( param, it.second );
                    break;
                case ParamType::PT_MAT3:
                    assert( false ); // TODO: Implement
                    //ApplyParamDelta< ParamMat3Ptr, ParamType::PT_MAT3 >( param, it.second );
                    break;
                case ParamType::PT_MAT4:
                    assert( false ); // TODO: Implement
                    //ApplyParamDelta< ParamMat4Ptr, ParamType::PT_MAT4 >( param, it.second );
                    break;
                case ParamType::PT_INT:
                    ApplyParamDelta< ParamIntPtr, ParamType::PT_INT >( param, it.second );
                    break;
                case ParamType::PT_BOOL:
                    //ApplyParamDelta< ParamBoolPtr, ParamType::PT_BOOL >( param, it.second );
                    break;
                case ParamType::PT_ENUM:
                    //ApplyParamDelta< ParamMat2Ptr, ParamType::PT_MAT2 >( param, it.second );
                    break;
                case ParamType::PT_STRING:
                    ApplyParamDelta< ParamStringPtr, ParamType::PT_STRING >( param, it.second );
                    break;
                case ParamType::PT_WSTRING:
                    ApplyParamDelta< ParamWStringPtr, ParamType::PT_WSTRING >( param, it.second );
                    break;
                }
            }
        }
    }
}

// *******************************
//
ShiftReplicationModifierConstPtr        ShiftReplicationModifier::Create()
{
    return ShiftReplicationModifierConstPtr( new ShiftReplicationModifier() );
}

} // model
} // bv