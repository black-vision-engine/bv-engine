#include "stdafxBVApp.h"

#include "TestVideoStreamDecoderKeyboardHandler.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "TestingScenes/MockScenes.h"
#include "BVAppLogic.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAVDecoderPlugin.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/Json/JsonSerializeObject.h"


namespace bv {

// *********************************
//
void TestVideoStreamDecoderKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
	static int seekOffset = 0;
   	auto editor = logic->GetBVProject()->GetProjectEditor();
	auto node = editor->GetModelScene( "sceneFromEnv@ VIDEO_STREAM_TEST_SCENE" )->GetRootNode();
    auto plugin = node->GetPlugin( "av_decoder" );
	if( c == 'q' )
    {
        SetParameter( plugin->GetParameter( "state" ), 0.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
    }
    else if( c == 'w' )
    {
        SetParameter( plugin->GetParameter( "state" ), 0.0f, model::DefaultAVDecoderPlugin::DecoderMode::PAUSE );
    }
	else if( c == 'e' )
	{
        SetParameter( plugin->GetParameter( "state" ), 0.0f, model::DefaultAVDecoderPlugin::DecoderMode::STOP );
    }
	else if( c == 'a' )
    {
		seekOffset -= 1;
		if( seekOffset < 0 ) seekOffset = 0;
        SetParameter( plugin->GetParameter( "offset" ), 0.0f, glm::vec2( (float)seekOffset, 0.0f ) );
    }
	else if( c == 's' )
    {
		seekOffset += 1;
        SetParameter( plugin->GetParameter( "offset" ), 0.0f, glm::vec2( (float)seekOffset, 0.0f ) );
    }
	else if( c == 'x' )
    {
        SetParameter( plugin->GetParameter( "state" ), 0.0f, model::DefaultAVDecoderPlugin::DecoderMode::STOP );
		auto desc = AVAssetDesc::Create( "rsrcy/big_buck_bunny_480p_H264_AAC_25fps_1800K_short.MP4", TextureFormat::F_A8R8G8B8 );
        editor->LoadAsset( plugin, desc );
        SetParameter( plugin->GetParameter( "state" ), 0.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
    }
	else if( c == 'z' )
    {
		editor->RemoveScene( "sceneFromEnv@ VIDEO_STREAM_TEST_SCENE" );
    }
    else if( c == 'g' )
    {
        BVXMLSerializer ser;
        //auto sob = new JsonSerializeObject();
        logic->GetBVProject()->GetModelScenes()[ 0 ]->Serialize( ser );
        ser.Save( "test.xml" );
    }
}

} //bv
