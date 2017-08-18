#include "stdafx.h"

#include "SceneNode.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


#include "Memory/MemoryLeaks.h"


namespace bv {

// ********************************
//
SceneNode::SceneNode           ( TransformableEntity * transformable )
    : m_repr( new SceneNodeRepr( transformable, this ) )
    , m_nodeEffectEnabled( true ) // FIXME: enabled by default - is this right?
{}

// ********************************
//
SceneNode::~SceneNode          ()
{
    delete m_repr;
}

// ********************************
//
SceneNodeRepr *         SceneNode::GetRepr              ()
{
    return m_repr;
}

// ********************************
//
SizeType                SceneNode::NumChildNodes        () const
{
    return m_repr->NumChildNodes();
}

// ********************************
//
void                    SceneNode::AddChildNode         ( SceneNode * child )
{
    m_repr->AddChildNode( child );
}

// ********************************
//
void                    SceneNode::AddChildNode         ( SceneNode * child, UInt32 idx )
{
    m_repr->AddChildNode( child, idx );
}

// ********************************
//
void                    SceneNode::DetachChildNode      ( SceneNode * node )
{
    m_repr->DetachChildNode( node );
}

// ********************************
//
SceneNode *             SceneNode::DetachChildNode      ( unsigned int idx )
{
    return m_repr->DetachChildNode( idx );
}

// ********************************
//
SceneNode *             SceneNode::GetChild             ( unsigned int idx )
{
    return m_repr->GetChild( idx );
}

// ********************************
//
bool                    SceneNode::HasChild            ( SceneNode * node ) const
{
    return m_repr->HasChild( node );
}

// ********************************
//
TransformableEntity *   SceneNode::GetTransformable     ()
{
    return m_repr->GetTransformable();
}

// ********************************
//
audio::AudioEntity *    SceneNode::GetAudio             () const
{
    return m_repr->GetAudio();
}

// ********************************
//
void            SceneNode::SetAudio             ( audio::AudioEntity * audio )
{
    m_repr->SetAudio( audio );
}

// ********************************
//
bool            SceneNode::IsNodeEffectEnabled () const
{
    return m_nodeEffectEnabled;
}

// ********************************
//
void            SceneNode::EnableNodeEffect     ()
{
    m_nodeEffectEnabled = true;
}

// ********************************
//
void            SceneNode::DisableNodeEffect    ()
{
    m_nodeEffectEnabled = false;
}

// ********************************
//
NodeEffectPtr   SceneNode::GetNodeEffect        ()
{
    return m_nodeEffect;
}

// ********************************
//
void                    SceneNode::SetNodeEffect   ( NodeEffectPtr nodeEffect )
{
    m_nodeEffect = nodeEffect;
}

// ********************************
//
void            SceneNode::SetTransformable         ( TransformableEntity * transformable )
{
    m_repr->SetTransformable( transformable );
}

// ********************************
//
void            SceneNode::DeleteTransformable      ()
{
    m_repr->DeleteTransformable();
}

// ********************************
//
void            SceneNode::Update                   ( const Transform & parentTransform )
{
    m_repr->Update( parentTransform );

    auto numGizmos = GetNumGizmos();
    if( numGizmos > 0 )
    {
        // We treat gizmos like they were child of this node.
        const auto & worldTransform = m_repr->GetTransformable()->WorldTransform();

        for( UInt32 i = 0; i < numGizmos; ++i )
            GetGizmo( i )->Update( worldTransform );
    }
}

// ********************************
//
bool                    SceneNode::IsVisible        () const
{
    return m_visible;
}

// ********************************
//
void                    SceneNode::SetVisible       ( bool visible )
{
    m_visible = visible;
}

// ********************************
//
void                    SceneNode::SetBoundingBox   ( const math::Box * bb )
{
    m_repr->SetBoundingBox( bb );
}

// ********************************
//
const math::Box *       SceneNode::GetBoundingBox   () const
{
    return m_repr->GetBoundingBox();
}

// ***********************
//
SceneNodePerformance *  SceneNode::GetPerformanceData  ()
{
    return m_repr->GetPerformanceData();
}

// ********************************
//
RenderableEntity * renderable( SceneNode * node )
{
    return renderable( node->GetRepr() );
}


// ========================================================================= //
// Gizmo
// ========================================================================= //

// ***********************
//
void                SceneNode::AddGizmo ( SceneNode * gizmoRoot, UInt32 idx )
{
    if( gizmoRoot )
    {
        auto gizmoContainer = AllocateGizmos();
        gizmoContainer->AddGizmo( gizmoRoot, idx );
    }
}

// ***********************
//
void                SceneNode::RemoveGizmo  ( UInt32 idx )
{
    if( m_gizmos )
    {
        auto gizmoContainer = AllocateGizmos();
        gizmoContainer->RemoveGizmo( idx );

        DeallocateGizmos();
    }
}

// ***********************
//
void                SceneNode::RemoveGizmo  ( SceneNode * gizmoRoot )
{
    if( m_gizmos )
    {
        auto gizmoContainer = AllocateGizmos();
        gizmoContainer->RemoveGizmo( gizmoRoot );

        DeallocateGizmos();
    }
}

// ***********************
//
SceneNode *        SceneNode::GetGizmo             ( UInt32 idx ) const
{
    if( m_gizmos )
        return m_gizmos->GetGizmo( idx );
    return nullptr;
}

// ***********************
//
UInt32              SceneNode::GetNumGizmos         () const
{
    if( m_gizmos )
        return m_gizmos->GetNumGizmos();
    return 0;
}

// ***********************
//
EngineGizmoContainer *      SceneNode::AllocateGizmos       ()
{
    if( !m_gizmos )
        m_gizmos = std::make_unique< EngineGizmoContainer >();

    return m_gizmos.get();
}

// ***********************
// Releases gizmo container if it's empty.
void                SceneNode::DeallocateGizmos     ()
{
    if( m_gizmos->GetNumGizmos() == 0 )
        m_gizmos.release();
}




} //bv
