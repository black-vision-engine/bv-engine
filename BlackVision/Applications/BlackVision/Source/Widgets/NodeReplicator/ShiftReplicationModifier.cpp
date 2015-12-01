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
        trans.AddTranslation( m_delta.x, m_delta.y, m_delta.z );

        typedTransformParam->AppendTransform( trans );
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