#include "SimpleTransformChannel.h"

namespace bv { namespace model
{

void                                    SimpleTransformChannel::Update( float t )
{
    assert( m_transformInterpolators.size() == m_transformations.size() );
    for( unsigned int i = 0; i < m_transformInterpolators.size(); ++i )
    {
        m_transformations[ i ]->SetMatrix( m_transformInterpolators[ i ]->evaluate( t ) );
    }
}

void                                    SimpleTransformChannel::AddTransform( TransformF * transInt )
{
    m_transformInterpolators.push_back( transInt );
    m_transformations.push_back( new Transform() );
}


} // model
} // bv