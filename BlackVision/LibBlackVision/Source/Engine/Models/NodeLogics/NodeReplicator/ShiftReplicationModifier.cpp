#include "stdafx.h"

#include "ShiftReplicationModifier.h"

#include "Engine/Models/NodeLogics/NodeLogicHelper.h"


namespace bv { namespace nodelogic {

// *******************************
//
ShiftReplicationModifier::ShiftReplicationModifier()
{}

// ***********************
//
void                            ShiftReplicationModifier::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "replicatorModifier" );
        ser.SetAttribute( "type", "shiftReplicationModifier" );

        ser.EnterArray( "paramShifts" );
        for( auto& shift : m_paramsShifts )
        {
            ser.EnterChild( "paramShift" );
                ser.SetAttribute( "pluginName", shift.first.first.first );
                ser.SetAttribute( "paramName", shift.first.first.second );

                ser.EnterChild( "paramDelta" );
                    ser.SetAttribute( "startTime", Convert::T2String( shift.second.startTime ) );
                    ser.SetAttribute( "deltaTime", Convert::T2String( shift.second.deltaTime ) );

                    ser.EnterChild( "delta" );

                        SerializationHelper::SerializeValue( ser, shift.second.delta );

                    ser.ExitChild();    // delta

                ser.ExitChild();

            ser.ExitChild();
        }
        ser.ExitChild();


    ser.ExitChild();
}

// ***********************
//
ShiftReplicationModifierPtr     ShiftReplicationModifier::Create          ( const IDeserializer& deser )
{
    auto shiftModifier = ShiftReplicationModifier::Create();
    AddParamShift( deser, shiftModifier );

    return shiftModifier;
}

// *******************************
//
void                            ShiftReplicationModifier::AddParamShift ( const std::string & pluginName, const std::string & paramName, const ParamValDelta & shift )
{
    auto p = std::make_pair( pluginName, paramName );
    auto mapKey = std::make_pair( p, shift.startTime );     // Parameter key time is part of map key. We can move multiple keys for one parameter.
    m_paramsShifts[ mapKey ] = shift;
}

// ***********************
//
bool                            ShiftReplicationModifier::AddParamShift ( const IDeserializer & deser, ShiftReplicationModifierPtr & shiftModifier )
{
    if( !deser.EnterChild( "paramShifts" ) )
        return false;

    if( deser.EnterChild( "paramShift" ) )
    {
        do
        {
            ParamValDelta shift;
            std::string pluginName = deser.GetAttribute( "pluginName" );
            std::string paramName = deser.GetAttribute( "paramName" );

            deser.EnterChild( "paramDelta" );
            shift.deltaTime = Convert::String2T( deser.GetAttribute( "deltaTime" ), 0.0f );
            shift.startTime = Convert::String2T( deser.GetAttribute( "startTime" ), 0.0f );

            deser.EnterChild( "delta" );

            shift.delta = SerializationHelper::CreateValue( deser, "delta" );

            deser.ExitChild();  // value 

            deser.ExitChild();  // paramDelta

            shiftModifier->AddParamShift( pluginName, paramName, shift );

        } while( deser.NextChild() );
        deser.ExitChild();  // paramShift

        deser.ExitChild();  // paramShifts
        return true;
    }
    return false;
}

// ***********************
//
bool            ShiftReplicationModifier::RemoveParamShift  ( const std::string & pluginName, const std::string & paramName, TimeType startTime )
{
    auto p = std::make_pair( pluginName, paramName );
    auto mapKey = std::make_pair( p, startTime );       // Parameter key time is part of map key. We can move multiple keys for one parameter.

    m_paramsShifts.erase( mapKey );

    return false;
}

// ***********************
//
void            ShiftReplicationModifier::ClearAllShifts    ()
{
    m_paramsShifts.clear();
}


// *******************************
//
namespace
{

template< class ParamTypePtr, ParamType paramType >
void ApplyParamDelta( model::IParameterPtr param, ParamValDelta delta, int repCounter )
{
    typedef std::shared_ptr< ValueImpl< ParamTypePtr::element_type::ValType, paramType > > ValType;

    if( auto p = model::QueryTypedParam< ParamTypePtr >( param ) )
    {
        TimeType startTime = delta.startTime + repCounter * delta.deltaTime;

        auto value = p->AccessInterpolator().Evaluate( startTime );
        auto deltaValue = QueryTypedValue< ValType >( delta.delta )->GetValue();

        p->SetVal( value + deltaValue, startTime );
        
        if( delta.deltaTime > 0 )
        {
            p->MoveKey( startTime, startTime + delta.deltaTime );
        }
    }
}

} // anonymous

