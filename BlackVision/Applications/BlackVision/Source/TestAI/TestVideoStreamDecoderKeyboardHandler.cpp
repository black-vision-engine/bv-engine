#include "TestVideoStreamDecoderKeyboardHandler.h"

#include "Engine/Models/BVSceneEditor.h"

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
   	auto editor = logic->GetBVScene()->GetSceneEditor();
	auto node = editor->GetScene( "sceneFromEnv: VIDEO_STREAM_TEST_SCENE" )->GetRootNode();
	if( c == 'q' )
    {
		DefaultVideoStreamDecoderPlugin::Start( node->GetPlugin( "video_stream_decoder" ) );
    }
    else if( c == 'w' )
    {
		DefaultVideoStreamDecoderPlugin::Pause( node->GetPlugin( "video_stream_decoder" ) );
    }
	else if( c == 'e' )
	{
		DefaultVideoStreamDecoderPlugin::Stop( node->GetPlugin( "video_stream_decoder" ) );
    }
	else if( c == 'a' )
    {
		seekOffset -= 1;
		if( seekOffset < 0 ) seekOffset = 0;
		DefaultVideoStreamDecoderPlugin::Seek( node->GetPlugin( "video_stream_decoder" ), seekOffset );
    }
	else if( c == 's' )
    {
		seekOffset += 1;
		DefaultVideoStreamDecoderPlugin::Seek( node->GetPlugin( "video_stream_decoder" ), seekOffset );
    }
	else if( c == 'x' )
    {
		DefaultVideoStreamDecoderPlugin::Stop( node->GetPlugin( "video_stream_decoder" ) );
		model::LoadVideoStream( node->GetPlugin( "video_stream_decoder" ), "rsrcy/big_buck_bunny_480p_H264_AAC_25fps_1800K_short.MP4", TextureFormat::F_A8R8G8B8 );
		model::DefaultVideoStreamDecoderPlugin::Start( node->GetPlugin( "video_stream_decoder" ) );
    }
	else if( c == 'z' )
    {
		editor->RemoveScene( "sceneFromEnv: VIDEO_STREAM_TEST_SCENE" );
    }
}

} //bv
