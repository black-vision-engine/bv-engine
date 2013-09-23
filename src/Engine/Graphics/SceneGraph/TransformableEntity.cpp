#include "TransformableEntity.h"
#include "TransformUpdater.h"

#include <cassert>

namespace bv {

//FIXME: multiple different transformations should be passed here for instancing purposes (one local transform and vector of world transforms)
//FIXME: and updaters should be tailored accordingly

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
