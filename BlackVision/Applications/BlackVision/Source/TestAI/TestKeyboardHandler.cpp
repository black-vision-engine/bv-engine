#include "TestKeyboardHandler.h"

#include "BVAppLogic.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/Json/JsonSerializeObject.h"

#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"

namespace bv {

namespace
{
    void SerializeAllEvents( const std::string& fileName );
};


// *********************************
//
TestKeyboardHandler::~TestKeyboardHandler()
{
}

// *********************************
//
void TestKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    if( c == 's' )
    {
        auto sob = new BVXMLSerializer();
        logic->GetBVProject()->GetScenes()[ 0 ]->Serialize( *sob );
        sob->Save( "test.xml" );
        delete sob;
    }
    else if( c == 'j' )
    {
        auto sob = new JsonSerializeObject();
        logic->GetBVProject()->Serialize( *sob );
        sob->Save( "test.json" );
        delete sob;
    }
    else if( c == 'e' )
    {
        // Serialize all events
        SerializeAllEvents( "serialization/Events.json" );
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


void TestKeyboardHandler::OnMouse             ( MouseAction action, int posX, int posY, BVAppLogic * /*logic*/ )
{
    if( action == MouseAction::LEFT_DOWN )
    {
        MouseEventPtr mouseEvent = std::make_shared< MouseEvent >();
        mouseEvent->MouseCommand = MouseEvent::Command::MouseDown;
        mouseEvent->MouseX = (Float32)posX;
        mouseEvent->MouseY = (Float32)posY;

        GetDefaultEventManager().QueueEvent( mouseEvent );
    }
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

    ser->EnterArray( L"Events" );
        heightmapEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        engineStateEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        videoCardEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        widgetEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        timerEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        timelineEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        infoEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        projectEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        sceneEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        pluginStructure->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        nodeStructureEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        paramKeyEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        loadTextureEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        loadFontEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        loadAnimEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        responseEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        globalEffectEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        timelineKeyframeEvent->Serialize( *ser );
    ser->ExitChild();

    ser->Save( fileName, FORMATSTYLE_READABLE );

    delete ser;
}


};

} //bv
