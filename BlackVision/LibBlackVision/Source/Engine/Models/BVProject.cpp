#include "stdafx.h"

#include "BVProject.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/ModelSceneEditor.h"
#include "Engine/Models/BVProjectEditor.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Models/Lights/HelperModelLights.h"


namespace bv {

const std::string	BVProject::MAIN_ROOT_NAME			= "main root";
const std::string	BVProject::GLOBAL_TIMELINE_NAME	= "global timeline";

// *******************************
//
void                    BVProject::Serialize           ( ISerializer& ser ) const
{
    ser.EnterArray( "scenes" );

    for( auto & sm : m_sceneModelVec )
    {
        sm->Serialize( ser );
    }

    ser.ExitChild();
}

// *******************************
//
BVProjectPtr  BVProject::Create              ( Renderer * renderer )
{
    struct make_shared_enabler_BVProject : public BVProject { make_shared_enabler_BVProject( Renderer * renderer ): BVProject( renderer ){} };
    return std::make_shared< make_shared_enabler_BVProject >( renderer );
}

// *******************************
//
BVProject::BVProject    ( Renderer * renderer )
    : m_renderer( renderer )
    , m_engineSceneRoot( nullptr )
    , m_timelineManager( std::make_shared < model::TimelineManager >() )
    , m_globalTimeline( model::OffsetTimeEvaluator::Create( GLOBAL_TIMELINE_NAME, TimeType( 0.0 ) ) )
{
    m_timelineManager->RegisterRootTimeline( m_globalTimeline );
    model::TimelineManager::SetInstance( m_timelineManager.get() );

    m_rootNode = model::BasicNode::Create( MAIN_ROOT_NAME, m_timelineManager->GetRootTimeline() );

    m_projectEditor = new BVProjectEditor( this );
}

// *******************************
//
BVProject::~BVProject         ()
{
    delete m_engineSceneRoot;
    delete m_projectEditor;
}

// *******************************
//
void            BVProject::Update( TimeType t )
{
    m_globalTimeline->SetGlobalTime( t );

    if( m_rootNode )
    {
        m_rootNode->Update( t );

        for( auto & scene : m_sceneModelVec )
        {
            scene->Update( t );
        }

        UpdatersManager::Get().UpdateStep();

        //FIXME: camera should be per scene model
        auto viewMat = m_renderer->GetCamera()->GetViewMatrix();
        auto transform = Transform( viewMat, glm::inverse( viewMat ) );

        m_engineSceneRoot->Update( transform );
    }
}

// *******************************
//
model::BasicNodePtr		BVProject::GetModelSceneRoot  () const
{
    return m_rootNode;
}

// *******************************
//
SceneNode *             BVProject::GetEngineSceneRoot   ()  const
{
    return m_engineSceneRoot;
}

// *******************************
//
BVProjectEditor *       BVProject::GetProjectEditor	    () const
{
    return m_projectEditor;
}

// *******************************
//
StringVector            BVProject::ListScenesNames      () const
{
    StringVector ret;
    for( auto & sm : m_sceneModelVec )
    {
        ret.push_back( sm->GetName() );
    }

    return ret;
}

// *******************************
//
model::SceneModelPtr    BVProject::GetModelScene            ( const std::string & name ) const
{
    for( unsigned int i = 0; i < m_sceneModelVec.size(); ++i )
    {
        if( m_sceneModelVec[ i ]->GetName() == name )
        {
            return m_sceneModelVec[ i ];
        }
    }
    return nullptr;
}

// *******************************
//
model::SceneModelPtr    BVProject::GetModelScene            ( UInt32 idx ) const
{
    if( idx < m_sceneModelVec.size() )
    {
        return m_sceneModelVec[ idx ];
    }
    return nullptr;
}

// *******************************
//
const model::SceneModelVec &    BVProject::GetModelScenes   () const
{
    return m_sceneModelVec;
}

// *******************************
//
const SceneVec &                BVProject::GetScenes        () const
{
    return m_sceneVec;
}

// *******************************
//
void							BVProject::SetStartTime	( unsigned long millis )
{
    m_globalTimeline->SetTimeOffset( -TimeType( millis ) * TimeType( 0.001 ) );
}

// *******************************
//
void                            BVProject::DetachEffect ( SceneNode * engineNode )
{
    m_renderer->FreeNodeEffectPDR( engineNode->GetNodeEffect().get() );

    //auto tEntity = engineNode->GetTransformable();

    //if( tEntity != nullptr )
    //{
    //    if( auto rEntity = dynamic_cast< RenderableEntity * >( tEntity ) )
    //    {
    //        auto rEffect = rEntity->GetRenderableEffect();

            
    //    }
    //}
}

} // bv
