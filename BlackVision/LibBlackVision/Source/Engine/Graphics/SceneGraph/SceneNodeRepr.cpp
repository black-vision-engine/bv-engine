#include "stdafx.h"

#include "SceneNodeRepr.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv {

// ********************************
//
SceneNodeRepr::SceneNodeRepr            ( TransformableEntity * transformable )
    : m_transformable( transformable )
{
}

// ********************************
//
SceneNodeRepr::~SceneNodeRepr           ()
{
    DeleteTransformable();

    for ( auto node : m_sceneNodes )
    {
        delete node;
    }
}

// ********************************
//
SizeType                SceneNodeRepr::NumChildNodes        () const
{
    return m_sceneNodes.size();
}

// ********************************
//
void                    SceneNodeRepr::AddChildNode         ( SceneNode * child )
{
    m_sceneNodes.push_back( child );
}

// ********************************
//
void                    SceneNodeRepr::DetachChildNode      ( SceneNode * node )
{
    for( auto it = m_sceneNodes.begin(); it != m_sceneNodes.end(); ++it )
    {
        if( *it == node )
        {
            m_sceneNodes.erase( it );

            return;
        }
    }

    assert( false );
}

// ********************************
//
SceneNode *             SceneNodeRepr::DetachChildNode      ( unsigned int idx )
{
    SceneNode * node = nullptr;

    if( idx < m_sceneNodes.size() )
    {
        node = m_sceneNodes[ idx ];
        m_sceneNodes.erase( m_sceneNodes.begin() + idx );
    }

    return node;
}

// ********************************
//
SceneNode *             SceneNodeRepr::GetChild             ( unsigned int idx )
{
    assert( idx < (unsigned int) NumChildNodes() );

    return m_sceneNodes[ idx ];
}

// ********************************
//
bool                    SceneNodeRepr::HasChild            ( SceneNode * node ) const
{
    for( auto child : m_sceneNodes )
    {
        if ( child == node )
        {
            return true;
        }
    }

    return false;
}

// ********************************
//
TransformableEntity *   SceneNodeRepr::GetTransformable     ()
{
    return m_transformable;
}

// ********************************
//
void            SceneNodeRepr::SetTransformable     ( TransformableEntity * transformable )
{
    DeleteTransformable();

    m_transformable = transformable;
}

// ********************************
//
void            SceneNodeRepr::DeleteTransformable  ()
{
    delete m_transformable;

    m_transformable = nullptr;
}

// ********************************
//
void            SceneNodeRepr::Update               ( const Transform & parentTransform )
{
    m_transformable->UpdateTransform( parentTransform );

    const auto & worldTransform = m_transformable->WorldTransform();

    for ( auto node : m_sceneNodes )
    {
        node->Update( worldTransform );
    }
}

// ********************************
//
RenderableEntity * renderable( SceneNodeRepr * nodeRepr )
{
    return static_cast< RenderableEntity * >( nodeRepr->GetTransformable() );
}

} //bv
