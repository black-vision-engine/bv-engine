#include "stdafx.h"

#include "SceneNodeRepr.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv {

// ********************************
//
SceneNodeRepr::SceneNodeRepr            ( TransformableEntity * transformable, SceneNode * owner )
    : m_owner( owner )
    , m_transformable( transformable )
    , m_audio( nullptr )
    , m_boundingBox( nullptr )
{
    m_performanceData = new SceneNodePerformance();
}

// ********************************
//
SceneNodeRepr::~SceneNodeRepr           ()
{
    DeleteTransformable();
    DeleteAudio();

    for ( auto node : m_sceneNodes )
    {
        delete node;
    }

    delete m_performanceData;
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
void                    SceneNodeRepr::AddChildNode         ( SceneNode * child, UInt32 idx )
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
audio::AudioEntity *    SceneNodeRepr::GetAudio            () const
{
    return m_audio;
}

// ********************************
//
void                    SceneNodeRepr::SetAudio            ( audio::AudioEntity * audio )
{
    DeleteAudio();
    
    m_audio = audio;
}

// ********************************
//
void                    SceneNodeRepr::SetBoundingBox      ( const math::Box * bb )
{
    m_boundingBox = bb;
}

// ********************************
//
const math::Box *       SceneNodeRepr::GetBoundingBox      () const
{
    return m_boundingBox;
}

// ********************************
//
SceneNodePerformance *  SceneNodeRepr::GetPerformanceData  ()
{
    return m_performanceData;
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
void            SceneNodeRepr::DeleteAudio          ()
{
    if( m_audio )
    {
        // release allocated memory for this node audio entity
        auto evt = std::make_shared< AssetTrackerInternalEvent >( AssetTrackerInternalEvent::Command::ReleaseAudioResource );
        evt->SceneNodeOwner = m_owner;
        GetDefaultEventManager().TriggerEvent( evt );

        delete m_audio;
        m_audio = nullptr;
    }
}

// ********************************
//
void            SceneNodeRepr::Update               ( const Transform & parentTransform )
{
    m_transformable->UpdateTransform( parentTransform );

    const auto & worldTransform = m_transformable->WorldTransform();

    for ( auto node : m_sceneNodes )
    {
        if( node->IsVisible() )
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
