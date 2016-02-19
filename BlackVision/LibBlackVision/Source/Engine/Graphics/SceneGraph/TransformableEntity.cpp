#include "stdafx.h"

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
void                TransformableEntity::UpdateTransform                ( const Transform & transform )
{
    UpdateSetWorldTransform( transform );
}

// *********************************
//
void                TransformableEntity::UpdateSetWorldTransform        ( const Transform & parentTransform )
{
    m_worldTransform = parentTransform * m_localTransform;
}

} //bv
