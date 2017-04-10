#include "stdafxBVApp.h"

#include "TestKeyboardHandler.h"

#include "BVAppLogic.h"

#include "Engine/Graphics/Effects/Logic/RenderLogic.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/OutputLogic.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Output.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/Json/JsonSerializeObject.h"

#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"

#include "Application/ApplicationContext.h"

#include <glm/gtx/euler_angles.hpp>

namespace bv {

namespace
{
    void SerializeAllEvents( const std::string& fileName );
};


// ***********************
//
TestKeyboardHandler::TestKeyboardHandler()
    :   m_moveCamera( false )
    ,   m_moveMultiplierBase( 0.03f )
    ,   m_rotationMultiplierBase( 0.02f )
    ,   m_lastX( 0 )
    ,   m_lastY( 0 )
{
    m_moveMultiplier = ComputeMoveMultiplier( '5' );
    m_rotationMultiplier = ComputeRotationMultiplier( '5' );
}

// *********************************
//
TestKeyboardHandler::~TestKeyboardHandler()
{}

// *********************************
//
void TestKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    if( c >= '1' && c <= '9' || c == '0' )
    { 
        //auto outputLogic = logic->GetRenderLogic()->GetOutputLogic();

        //auto preview = outputLogic->GetOutput( nrl::CustomOutputType::COT_PREVIEW );
        //auto & statePrev = preview->AccessOutputState();

        //auto shm = outputLogic->GetOutput( nrl::CustomOutputType::COT_STREAM_SHM );
        //auto & stateShm = shm->AccessOutputState();

        switch( c )
        {
        //    case '1':
        //        statePrev.SetChannelMapping( 0, 1, 2, 3 );
        //        statePrev.SetMaskState( true, false, false, false );
        //        break;
        //    case '2':
        //        statePrev.SetChannelMapping( 0, 1, 2, 3 );
        //        statePrev.SetMaskState( false, true, false, false );
        //        break;
        //    case '3':
        //        statePrev.SetChannelMapping( 0, 1, 2, 3 );
        //        statePrev.SetMaskState( false, false, true, false );
        //        break;
        //    case '4':
        //        statePrev.SetChannelMapping( 3, 3, 3, 3 );
        //        statePrev.SetMaskState( true, true, true, true );
        //        break;
        //    case '5':
        //        statePrev.SetChannelMapping( 0, 1, 2, 3 );
        //        statePrev.SetMaskState( true, true, true, true );
        //        break;
        //    case '6':
        //        stateShm.SetChannelMapping( 0, 1, 2, 3 );
        //        stateShm.SetMaskState( true, false, false, false );
        //        break;
        //    case '7':
        //        stateShm.SetChannelMapping( 0, 1, 2, 3 );
        //        stateShm.SetMaskState( false, true, false, false );
        //        break;
        //    case '8':
        //        stateShm.SetChannelMapping( 0, 1, 2, 3 );
        //        stateShm.SetMaskState( false, false, true, false );
        //        break;
        //    case '9':
        //        stateShm.SetChannelMapping( 3, 3, 3, 3 );
        //        stateShm.SetMaskState( true, true, true, true );
        //        break;
        //    case '0':
        //        stateShm.SetChannelMapping( 0, 1, 2, 3 );
        //        stateShm.SetMaskState( true, true, true, true );
        //        break;
            case'1':
            default:
                assert( false );
        }

        return;
    }
    //if( c == 's' )
    //{
    //    auto sob = new BVXMLSerializer();
    //    logic->GetBVProject()->GetModelScenes()[ 0 ]->Serialize( *sob );
    //    sob->Save( "test.xml" );
    //    delete sob;
    //}
    //else if( c == 'j' )
    //{
    //    auto sob = new JsonSerializeObject();
    //    logic->GetBVProject()->Serialize( *sob );
    //    sob->Save( "test.json" );
    //    delete sob;
    //}
    //else if( c == 'e' )
    //{
    //    // Serialize all events
    //    SerializeAllEvents( "serialization/Events.json" );
    //}
    //

    if( c >= '0' && c <= '9' )
    {
        m_moveMultiplier = ComputeMoveMultiplier( c );
        m_rotationMultiplier = ComputeRotationMultiplier( c );
    }

    if( c == 'w' || c == 's' )
    {
        float multiplier = m_moveMultiplier;
        if( c == 's' )
            multiplier = -1.0f * multiplier;

        // Move forward and backward camera logic
        auto & scenes = logic->GetBVProject()->GetModelScenes();
        for( auto & scene : scenes )
        {
            auto & camera = scene->GetCamerasLogic().GetCurrentCamera();
            auto posValue = QueryTypedValue< ValueVec3Ptr >( camera->GetValue( "Position" ) );
            auto posParam = camera->GetParameter( "Position" );
            auto dirValue = QueryTypedValue< ValueVec3Ptr >( camera->GetValue( "Direction" ) );

            glm::vec3 translation = dirValue->GetValue() * multiplier;
            model::SetParameter( posParam, TimeType( 0.0f ), posValue->GetValue() + translation );
        }
    }