// *******************************
//
void                            ShiftReplicationModifier::Apply( const model::BasicNodeConstPtr &, const model::BasicNodePtr & next, BVProjectEditor * /* editor */, int repCounter ) const
{
    for( auto it : m_paramsShifts )
    {
        if( auto p = next->GetPlugin( it.first.first.first ) )
        {
            if( it.first.first.first == "transform" )
            {
                if( it.first.first.second == "translation" )
                {
                    ApplyTranslationDelta( it.second, next, repCounter );
                }

                if( it.first.first.second == "scale" )
                {
                    ApplyScaleDelta( it.second, next, repCounter );
                }

                if( it.first.first.second == "rotation" )
                {
                    ApplyRotationDelta( it.second, next, repCounter );
                }
            }

            if( auto param = p->GetParameter( it.first.first.second ) )
            {
                switch( it.second.delta->GetType() )
                {
                case ParamType::PT_FLOAT1:
                    ApplyParamDelta< model::ParamFloatPtr, ParamType::PT_FLOAT1 >( param, it.second, repCounter );
                    break;
                case ParamType::PT_FLOAT2:
                    ApplyParamDelta< model::ParamVec2Ptr, ParamType::PT_FLOAT2 >( param, it.second, repCounter );
                    break;
                case ParamType::PT_FLOAT3:
                    ApplyParamDelta< model::ParamVec3Ptr, ParamType::PT_FLOAT3 >( param, it.second, repCounter );
                    break;
                case ParamType::PT_FLOAT4:
                    ApplyParamDelta< model::ParamVec4Ptr, ParamType::PT_FLOAT4 >( param, it.second, repCounter );
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
                    ApplyParamDelta< model::ParamIntPtr, ParamType::PT_INT >( param, it.second, repCounter );
                    break;
                case ParamType::PT_BOOL:
                    //ApplyParamDelta< ParamBoolPtr, ParamType::PT_BOOL >( param, it.second );
                    break;
                case ParamType::PT_ENUM:
                    //ApplyParamDelta< ParamMat2Ptr, ParamType::PT_MAT2 >( param, it.second );
                    break;
                case ParamType::PT_STRING:
                    ApplyParamDelta< model::ParamStringPtr, ParamType::PT_STRING >( param, it.second, repCounter );
                    break;
                case ParamType::PT_WSTRING:
                    ApplyParamDelta< model::ParamWStringPtr, ParamType::PT_WSTRING >( param, it.second, repCounter );
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
void                                    ShiftReplicationModifier::ApplyTranslationDelta ( const ParamValDelta & delta, const model::BasicNodePtr & node, int repCounter ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM )
    {
        TimeType startTime = delta.startTime + repCounter * delta.deltaTime;

        auto transformParamTyped = model::QueryTypedParam< model::ParamTransformPtr >( transformParam );

        auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();
        auto val = transformParamTyped->GetTransform().GetTranslation( startTime );

        //FIXME: it might not work as original (previous) code
        //transformParamTyped->SetTranslation( val + dv, delta.startTime + delta.deltaTime );
        transformParamTyped->SetTranslation( val + dv, startTime );
        
        if( delta.deltaTime > 0.0f )
        {
            transformParamTyped->MoveTranslation( startTime, startTime + delta.deltaTime );
        }
    }
}

// *******************************
//
void                                    ShiftReplicationModifier::ApplyScaleDelta       ( const ParamValDelta & delta, const model::BasicNodePtr & node, int repCounter ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM )
    {
        TimeType startTime = delta.startTime + repCounter * delta.deltaTime;

        auto transformParamTyped = model::QueryTypedParam< model::ParamTransformPtr >( transformParam );

        auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();
        auto val = transformParamTyped->GetTransform().GetScale( startTime );

        //FIXME: it might not work as original (previous) code
        transformParamTyped->SetScale( val + dv, startTime );
        
        if( delta.deltaTime > 0.0f )
        {
            transformParamTyped->MoveScale( startTime, startTime + delta.deltaTime );
        }
    }

}

// *******************************
//
void                                    ShiftReplicationModifier::ApplyRotationDelta    ( const ParamValDelta & delta, const model::BasicNodePtr & node, int repCounter ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM )
    {
        TimeType startTime = delta.startTime + repCounter * delta.deltaTime;

        auto transformParamTyped = model::QueryTypedParam< model::ParamTransformPtr >( transformParam );

        auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();
        auto valAngles = transformParamTyped->GetTransform().GetRotation( startTime );

        //FIXME: it might not work as original (previous) code
        transformParamTyped->SetRotation( valAngles, startTime );

        if( delta.deltaTime > 0.0f )
        {
            transformParamTyped->MoveRotation( startTime, startTime + delta.deltaTime );
        }
    }
}

} // nodelogic
} // bv