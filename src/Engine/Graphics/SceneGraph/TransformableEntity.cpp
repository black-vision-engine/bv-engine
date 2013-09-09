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
const Transform &   TransformableEntity::LocalTransform             () const
{
    return m_localTransform;
}

// *********************************
//
void                TransformableEntity::SetLocalTransform          ( const Transform & t )
{
    m_localTransform = t;
}

// *********************************
//
const Transform &   TransformableEntity::WorldTransform             () const
{
    return m_worldTransorm;
}

// *********************************
//
void                TransformableEntity::SetWorldTransform          ( const Transform & t )
{
    m_worldTransorm = t;
}

// *********************************
//
void                TransformableEntity::UpdateTransforms           ( double t, const Transform & parentTransform )
{
    UpdateSetWorldTransform( parentTransform );
}

// *********************************
//
void                TransformableEntity::UpdateSetWorldTransform     ( const Transform & parentTransform )
{
    m_worldTransorm = parentTransform * m_localTransform;
}

}
