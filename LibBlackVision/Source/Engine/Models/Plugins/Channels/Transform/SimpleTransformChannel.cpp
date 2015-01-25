#include "SimpleTransformChannel.h"


namespace bv { namespace model
{

// ******************************
//
SimpleTransformChannel::SimpleTransformChannel  ( const ParamTransformVec & paramTransformVec )
    : m_transformParams( paramTransformVec )
{
    for( unsigned int i = 0; i < paramTransformVec.NumTransforms(); ++i )
    {
        m_transformations.push_back( TransformPtr( new Transform() ) );
    }

    assert( paramTransformVec.NumTransforms() == m_transformations.size() );
}

// ******************************
//
void                                    SimpleTransformChannel::Update( TimeType t )
{
    assert( m_transformParams.NumTransforms() == m_transformations.size() );

    for( unsigned int i = 0; i < m_transformations.size(); ++i )
    {
        m_transformations[ i ]->SetMatrix( m_transformParams.Evaluate( i ) );
    }
}

// ******************************
//
ParamTransformVec *                     SimpleTransformChannel::GetParamTransformVec    ()
{
    return &m_transformParams;
}

} // model
} // bv
