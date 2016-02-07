#include "TestVideoStreamDecoderKeyboardHandler.h"

#include "Engine/Models/BVProjectEditor.h"

#include "MockScenes.h"
#include "BVAppLogic.h"

#include "Engine/Models/Plugins/Simple/DefaultVideoStreamDecoderPlugin.h"
#include "Engine/Models/Plugins/PluginUtils.h"

namespace bv {

// *********************************
//
void TestVideoStreamDecoderKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
	static int seekOffset = 0;
   	auto editor = logic->GetBVProject()->GetProjectEditor();
	auto node = editor->GetScene( "sceneFromEnv: VIDEO_STREAM_TEST_SCENE" )->GetRootNode();
	if( c == 'q' )
    {
        SetParameter( node->GetPlugin( "video_stream_decoder" )->GetParameter( "state" ), 0.0f, DefaultVideoStreamDecoderPlugin::DecoderMode::PLAY );
    }
    else if( c == 'w' )
    {
        SetParameter( node->GetPlugin( "video_stream_decoder" )->GetParameter( "state" ), 0.0f, DefaultVideoStreamDecoderPlugin::DecoderMode::PAUSE );
    }
	else if( c == 'e' )
	{
        SetParameter( node->GetPlugin( "video_stream_decoder" )->GetParameter( "state" ), 0.0f, DefaultVideoStreamDecoderPlugin::DecoderMode::STOP );
    }
	else if( c == 'a' )
    {
		seekOffset -= 1;
		if( seekOffset < 0 ) seekOffset = 0;
        SetParameter( node->GetPlugin( "video_stream_decoder" )->GetParameter( "offset" ), 0.0f, (float)seekOffset );
    }
	else if( c == 's' )
    {
		seekOffset += 1;
        SetParameter( node->GetPlugin( "video_stream_decoder" )->GetParameter( "offset" ), 0.0f, (float)seekOffset );
    }
	else if( c == 'x' )
    {
        SetParameter( node->GetPlugin( "video_stream_decoder" )->GetParameter( "state" ), 0.0f, DefaultVideoStreamDecoderPlugin::DecoderMode::STOP );
		auto desc = VideoStreamAssetDesc::Create( "rsrcy/big_buck_bunny_480p_H264_AAC_25fps_1800K_short.MP4", TextureFormat::F_A8R8G8B8 );
        editor->LoadAsset( node->GetPlugin( "video_stream_decoder" ), desc );
        SetParameter( node->GetPlugin( "video_stream_decoder" )->GetParameter( "state" ), 0.0f, DefaultVideoStreamDecoderPlugin::DecoderMode::PLAY );
    }
	else if( c == 'z' )
    {
		editor->RemoveScene( "sceneFromEnv: VIDEO_STREAM_TEST_SCENE" );
    }
}

} //bv
