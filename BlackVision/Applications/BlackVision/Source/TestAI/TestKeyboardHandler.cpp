#include "TestKeyboardHandler.h"

#include "BVAppLogic.h"
#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/JsonSpirit/JsonSpiritSerializeObject.h"



namespace bv {

namespace
{
    void SerializeAllEvents();
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
        auto sob = new XMLSerializer();
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
        SerializeAllEvents();
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


namespace
{


void SerializeAllEvents()
{
    JsonSpiritSerializeObject* ser = new JsonSpiritSerializeObject();

    HightmapEventPtr        heightmapEvent      = std::make_shared<HightmapEvent>();
    RenderingModeEventPtr   renderingModeEvent  = std::make_shared<RenderingModeEvent>();
    VideoCardEventPtr       videoCardEvent      = std::make_shared<VideoCardEvent>();
    WidgetEventPtr          widgetEvent         = std::make_shared<WidgetEvent>();
    TimerEventPtr           timerEvent          = std::make_shared<TimerEvent>();
    TimeLineEventPtr        timelineEvent       = std::make_shared<TimeLineEvent>();
    InfoEventPtr            infoEvent           = std::make_shared<InfoEvent>();
    ProjectEventPtr         projectEvent        = std::make_shared<ProjectEvent>();
    PluginStructureEventPtr pluginStructure     = std::make_shared<PluginStructureEvent>();
    NodeStructureEventPtr   nodeStructureEvent  = std::make_shared<NodeStructureEvent>();
    ParamKeyEventPtr        paramKeyEvent       = std::make_shared<ParamKeyEvent>();
    LoadAssetEventPtr       loadAssetEvent      = std::make_shared<LoadAssetEvent>();

    ResponseEventPtr        responseEvent       = std::make_shared<ResponseEvent>();

    ser->EnterArray( L"Events" );
        heightmapEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        renderingModeEvent->Serialize( *ser );
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
        pluginStructure->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        nodeStructureEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        paramKeyEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        loadAssetEvent->Serialize( *ser );
    ser->ExitChild();
    ser->EnterArray( L"Events" );
        responseEvent->Serialize( *ser );
    ser->ExitChild();

    ser->Save( "serialization/Events.json", FORMATSTYLE_READABLE );
}


};

} //bv
