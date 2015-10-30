#include "TestRemoteEventsKeyboardHandler.h"


#include "Serialization/JsonSpirit/JsonSpiritSerializeObject.h"
#include "Engine/Events/Events.h"
#include "../EndUserAPI/RemoteCommandsConverter.h"

namespace bv
{

// *********************************
//
TestRemoteEventsKeyboardHandler::TestRemoteEventsKeyboardHandler()
{}
TestRemoteEventsKeyboardHandler::~TestRemoteEventsKeyboardHandler()
{}

// *********************************
//
void TestRemoteEventsKeyboardHandler::HandleKey           ( unsigned char c, BVAppLogic* /*logic*/ )
{
    if( c == 'm' )
    {// test remote commands magic
        std::ifstream file( "textureSerialize.json" );
        std::string assetDesc( (std::istreambuf_iterator<char>( file )), std::istreambuf_iterator<char>() );
	    file.close();

        LoadAssetEventPtr newEvent = std::make_shared<LoadAssetEvent>();
        newEvent->PluginName = "texture";
        newEvent->NodeName = "DEFAULT_CONE";
        newEvent->AssetData = std::move( assetDesc );

        JsonSpiritSerializeObject ser;
        newEvent->Serialize( ser );

        std::wstring pretendRemoteString = ser.Save( FORMATSTYLE_SPARING );
        RemoteCommandsConverter::GetRemoteCommandsConverter().QueueEvent( pretendRemoteString );
    }
}


} //bv
