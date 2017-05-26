#include "stdafxBVApp.h"

#include "HightmapHandlers.h"

#include "BVAppLogic.h"
#include "Engine/Models/Plugins/Custom/DefaultHeightMapPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"
#include "UseLoggerBVAppModule.h"

#include "Engine/Events/EventHandlerHelpers.h"

namespace bv
{

// ***********************
//
HightmapHandlers::HightmapHandlers( BVAppLogic* appLogic )
    : m_appLogic( appLogic )
{}
HightmapHandlers::~HightmapHandlers()
{}

// *********************************
//
void            HightmapHandlers::HightmapHandler         ( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != bv::HightmapEvent::Type() )
        return;

    HightmapEventPtr evtHightmap = std::static_pointer_cast<bv::HightmapEvent>( eventPtr );
    BVProjectPtr modelScene = m_appLogic->GetBVProject();
    auto root = modelScene->GetModelSceneRoot();

    //float hours = evtHightmap->Hours;
    //float minutes = evtHightmap->Minutes;
    //float seconds = evtHightmap->Seconds;
    //float millis = evtHightmap->Milliseconds;
    HightmapEvent::Command command = evtHightmap->HightmapCommand;

    if( command == HightmapEvent::Command::Enable )
    {
        printf("hm enable\n");
            modelScene->GetModelSceneRoot()->GetChild(".")->SetVisible(true);
				 
        //hm_enable = true;
            
    }
    else if( command == HightmapEvent::Command::Start )
    {
        printf("hm start\n");
        //this->HMStart(evtTimer->H);
    }
    else if( command == HightmapEvent::Command::Reset )
    {
        modelScene->GetModelSceneRoot()->GetChild(".")->SetVisible(false);
        // hm_enable = false;
        //this->HMReset();
    }
    else if( command == HightmapEvent::Command::Show )
    {
        printf("hm show\n");
        //this->HMShow();
    }
    else if( command == HightmapEvent::Command::Zoom )
    {
        printf("hm zoom\n");
        //FIXME: USE NEW API
        // this->HMZoomIn( evtTimer->H,evtTimer->M,evtTimer->S,2.0f );
    }
    else if( command == HightmapEvent::Command::ZoomOut )
    {
        printf("hm zoom out\n");
        //FIXME: USE NEW API
        //this->HMZoomOutFromCurrent( evtTimer->H );
    }
    else if( command == HightmapEvent::Command::Set )
    {
        printf("hm set\n");
        //HMSetCyclistPosition( 1, evtTimer->H );
        //HMSetCyclistPosition( 2, evtTimer->M );
        // HMSetCyclistPosition( 3, evtTimer->S );
    }
    else if( command == HightmapEvent::Command::Anim )
    {
        printf("hm anim\n");
            //this->HMConcentratedDistanceAnimStart(9);
    }
    else if( command == HightmapEvent::Command::Anim2 )
    {
        printf("hm anim2\n");
        // this->HMConcentratedDistanceAnimStart2(9);
    }
    else if( command == HightmapEvent::Command::Set3 )
    {
        // this->HMStart( 150 );
    }
    else
    {
        SendSimpleErrorResponse( command, evtHightmap->EventID, evtHightmap->SocketID, "Unknown command" );
        return;
    }

    //SendSimpleResponse( command, evtHightmap->EventID, evtHightmap->SocketID, true );
    SendSimpleErrorResponse( command, evtHightmap->EventID, evtHightmap->SocketID, "Hightmap events are not implemented yet" );
}


// *********************************
//
void            HightmapHandlers::HMSetCyclistPosition    ( int cyclistPos, float km )
{
    assert( cyclistPos > 0 );
    assert( size_t(cyclistPos) <= m_cyclistPos.size() );

    if(cyclistPos==1)
    {
		auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
        model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.0f );
    }

    m_cyclistPos[ cyclistPos - 1 ] = km;
}

// *********************************
//
void            HightmapHandlers::UpdateCyclistPosition    ( int cyclistPos, float km )
{
    assert( cyclistPos > 0 );
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );

    auto cyclistNode = root->GetChild( "cyclistNode" + std::to_string( cyclistPos - 1 ) );
    if( cyclistNode )
    {
        //if( cyclistPos == 1 )
///model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.f );

        auto transformPlugin = cyclistNode->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        auto pos = tp->QueryEdgePosition( km * 1000.f );
        float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
        float y = ( 2.f * pos.y - 1.f );

        model::SetParameterTranslation( param, 0.0f, glm::vec3( x, y, 0.f ) );

    }
}


