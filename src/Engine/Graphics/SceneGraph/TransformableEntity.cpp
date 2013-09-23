#include "TransformableEntity.h"
#include "TransformUpdater.h"

#include <cassert>

namespace bv {

// *********************************
//
TransformableEntity::TransformableEntity                            ( TransformUpdater * locTransformUpdater )
{
    RegisterTransformUpdater( locTransformUpdater );
}

// *********************************
//
TransformableEntity::~TransformableEntity                           ()
{
}

// *********************************
//
void                TransformableEntity::RegisterTransformUpdater    ( TransformUpdater * locTransformUpdater )
{
    m_locTransformUpdater = locTransformUpdater;
}

// *********************************
//
const Transform &   TransformableEntity::LocalTransform                 () const
{
    return m_localTransform;
}

// *********************************
//
void                TransformableEntity::SetLocalTransform              ( const Transform & t )
{
    m_localTransform = t;
}

// *********************************
//
const std::vector< Transform > &  TransformableEntity::WorldTransforms  () const
{
    return m_worldTransforms;
}

    
// *********************************
//
void  TransformableEntity::SetWorldTransforms                           ( const std::vector< Transform > & transforms )
{
    m_worldTransforms = transforms;
}

// *********************************
//
void   TransformableEntity::UpdateTransforms                            ( double t, const std::vector< Transform > & transforms )
{
    UpdateSetWorldTransform( transforms );
}

// *********************************
//
void                TransformableEntity::UpdateSetWorldTransform        ( const std::vector< Transform > & parentTransforms )
{
    assert( parentTransforms.size() == m_worldTransforms.size() );

    for( int i = 0; i < (int) m_worldTransforms.size(); ++i )
    {
        m_worldTransforms[ i ] = parentTransforms[ i ] * m_localTransform;
    }
}

}
