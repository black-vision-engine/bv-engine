#include "ShiftReplicationModifier.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

namespace bv { namespace model
{

// *******************************
//
ShiftReplicationModifier::ShiftReplicationModifier()
{}

// ***********************
//
void                            ShiftReplicationModifier::Serialize       ( ISerializer& ser ) const
{
    ser.EnterArray( "replicatorModifier" );
        ser.SetAttribute( "type", "shiftReplicationModifier" );

        ser.EnterArray( "paramShifts" );
        for( auto& shift : m_paramsShifts )
        {
            ser.EnterChild( "paramShift" );
                ser.SetAttribute( "pluginName", shift.first.first );
                ser.SetAttribute( "paramName", shift.first.second );

                ser.EnterChild( "paramDelta" );
                    ser.SetAttribute( "startTime", SerializationHelper::T2String( shift.second.startTime ) );
                    ser.SetAttribute( "deltaTime", SerializationHelper::T2String( shift.second.deltaTime ) );

                ser.ExitChild();

            ser.ExitChild();
        }
        ser.ExitChild();


    ser.ExitChild();
}

// ***********************
//
ShiftReplicationModifierPtr     ShiftReplicationModifier::Create          ( const IDeserializer& /*deser*/ )
{
    auto shiftModifier = ShiftReplicationModifier::Create();
    return shiftModifier;
}

// *******************************
//
void                            ShiftReplicationModifier::AddParamShift( const std::string & pluginName, const std::string & paramName, const ParamValDelta & shift )
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
        bool valueSet = false;

        for( auto & k : p->AccessInterpolator().GetKeys() )
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
            if( it.first.first == "transform" )
            {
                if( it.first.second == "translation" )
                {
                    ApplyTranslationDelta( it.second, next );
                }

                if( it.first.second == "scale" )
                {
                    ApplyScaleDelta( it.second, next );
                }

                if( it.first.second == "rotation" )
                {
                    ApplyRotationDelta( it.second, next );
                }
            }

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
ShiftReplicationModifierPtr             ShiftReplicationModifier::Create()
{
    return ShiftReplicationModifierPtr( new ShiftReplicationModifier() );
}

// *******************************
//
void                                    ShiftReplicationModifier::ApplyTranslationDelta ( const ParamValDelta & delta, const BasicNodePtr & node ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM_VEC )
    {
        auto transformParamTyped = QueryTypedParam< ParamTransformVecPtr >( transformParam );

        // X
        bool valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 1 ]->GetP0MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.x;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 1 ]->GetP0MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.x );
        }

        // Y
        valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 1 ]->GetP1MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.y;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 1 ]->GetP1MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.y );
        }

        // Z
        valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 1 ]->GetP2MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.z;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 1 ]->GetP2MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.z );
        }
    }
}

// *******************************
//
void                                    ShiftReplicationModifier::ApplyScaleDelta       ( const ParamValDelta & delta, const BasicNodePtr & node ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM_VEC )
    {
        auto transformParamTyped = QueryTypedParam< ParamTransformVecPtr >( transformParam );

        // X
        bool valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 3 ]->GetP0MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.x;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 3 ]->GetP0MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.x );
        }

        // Y
        valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 3 ]->GetP1MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.y;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 3 ]->GetP1MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.y );
        }

        // Z
        valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 3 ]->GetP2MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.z;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 3 ]->GetP2MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.z );
        }
    }

}

// *******************************
//
void                                    ShiftReplicationModifier::ApplyRotationDelta    ( const ParamValDelta & delta, const BasicNodePtr & node ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM_VEC )
    {
        auto transformParamTyped = QueryTypedParam< ParamTransformVecPtr >( transformParam );

        // X
        bool valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 2 ]->GetP0MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.x;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 2 ]->GetP0MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.x );
        }

        // Y
        valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 2 ]->GetP1MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.y;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 2 ]->GetP1MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.y );
        }

        // Z
        valueSet = false;

        for( auto & k : transformParamTyped->Transform( 0 )[ 2 ]->GetP2MotylaNoga().GetKeys() )
        {
            if( k.t == delta.startTime )
            {
                k.t += delta.deltaTime;

                auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

                k.val += dv.z;
                valueSet = true;
            }
        }
       
        if( !valueSet )
        {
            auto interpolator = transformParamTyped->Transform( 0 )[ 2 ]->GetP2MotylaNoga();

            auto val = interpolator.Evaluate( delta.startTime );

            auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();

            interpolator.AddKey( delta.startTime + delta.deltaTime,  val + dv.z );
        }
    }
}

} // model
} // bv