    if( c == 'a' || c == 'd' )
    {
        float multiplier = m_moveMultiplier;
        if( c == 'a' )
            multiplier = -1.0f * multiplier;

        // Move left and right camera logic
        auto & scenes = logic->GetBVProject()->GetModelScenes();
        for( auto & scene : scenes )
        {
            auto & camera = scene->GetCamerasLogic().GetCurrentCamera();
            auto posValue = QueryTypedValue< ValueVec3Ptr >( camera->GetValue( "Position" ) );
            auto posParam = camera->GetParameter( "Position" );
            auto dirValue = QueryTypedValue< ValueVec3Ptr >( camera->GetValue( "Direction" ) );
            auto upValue = QueryTypedValue< ValueVec3Ptr >( camera->GetValue( "UpVector" ) );

            glm::vec3 rightVec = glm::cross( dirValue->GetValue(), upValue->GetValue() );

            glm::vec3 translation = rightVec * multiplier;
            model::SetParameter( posParam, TimeType( 0.0f ), posValue->GetValue() + translation );
        }
    }

    if( c == 'r' || c == 'f' )
    {
        float multiplier = m_moveMultiplier;
        if( c == 'f' )
            multiplier = -1.0f * multiplier;

        // Move up and down camera logic
        auto & scenes = logic->GetBVProject()->GetModelScenes();
        for( auto & scene : scenes )
        {
            auto & camera = scene->GetCamerasLogic().GetCurrentCamera();
            auto posValue = QueryTypedValue< ValueVec3Ptr >( camera->GetValue( "Position" ) );
            auto posParam = camera->GetParameter( "Position" );

            glm::vec3 translation = glm::vec3( 0.0, 1.0, 0.0 ) * multiplier;
            model::SetParameter( posParam, TimeType( 0.0f ), posValue->GetValue() + translation );
        }
    }

    { logic; }
    // To be implemented in subclasses

    //auto root = m_modelScene->GetSceneRoot();
    //auto timerPlugin = root->GetPlugin("timer");
    //if(c == 'q')
    //{
    //    model::StartTimerPlugin( timerPlugin );
    //}

    //if(c == 'w')
    //{
    //    model::StopTimerPlugin( timerPlugin );
    //}
    //   

    //if(c == 'a')
    //{
    //    model::SetTimeTimerPlugin( timerPlugin, 3600.f * 5 + 60.f * 4 + 23.f + 0.12f );
    //}

    //if(c == 's')
    //{
    //    model::SetTimeTimerPlugin( timerPlugin, 43.f + 0.88f );
    //}
        
    //FIXME: keypressed event was used here to set text in all currently loaded Text plugins
    //KeyPressedSendEvent( c );
    //TODO: implement whatever you want here

}


void TestKeyboardHandler::OnMouse             ( MouseAction action, int posX, int posY, BVAppLogic * logic )
{
    auto & context = ApplicationContext::Instance();
    posX = static_cast< int >( posX * (double)context.GetWidth() / (double)context.GetClientWidth() );
    posY = static_cast< int >( posY * (double)context.GetHeight() / (double)context.GetClientHeight() );

    if( action == MouseAction::LEFT_DOWN )
    {
        MouseEventPtr mouseEvent = std::make_shared< MouseEvent >();
        mouseEvent->MouseCommand = MouseEvent::Command::MouseDown;
        mouseEvent->MouseX = (Float32)posX;
        mouseEvent->MouseY = (Float32)posY;
        mouseEvent->AutoSelect = true;
        mouseEvent->AutoSelectColor = glm::vec4( 1, 0, 0, 1 );
        mouseEvent->SocketID = SEND_BROADCAST_EVENT;
        mouseEvent->EventID = 0;

        GetDefaultEventManager().QueueEvent( mouseEvent );
    }
    else if( action == MouseAction::RIGHT_DOWN )
    {
        m_moveCamera = true;
    }
    else if( action == MouseAction::RIGHT_UP )
    {
        m_moveCamera = false;
    }
    else if( action == MouseAction::MOVE )
    {
        if( m_moveCamera )
        {
            // Move left and right camera logic
            auto & scenes = logic->GetBVProject()->GetModelScenes();
            for( auto & scene : scenes )
            {
                auto & camera = scene->GetCamerasLogic().GetCurrentCamera();

                auto dirValue = QueryTypedValue< ValueVec3Ptr >( camera->GetValue( "Direction" ) );
                auto dirParam = camera->GetParameter( "Direction" );

                glm::mat4 rotMatrix = glm::eulerAngleYXZ( m_rotationMultiplier * (float)( m_lastX - posX ), 0.0f, 0.0f );
                glm::vec3 newDir = glm::vec3( rotMatrix * glm::vec4( dirValue->GetValue(), 0.0 ) );
                
                model::SetParameter( dirParam, TimeType( 0.0f ), newDir );
            }
        }

        m_lastX = posX;
        m_lastY = posY;
    }
}