// *********************************
//
void            HightmapHandlers::HMShow                  ()
{
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
	auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "timeline [ height map ] does not exist :(";
        return;
    }

    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    //auto time1 = timeline->GetKeyFrameEvent( "stop0" )->GetEventTime();

    auto time2 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    
    
    SetParameterTranslation( tr, time2, glm::vec3(0,0,0) );

    timeline->Play();
}

// *********************************
//
void            HightmapHandlers::HMReset                  ()
{
	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    timeline->SetTimeAndStop(0.0f);
}

// *********************************
//

// *********************************
//
void            HightmapHandlers::HMStart                 ( float km )
{
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "timeline [ height map ] does not exist :(";
        return;
    }

    auto time1 = timeline->GetKeyFrameEvent( "stop1" )->GetEventTime();
    auto time2 = timeline->GetKeyFrameEvent( "stop2" )->GetEventTime();


    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.0f ); // 80km // 220863.f * 0.5f //pionowa kreska
    
    model::SetParameter( hmplugin->GetParameter( "coveredDistShowFactor" ), time1, 0.0f );
    model::SetParameter( hmplugin->GetParameter( "coveredDistShowFactor" ), time2, 1.f );

    timeline->Play();
}

// *********************************
//FIXME: uses hardcoded totalDistance
void        HightmapHandlers::HMZoomIn                ( float km, float leftKM, float rightKM, float yScale )
{
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "timeline [ height map ] does not exist :(";
        return;
    }

    float visibleZoomedKm = leftKM + rightKM; //powinno sie nazywac totalzoomedkm

    auto time2 = timeline->GetKeyFrameEvent( "stop2" )->GetEventTime();
    auto time3 = timeline->GetKeyFrameEvent( "stop3" )->GetEventTime();
    auto time5 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    //FIXME: totalDistance in meters- should be set globally or read from scene parameters
    float totalDistance = 173707.f;
    
    float xscl = totalDistance / (visibleZoomedKm * 500.f);
    float ixscl = 1.0f / xscl;
    
    float u1 = ( km * 1000.f ) / totalDistance;
    float u0 = leftKM / ( leftKM + rightKM );

    float xSnapNormalized = ( u1 - u0 * ixscl ) / ( 1.0f - ixscl );
    float xSnapInMeters = xSnapNormalized * totalDistance;

    //model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.f );
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( 0.f ), xSnapInMeters );
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( time5 ), xSnapInMeters );
    
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time2 ), glm::vec2( 1.f, 1.f ) ); //yScale == 2.0f w starej wersji
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time3 ), glm::vec2( xscl, yScale ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time5 ), glm::vec2( xscl, yScale ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2

    m_concentrateDistMarks[ 0 ] = km - 20;
    m_concentrateDistMarks[ 1 ] = km - 15;
    m_concentrateDistMarks[ 2 ] = km - 10;
    m_concentrateDistMarks[ 3 ] = km - 5;
    m_concentrateDistMarks[ 4 ] = km;
    m_concentrateDistMarks[ 5 ] = km + 5;
    m_concentrateDistMarks[ 6 ] = km + 10;
    m_concentrateDistMarks[ 7 ] = km + 15;
    m_concentrateDistMarks[ 8 ] = km + 20;

    timeline->Play();
}

// *********************************
//FIXME: uses hardcoded totalDistance
void            HightmapHandlers::HMZoomOutFromCurrent    (float scale)
{
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
     auto hmplugin = root->GetPlugin( "height map" );

	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "timeline [ height map ] does not exist :(";
        return;
    }

    
    auto tr = root->GetPlugin( "height map" )->GetParameter( "simple_transform" );
    auto time3 = timeline->GetKeyFrameEvent( "stop6" )->GetEventTime();

    
    //model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( time3 ), 174000.f );

    //SetParameterTranslation( tr, 0, time3, glm::vec3(-1,0,0) );


   
    

    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time3 ), glm::vec2( scale, 1.0f ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2

    timeline->Play();
}

