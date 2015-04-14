#include "SceneNode.h"

#include <cassert>

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv {

// ********************************
//
SceneNode::SceneNode           ( TransformableEntity * transformable )
    : m_transformable( transformable )
    , m_overridenStateAlphaMask( false )
    , m_overridenStateNodeMask( false )
    , m_overrideAlphaVal( nullptr )
{
}

// ********************************
//
SceneNode::~SceneNode          ()
{
    delete m_transformable;

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
TransformableEntity *   SceneNode::GetTransformable     ()
{
    return m_transformable;
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

// ********************************
//
void                    SceneNode::DeleteNode   ( SceneNode * node, Renderer * renderer )
{
    while( node->NumChildNodes() > 0 )
    {
        DeleteNode( node->DetachChildNode( 0 ), renderer );
    }

    renderer->FreeAllPDResources( static_cast< RenderableEntity * >( node->GetTransformable() ) );

    delete node;
}

// ********************************
//
void            SceneNode::SetOverrideAlphaVal  ( const IValue * val )
{
    assert( val != nullptr );

    m_overrideAlphaVal = val;
}

// ********************************
//
const IValue * SceneNode::GetOverrideAlphaVal   () const
{
    return m_overrideAlphaVal;
}

// ********************************
//
bool                    SceneNode::IsOverridenAM() const
{
    return m_overridenStateAlphaMask;
}

// ********************************
//
bool                    SceneNode::IsOverridenNM() const
{
    return m_overridenStateNodeMask;
}

// ********************************
//
void                    SceneNode::SetOverridenAM( bool overriden )
{
    m_overridenStateAlphaMask = overriden;
}

// ********************************
//
void                    SceneNode::SetOverridenNM( bool overriden )
{
    m_overridenStateNodeMask = overriden;
}

} //bv
