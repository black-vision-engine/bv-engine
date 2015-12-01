#include "ShiftReplicationModifier.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

namespace bv { namespace model
{

// *******************************
//
ShiftReplicationModifier::ShiftReplicationModifier( const glm::vec3 & delta )
    : m_delta( delta )
{}

// *******************************
//
void                            ShiftReplicationModifier::Apply( const BasicNodeConstPtr &, const BasicNodePtr & next ) const
{
    auto transformParam = next->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    if( transformParam->GetType() == ModelParamType::MPT_TRANSFORM_VEC )
    {
        auto typedTransformParam = QueryTypedParam< ParamTransformVecPtr >( transformParam );

        TransformF trans;
        FloatInterpolator fx;
        fx.AddKey( 0.f, m_delta.x );
        
        FloatInterpolator fy;
        fy.AddKey( 0.f, m_delta.y );

        FloatInterpolator fz;
        fz.AddKey( 0.f, m_delta.z );

        trans.AddTranslation( fx, fy, fz );

        typedTransformParam->Transform( 0 ).AddTranslation( fx, fy, fz );
    }
}

// *******************************
//
ShiftReplicationModifierConstPtr        ShiftReplicationModifier::Create( const glm::vec3 & delta )
{
    return ShiftReplicationModifierConstPtr( new ShiftReplicationModifier( delta ) );
}

} // model
} // bv