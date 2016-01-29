#include "stdafx.h"

#include "BVProject.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/BVProjectEditor.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

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
    , m_globalTimeline( new model::OffsetTimeEvaluator( GLOBAL_TIMELINE_NAME, TimeType( 0.0 ) ) )
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
SceneNode *             BVProject::GetEngineSceneRoot ()  const
{
    return m_engineSceneRoot;
}

// *******************************
//
BVProjectEditor *       BVProject::GetProjectEditor		()
{
    return m_projectEditor;
}

// *******************************
//
bool					BVProject::AddScene				( model::SceneModelPtr sceneModel )
{
	return AddScene( sceneModel, ( UInt32 )m_sceneModelVec.size() );
}

// *******************************
//
bool                    BVProject::AddScene				( model::SceneModelPtr sceneModel, UInt32 idx )
{
	//FIXME: prevent adding two scenes with the same name
	if( GetScene( sceneModel->GetName() ) )
	{
		return false;
	}
	
	if( idx < m_sceneModelVec.size() )
	{
		m_sceneModelVec.insert( m_sceneModelVec.begin() + idx, sceneModel );
	}
	else
	{
		m_sceneModelVec.push_back( sceneModel );
	}

	if( sceneModel->GetRootNode() )
	{
		m_rootNode->AddChildToModelOnly( sceneModel->GetRootNode(), idx );
	}

	m_globalTimeline->AddChild( sceneModel->GetTimeline() );
	
	return true;
}

// *******************************
//
bool                    BVProject::RemoveScene			( model::SceneModelPtr sceneModel )
{
	for( unsigned int i = 0; i < m_sceneModelVec.size(); ++i )
	{
		if( m_sceneModelVec[ i ] == sceneModel )
		{
            if( m_sceneModelVec[ i ]->GetRootNode() )
            {
			    m_rootNode->DetachChildNodeOnly( m_sceneModelVec[ i ]->GetRootNode() );
            }
			m_globalTimeline->RemoveChild( m_sceneModelVec[ i ]->GetTimeline() );

			m_sceneModelVec.erase( m_sceneModelVec.begin() + i );

			return true;
		}
	}
	return false;
}

// *******************************
//
StringVector            BVProject::ListScenesNames     () const
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
model::SceneModelPtr    BVProject::GetScene            ( const std::string & name ) const
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
model::SceneModelPtr    BVProject::GetScene            ( UInt32 idx ) const
{
    if( idx < m_sceneModelVec.size() )
    {
        return m_sceneModelVec[ idx ];
    }
    return nullptr;
}

// *******************************
//
const model::SceneModelVec &    BVProject::GetScenes    () const
{
    return m_sceneModelVec;
}

// *******************************
//
void							BVProject::SetStartTime		( unsigned long millis )
{
	m_globalTimeline->SetTimeOffset( -TimeType( millis ) * TimeType( 0.001 ) );
}

} // bv
