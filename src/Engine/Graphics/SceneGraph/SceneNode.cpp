#include "SceneNode.h"

#include <cassert>


namespace bv {

// ********************************
//
SceneNode::SceneNode           ( TransformableEntity * transformRep )
    : m_overridenState( false )
{
    RegisterTransformRep( transformRep );
}

// ********************************
//
SceneNode::~SceneNode          ()
{
    m_transformRep = nullptr; //this one is not owned here

    for ( auto transformable : m_transformables )
    {
        delete transformable;
    }

    for ( auto node : m_sceneNodes )
    {
        delete node;
    }
}

// ********************************
//
int                     SceneNode::NumChildrenNodes    () const
{
    return m_sceneNodes.size();
}

// ********************************
//
int                     SceneNode::NumTransformables   () const
{
    return m_transformables.size();
}

// ********************************
//
void                    SceneNode::AddChildNode        ( SceneNode * child )
{
    m_sceneNodes.push_back( child );
}

// ********************************
//
void                    SceneNode::AddTransformable    ( TransformableEntity * transformable )
{
    m_transformables.push_back( transformable );
}

// ********************************
//
SceneNode *             SceneNode::GetChild            ( int idx )
{
    assert( idx >= 0 );
    assert( idx < NumChildrenNodes() );

    return m_sceneNodes[ idx ];
}

// ********************************
//
TransformableEntity *   SceneNode::GetTransformable    ( int idx )
{
    assert( idx >= 0 );
    assert( idx < NumTransformables() );

    return m_transformables[ idx ];
}

// ********************************
//
TransformableEntity *   SceneNode::GetAnchor           ()
{
    return m_transformRep;
}


// ********************************
//
void            SceneNode::RegisterTransformRep( TransformableEntity * transformable )
{
    m_transformRep = transformable;
}

// ********************************
//
void            SceneNode::Update              ( const std::vector< Transform > & parentTransforms )
{
    const std::vector< Transform > * pWorldTransforms = &parentTransforms;

    if( m_transformRep )
    {
        m_transformRep->UpdateTransforms( *pWorldTransforms );

        pWorldTransforms = &m_transformRep->WorldTransforms();
    }

    const std::vector< Transform > & worldTransforms = *pWorldTransforms;

    for ( auto transformable : m_transformables )
    {
        transformable->UpdateTransforms( worldTransforms );
    }

    for ( auto node : m_sceneNodes )
    {
        node->Update( worldTransforms );
    }
}

// ********************************
//
bool                    SceneNode::IsVisible           () const
{
    return m_visible;
}

// ********************************
//
void                    SceneNode::SetVisible          ( bool visible )
{
    m_visible = visible;
}

// ********************************
//
bool                    SceneNode::IsOverriden         () const
{
    return m_overridenState;
}

// ********************************
//
void                    SceneNode::SetOverriden        ( bool overriden )
{
    m_overridenState = overriden;
}

} //bv
