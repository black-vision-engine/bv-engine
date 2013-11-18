#include "SimpleTransformChannel.h"


namespace bv { namespace model
{

// ******************************
//
void                                    SimpleTransformChannel::Update( TimeType t )
{
    assert( m_transformParams.size() == m_transformations.size() );
    for( unsigned int i = 0; i < m_transformParams.size(); ++i )
    {
        m_transformations[ i ]->SetMatrix( m_transformParams[ i ].Evaluate( t ) );
    }
}

// ******************************
//
void                                    SimpleTransformChannel::AddTransform( const ParamTransform&  transInt )
{
    m_transformParams.push_back( transInt );
    m_transformations.push_back( new Transform() );
}

} // model
} // bv