// *********************************
//FIXME: uses hardcoded totalDistance
void        HightmapHandlers::HMZoomInFake                ( float km, float leftKM, float rightKM, float yScale )
{
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "timeline [ height map ] does not exist :(";
        return;
    }

    float visibleZoomedKm = leftKM + rightKM; //powinno sie nazywac totalzoomedkm

    auto time2 = timeline->GetKeyFrameEvent( "stop4" )->GetEventTime();
    auto time3 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    //FIXME: totalDistance in meters- should be set globally or read from scene parameters
    float totalDistance = 173707.f;
    
    float xscl = totalDistance / (visibleZoomedKm * 500.f);
    float ixscl = 1.0f / xscl;
    
    float u1 = ( km * 1000.f ) / totalDistance;
    float u0 = leftKM / ( leftKM + rightKM );

    float xSnapNormalized = ( u1 - u0 * ixscl ) / ( 1.0f - ixscl );
    float xSnapInMeters = xSnapNormalized * totalDistance;

    //model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.f );
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( 0.f ), xSnapInMeters );
    
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time2 ), glm::vec2( 1.f, 1.f ) ); //yScale == 2.0f w starej wersji
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time3 ), glm::vec2( xscl, yScale ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2

    m_concentrateDistMarks[ 0 ] = km - 20;
    m_concentrateDistMarks[ 1 ] = km - 15;
    m_concentrateDistMarks[ 2 ] = km - 10;
    m_concentrateDistMarks[ 3 ] = km - 5;
    m_concentrateDistMarks[ 4 ] = km;
    m_concentrateDistMarks[ 5 ] = km + 5;
    m_concentrateDistMarks[ 6 ] = km + 10;
    m_concentrateDistMarks[ 7 ] = km + 15;
    m_concentrateDistMarks[ 8 ] = km + 20;

    timeline->Play();
}

