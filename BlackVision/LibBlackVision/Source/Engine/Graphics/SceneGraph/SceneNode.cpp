#include "stdafx.h"

#include "SceneNode.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Events/Events.h"

#include "Memory/MemoryLeaks.h"


namespace bv {

// ********************************
//
SceneNode::SceneNode           ( TransformableEntity * transformable )
    : m_repr( new SceneNodeRepr( transformable, this ) )
    , m_drawBoundingBox( false )
    , m_NNodeEffectEnabled( true ) // FIXME: enabled by default - is this right?
    , m_boundingBoxColor( glm::vec4( 1, 1, 1, 1 ) )
{
}

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
bool            SceneNode::IsNNodeEffectEnabled () const
{
    return m_NNodeEffectEnabled;
}

// ********************************
//
void            SceneNode::EnableNNodeEffect    ()
{
    m_NNodeEffectEnabled = true;
}

// ********************************
//
void            SceneNode::DisableNNodeEffect   ()
{
    m_NNodeEffectEnabled = false;
}

// ********************************
//
nrl::NodeEffectPtr     SceneNode::GetNNodeEffect   ()
{
    return m_nNodeEffect;
}

// ********************************
//
void                    SceneNode::SetNNodeEffect   ( nrl::NodeEffectPtr nNodeEffect )
{
    m_nNodeEffect = nNodeEffect;
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
bool                    SceneNode::IsSelected          () const
{
    return m_drawBoundingBox;
}

// ***********************
//
const glm::vec4 &       SceneNode::GetBoundingBoxColor () const
{
    return m_boundingBoxColor;
}

// ***********************
//
void                    SceneNode::Select              ( glm::vec4 color )
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

} //bv
