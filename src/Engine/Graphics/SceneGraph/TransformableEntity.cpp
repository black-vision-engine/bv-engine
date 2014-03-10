#include "TransformableEntity.h"


namespace bv {

// *********************************
//
TransformableEntity::TransformableEntity                                ()
{
}

// *********************************
//
TransformableEntity::~TransformableEntity                               ()
{
}

// *********************************
//
void   TransformableEntity::UpdateTransforms                            ( const std::vector< Transform > & transforms )
{
    UpdateSetWorldTransform( transforms );
}

// *********************************
//
void                TransformableEntity::UpdateSetWorldTransform        ( const std::vector< Transform > & parentTransforms )
{
    assert( parentTransforms.size() == m_worldTransforms.size() );

    for( unsigned int i = 0; i < m_worldTransforms.size(); ++i )
    {
        m_worldTransforms[ i ] = parentTransforms[ i ] * m_localTransform;
    }
}

} //bv