// *********************************
//
void            HightmapHandlers::HMConcentratedDistanceAnimStart( int num )
{
	{num;}
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );

	{tp;}

	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "timeline [ height map ] does not exist :(";
        return;
    }
       
    auto time3 = timeline->GetKeyFrameEvent( "stop3" )->GetEventTime();
    auto time4 = timeline->GetKeyFrameEvent( "stop4" )->GetEventTime();

    {
        auto markTextConcentratedRoot = root->GetChild( "markTextConcentratedRoot" );

        auto transformPlugin = markTextConcentratedRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, time3, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, time4, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    {
        auto markTextRoot = root->GetChild( "markTextRoot" );

        auto transformPlugin = markTextRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, time4, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, time3, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    timeline->Play();
}
// *********************************
//
void            HightmapHandlers::HMConcentratedDistanceAnimStart2( int num )
{
	{num;}

    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );
	{tp;}

	//FIXME: GetTimeline finds the first timeline with given name; names might not be unique
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "timeline [ height map ] does not exist :(";
        return;
    }
       
    auto time3 = timeline->GetKeyFrameEvent( "stop4" )->GetEventTime();
    auto time4 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    {
        auto markTextConcentratedRoot = root->GetChild( "markTextConcentratedRoot" );

        auto transformPlugin = markTextConcentratedRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, time4, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, time3, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    {
        auto markTextRoot = root->GetChild( "markTextRoot" );

        auto transformPlugin = markTextRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, time3, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, time4, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    timeline->Play();
}



// *********************************
//
void    HightmapHandlers::UpdateHM        ()
{
	bool hm_enable=false;
    if(hm_enable){
                
        auto root = m_appLogic->GetBVProject()->GetModelSceneRoot()->GetChild(".");
        auto hmplugin = root->GetPlugin( "height map" );
        //auto rctn = root->GetChild( "rct" );
        //auto rect = rctn->GetPlugin( "transform" );
        //auto param = rect->GetParameter( "simple_transform" );
                
        model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );

        //kreska
        //auto pos = tp->QueryEdgePosition( 73000.f );
        //float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
        ////SetParameterTranslation( param, 0.0f, glm::vec3( x, 2.f * 98.f / 1080.f, 0.f ) );
        //SetParameterTranslation( param, 0.0f, glm::vec3( x, -( pos.y + 98.0f / 1080.f ) * 2.f, 0.f ) );
        //SetParameterScale( param, 0.0f, glm::vec3( 1.f, pos.y, 1.f ) );

        //auto pos = tp->QueryEdgePosition( 73000.f );
        //float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
        //float y = ( 2.f * pos.y - 1.f );
        //SetParameterTranslation( param, 0.0f, glm::vec3( x, y, 0.f ) );


        auto startFinishRoot = root->GetChild( "startFinishRoot" );

        if( startFinishRoot )
        {
            {
                auto startTexture = startFinishRoot->GetChild( "startTexture" );

                auto transformPlugin = startTexture->GetPlugin( "transform" );
                auto param = transformPlugin->GetParameter( "simple_transform" );
                auto pos = tp->QueryEdgePosition( 6000.f );
                float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                float y = ( 2.f * pos.y - 1.f );

                SetParameterTranslation( param, 0.0f, glm::vec3( x, y + 180.f / 1080.f , 0.f ) );
            }

            {
                auto finishTexture = startFinishRoot->GetChild( "finishTexture" );

                auto transformPlugin = finishTexture->GetPlugin( "transform" );
                auto param = transformPlugin->GetParameter( "simple_transform" );
                auto pos = tp->QueryEdgePosition( 170707.f );
                float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                float y = ( 2.f * pos.y - 1.f );

                SetParameterTranslation( param, 0.0f, glm::vec3( x, y + 180.f / 1080.f, 0.f ) );
            }



            //    auto distPoint = markTextRoot->GetChild( "markText" + to_string( i ) );
            //    if( distPoint )
            //    {
            //        auto transformPlugin = distPoint->GetPlugin( "transform" );
            //        auto param = transformPlugin->GetParameter( "simple_transform" );

            //        auto textPlugin = std::static_pointer_cast< model::DefaultTextPlugin >( distPoint->GetPlugin( "text" ) );

            //        auto text = textPlugin->GetText();
            //        auto distansInKm = _wtoi( text.c_str() ) * 1000;

            //        auto pos = tp->QueryEdgePosition( distansInKm );
            //        float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
            //        float y = ( 2.f * pos.y - 1.f );

            //        SetParameterTranslation( param, 0.0f, glm::vec3( x, 0.f, 0.f ) );
            //    }
            //    else
            //    {
            //        break;
            //    }
            //}
        }


        auto markTextRoot = root->GetChild( "markTextRoot" );

        if( markTextRoot )
        {
            for( int i = 0; i < 100; ++i )
            {
                auto distPoint = markTextRoot->GetChild( "markText" + std::to_string( i ) );
                if( distPoint )
                {
                    auto transformPlugin = distPoint->GetPlugin( "transform" );
                    auto param = transformPlugin->GetParameter( "simple_transform" );

                    auto textPlugin = std::static_pointer_cast< model::DefaultTextPlugin >( distPoint->GetPlugin( "text" ) );

                    auto text = textPlugin->GetText();
                    auto distansInKm = _wtoi( text.c_str() ) * 1000;

                    auto pos = tp->QueryEdgePosition( (float)distansInKm );
                    float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                    //float y = ( 2.f * pos.y - 1.f );

                    SetParameterTranslation( param, 0.0f, glm::vec3( x, 0.f, 0.f ) );
                }
                else
                {
                    break;
                }
            }
        }


        auto markTextConcentratedRoot = root->GetChild( "markTextConcentratedRoot" );

        if( markTextConcentratedRoot )
        {
            for( int i = 0; i < 9; ++i )
            {
                auto markConcentratedNode = markTextConcentratedRoot->GetChild( "markTextConcentrated" + std::to_string( i ) );
     
                float p = m_concentrateDistMarks[ i ];

                model::SetParameter( markConcentratedNode->GetPlugin( "text" )->GetParameter( "text" ), 0.0, std::to_wstring( int( p ) ) + L" km" );

                auto pos = tp->QueryEdgePosition( p * 1000.f );
                float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                //float y = ( 2.f * pos.y - 1.f );

                auto transformPlugin = markConcentratedNode->GetPlugin( "transform" );
                auto param = transformPlugin->GetParameter( "simple_transform" );

                SetParameterTranslation( param, 0.0f, glm::vec3( x, 0.f, 0.f ) );

            }
        }

        for( int i = 0; i < 3; ++i )
        {
            UpdateCyclistPosition( i + 1, m_cyclistPos[ i ] );
        }

    }
}


} //bv
