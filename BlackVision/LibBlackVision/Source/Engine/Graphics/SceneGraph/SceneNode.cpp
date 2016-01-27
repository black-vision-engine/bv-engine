#include "SceneNode.h"

#include <cassert>

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/NodeEffect/NodeEffectFactory.h"


namespace bv {

// ********************************
//
SceneNode::SceneNode           ( TransformableEntity * transformable )
    : m_transformable( transformable )
    , m_nodeEffect( nullptr )
{
    m_nodeEffect = CreateNodeEffect( NodeEffectType::NET_DEFAULT );
}

// ********************************
//
SceneNode::~SceneNode          ()
{
    DeleteTransformable();

    for ( auto node : m_sceneNodes )
    {
        delete node;
    }
}

// ********************************
//
SizeType                SceneNode::NumChildNodes        () const
{
    return m_sceneNodes.size();
}

// ********************************
//
void                    SceneNode::AddChildNode         ( SceneNode * child )
{
    m_sceneNodes.push_back( child );
}

// ********************************
//
void                    SceneNode::DetachChildNode      ( SceneNode * node )
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
SceneNode *             SceneNode::DetachChildNode      ( unsigned int idx )
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
SceneNode *             SceneNode::GetChild             ( unsigned int idx )
{
    assert( idx < (unsigned int) NumChildNodes() );

    return m_sceneNodes[ idx ];
}

// ********************************
//
bool                    SceneNode::HasChild            ( SceneNode * node ) const
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
TransformableEntity *   SceneNode::GetTransformable     ()
{
    return m_transformable;
}

// ********************************
//
NodeEffectPtr   SceneNode::GetNodeEffect  ()
{
    return m_nodeEffect;
}

// ********************************
//
void            SceneNode::SetNodeEffect  ( NodeEffectPtr nodeEffect )
{
    m_nodeEffect = nodeEffect;
}

// ********************************
//
void            SceneNode::SetTransformable     ( TransformableEntity * transformable )
{
    DeleteTransformable();

    m_transformable = transformable;
}

// ********************************
//
void            SceneNode::DeleteTransformable  ()
{
    delete m_transformable;

    m_transformable = nullptr;
}

// ********************************
//
void            SceneNode::Update               ( const std::vector< Transform > & parentTransforms )
{
    m_transformable->UpdateTransforms( parentTransforms );

    const std::vector< Transform > & worldTransforms = m_transformable->WorldTransforms();

    for ( auto node : m_sceneNodes )
    {
        node->Update( worldTransforms );
    }
}

// ********************************
//
bool                    SceneNode::IsVisible    () const
{
    return m_visible;
}

// ********************************
//
void                    SceneNode::SetVisible   ( bool visible )
{
    m_visible = visible;
}

} //bv