// ***********************
//
float TestKeyboardHandler::ComputeMoveMultiplier   ( char c )
{
    float param = 1.5f * static_cast< float >( c - '5' );
    float value = pow( 2.0f, param );
    return m_moveMultiplierBase * value;
}

// ***********************
//
float TestKeyboardHandler::ComputeRotationMultiplier   ( char c )
{
    float param = static_cast< float >( c - '8' );
    float value = pow( 2.0f, param );
    return m_rotationMultiplierBase * value;
}

namespace
{


void SerializeAllEvents( const std::string& fileName )
{
    JsonSerializeObject*  ser = new JsonSerializeObject();

    HightmapEventPtr        heightmapEvent      = std::make_shared<HightmapEvent>();
    EngineStateEventPtr     engineStateEvent    = std::make_shared<EngineStateEvent>();
    VideoCardEventPtr       videoCardEvent      = std::make_shared<VideoCardEvent>();
    NodeLogicEventPtr       widgetEvent         = std::make_shared<NodeLogicEvent>();
    TimerEventPtr           timerEvent          = std::make_shared<TimerEvent>();
    TimeLineEventPtr        timelineEvent       = std::make_shared<TimeLineEvent>();
    InfoEventPtr            infoEvent           = std::make_shared<InfoEvent>();
    ProjectEventPtr         projectEvent        = std::make_shared<ProjectEvent>();
    SceneEventPtr           sceneEvent          = std::make_shared<SceneEvent>();
    PluginStructureEventPtr pluginStructure     = std::make_shared<PluginStructureEvent>();
    NodeStructureEventPtr   nodeStructureEvent  = std::make_shared<NodeStructureEvent>();
    ParamKeyEventPtr        paramKeyEvent       = std::make_shared<ParamKeyEvent>();
    LoadAssetEventPtr       loadTextureEvent    = std::make_shared<LoadAssetEvent>();
    LoadAssetEventPtr       loadFontEvent       = std::make_shared<LoadAssetEvent>();
    LoadAssetEventPtr       loadAnimEvent       = std::make_shared<LoadAssetEvent>();
    GlobalEffectEventPtr    globalEffectEvent   = std::make_shared<GlobalEffectEvent>();
    TimelineKeyframeEventPtr        timelineKeyframeEvent     = std::make_shared<TimelineKeyframeEvent>();

    ResponseEventPtr        responseEvent       = std::make_shared<ResponseEvent>();
    

    assert( "Wrong assets serialization" );

    //TextureAssetDescConstPtr    texAsset =      TextureAssetDesc::Create( std::string( "textures/poison.jpg" ), true );
    //FontAssetDescConstPtr       fontAsset =     FontAssetDesc::Create( std::string( "filePath.jpg" ), 0, 0, 0, false );
    //AnimationAssetDescConstPtr  animAsset =     AnimationAssetDesc::Create( std::string( "filePath.jpg" ), 1, 0, 0, ".*" );
    //
    //JsonSerializeObject* serAssetTex = new JsonSerializeObject();
    //texAsset->Serialize( *serAssetTex );
    //loadTextureEvent->AssetData = serAssetTex;

    //JsonSerializeObject* serAssetFont = new JsonSerializeObject();
    //fontAsset->Serialize( *serAssetFont );
    //loadFontEvent->AssetData = serAssetFont;

    //JsonSerializeObject* serAssetAnim = new JsonSerializeObject();
    //animAsset->Serialize( *serAssetAnim );
    //loadAnimEvent->AssetData = serAssetAnim;

    ser->EnterArray( "Events" );
        heightmapEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        engineStateEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        videoCardEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        widgetEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        timerEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        timelineEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        infoEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        projectEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        sceneEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        pluginStructure->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        nodeStructureEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        paramKeyEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        loadTextureEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        loadFontEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        loadAnimEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        responseEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        globalEffectEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( "Events" );
        timelineKeyframeEvent->Serialize( *ser );
    ser->ExitChild();

    ser->Save( fileName, FORMATSTYLE_READABLE );

    delete ser;
}


};

} //bv
