#include "ShiftReplicationModifier.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "../NodeLogicHelper.h"

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
    ser.EnterChild( "replicatorModifier" );
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

    if( !deser.EnterChild( "paramShifts" ) )
        return nullptr;

    if( deser.EnterChild( "paramShift" ) )
    {
        do
        {
            model::ParamValDelta shift;
            std::string pluginName = deser.GetAttribute( "pluginName" );
            std::string paramName = deser.GetAttribute( "paramName" );

            deser.EnterChild( "paramDelta" );
                shift.deltaTime = SerializationHelper::String2T( deser.GetAttribute( "deltaTime" ), 0.0f );
                shift.startTime = SerializationHelper::String2T( deser.GetAttribute( "startTime" ), 0.0f );

                deser.EnterChild( "delta" );

                    shift.delta = SerializationHelper::CreateValue( deser, "delta" );

                deser.ExitChild();  // value 

            deser.ExitChild();  // paramDelta

        } while( deser.NextChild() );
        deser.ExitChild();  // paramShift
    }

    deser.ExitChild();  // paramShifts

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

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM )
    {
        auto transformParamTyped = QueryTypedParam< ParamTransformPtr >( transformParam );

        auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();
        auto val = transformParamTyped->GetTransform().GetTranslation( delta.startTime );

        //FIXME: it might not work as original (previous) code
        transformParamTyped->SetTranslation( val + dv, delta.startTime + delta.deltaTime );
    }
}

// *******************************
//
void                                    ShiftReplicationModifier::ApplyScaleDelta       ( const ParamValDelta & delta, const BasicNodePtr & node ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM )
    {
        auto transformParamTyped = QueryTypedParam< ParamTransformPtr >( transformParam );

        auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();
        auto val = transformParamTyped->GetTransform().GetScale( delta.startTime );

        //FIXME: it might not work as original (previous) code
        transformParamTyped->SetScale( val + dv, delta.startTime + delta.deltaTime );
    }

}

// *******************************
//
void                                    ShiftReplicationModifier::ApplyRotationDelta    ( const ParamValDelta & delta, const BasicNodePtr & node ) const
{
    auto transformParam = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM )
    {
        auto transformParamTyped = QueryTypedParam< ParamTransformPtr >( transformParam );

        auto dv = QueryTypedValue< ValueVec3Ptr >( delta.delta )->GetValue();
        auto valAxis = transformParamTyped->GetTransform().GetRotationAxis( delta.startTime );
        auto valAngle = transformParamTyped->GetTransform().GetRotationAngle( delta.startTime );

        //FIXME: it might not work as original (previous) code
        transformParamTyped->SetRotation( valAxis + dv, valAngle, delta.startTime + delta.deltaTime );
    }
}

} // model
} // bv