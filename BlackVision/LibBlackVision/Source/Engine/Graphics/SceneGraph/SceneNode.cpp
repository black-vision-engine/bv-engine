#include "stdafx.h"

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
    , m_boundingBox( nullptr )
    , m_drawBoundingBox( false )
    , m_boundingBoxColor( glm::vec4( 1, 1, 1, 1 ) )
    , m_audio( nullptr )
{
    m_nodeEffect = CreateNodeEffect( NodeEffectType::NET_DEFAULT );
}

// ********************************
//
SceneNode::~SceneNode          ()
{
    DeleteTransformable();
    
    DeleteAudio();

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
void                    SceneNode::AddChildNode         ( SceneNode * child, UInt32 idx )
{
    if( idx < m_sceneNodes.size() )
    {
        m_sceneNodes.insert( m_sceneNodes.begin() + idx, child );
    }
    else
    {
        m_sceneNodes.push_back( child );
    }
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
AudioEntity *           SceneNode::GetAudio             ()
{
    return m_audio;
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
void            SceneNode::SetAudio             ( AudioEntity * audio )
{
    DeleteAudio();
    m_audio = audio;
}

// ********************************
//
void            SceneNode::DeleteAudio          ()
{
    delete m_audio;
    m_audio = nullptr;
}

// ********************************
//
void            SceneNode::Update               ( const Transform & parentTransform )
{
    m_transformable->UpdateTransform( parentTransform );

    auto worldTransform = m_transformable->WorldTransform();

    for ( auto node : m_sceneNodes )
    {
        node->Update( worldTransform );
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

// ********************************
//
void                    SceneNode::SetBoundingBox   ( const math::Box * bb )
{
    m_boundingBox = bb;
}

// ********************************
//
const math::Box *       SceneNode::GetBoundingBox   () const
{
    return m_boundingBox;
}

// ***********************
//
bool                    SceneNode::IsSelected          () const
{
    return m_drawBoundingBox;
}

// ***********************
//
glm::vec4               SceneNode::GetBoundingBoxColor () const
{
    return m_boundingBoxColor;
}

// ***********************
//
void               SceneNode::Select              ( glm::vec4 color )
{
    m_drawBoundingBox = true;
    m_boundingBoxColor = color;
}

// ***********************
//
void                    SceneNode::Unselect             ()
{
    m_drawBoundingBox = false;
}

} //bv
