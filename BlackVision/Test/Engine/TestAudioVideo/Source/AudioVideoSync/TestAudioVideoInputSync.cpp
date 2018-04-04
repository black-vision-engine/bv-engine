#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "Utils/Mocks/FakeVideoCard/FakeVideoCard.h"
#include "Utils/Nodes/TestNodesCreator.h"
#include "Utils/Scenes/TestScenesCreator.h"

#include "Memory/ColorRGBA.h"
#include "IO/DirIO.h"
#include "LibImage.h"


using namespace bv;



class VideoAudioInputSyncTest : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO( VideoAudioInputSyncTest )

public:

    explicit VideoAudioInputSyncTest()
        : m_fakeVideoCard( nullptr )
        , m_testStartFrame( 30 )
    {}

public:

    virtual void        PreEvents           () override;
    virtual void        PreRender           () override;
    virtual void        PostRender          () override;


    void                Initialize          ();
    void                CheckOutput         ( SizeType frame );

    std::vector< UInt16 >       ChunkToVector           ( MemoryChunkConstPtr chunk );

protected:

    videocards::FakeVideoCardPtr        m_fakeVideoCard;

    UInt32          m_testStartFrame;

};
REGISTER_FRAMEWORK_GTEST_INFO( VideoAudioInputSyncTest, Engine_Audio, AudioVideoInputSync )




// ========================================================================= //
// Tests
// ========================================================================= //



// ***********************
//
void        VideoAudioInputSyncTest::Initialize            ()
{
    auto texturePlugin = TestScenesCreator::VideoInputRectangle( GetProjectEditor(), "FirstScene", 1920, 1080, 0 );

    m_fakeVideoCard = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 0 ) );
    auto inputCard = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 0 ) );

    ASSERT_NE( m_fakeVideoCard, nullptr );
    ASSERT_NE( inputCard, nullptr );

    inputCard->ResetInputFrame( 0 );
}

// ***********************
//
void        VideoAudioInputSyncTest::CheckOutput           ( SizeType )
{
    auto & outputs = m_fakeVideoCard->AccessOutputs();
    auto & avFrame = outputs.Outputs.begin()->second;

    auto videoChunk = avFrame->m_videoData;
    auto audioChunk = avFrame->m_audioData;
    
    // Check video sample
    ColorRGBA color = ColorRGBA::Load( videoChunk->Get() );

    // Check audio sample
    auto samplesVec = ChunkToVector( audioChunk );
    auto randomSample = samplesVec[ 100 ];
    
    Int32 framesDiff = ( ( Int32 )color.G() - ( Int32 )randomSample / 100 ) / 10;

    EXPECT_EQ( framesDiff, 0 ) << "Audio is delayed relative to video [" << Convert::T2String( framesDiff ) << "] frames.";
}

// ***********************
//
inline std::vector< UInt16 >            VideoAudioInputSyncTest::ChunkToVector      ( MemoryChunkConstPtr chunk )
{
    std::vector< UInt16 > samplesVec;
    samplesVec.resize( chunk->Size() / sizeof( UInt16 ) );

    memcpy( samplesVec.data(), chunk->Get(), chunk->Size() );

    return samplesVec;
}

// ***********************
//
void        VideoAudioInputSyncTest::PreEvents             ()
{
    if( GetFrameNumber() == 0 )
    {
        EndTestAfterThisFrame( false );
    }
    else if( GetFrameNumber() == m_testStartFrame )
    {
        Initialize();
    }
}

// ***********************
//
inline void VideoAudioInputSyncTest::PreRender  ()
{
    if( m_fakeVideoCard )
        m_fakeVideoCard->SetOutputSyncPoint();
}

// ***********************
//
void        VideoAudioInputSyncTest::PostRender            ()
{
    if( GetFrameNumber() == m_testStartFrame + 26 )
    {
        EndTestAfterThisFrame( true );
    }
    else if( GetFrameNumber() > m_testStartFrame )
    {
        CheckOutput( GetFrameNumber() );
    }
}





// ========================================================================= //
// Code to generate assets for this test.
// ========================================================================= //


//
//TEST( Produce_Audio, Audio )
//{
//    auto sampleSize = sizeof( UInt16 );
//    auto numChannels = 2;
//    auto frequency = 48000;
//    auto fps = 50;
//    auto numFrames = 25;
//    auto audioScale = 100;
//
//    auto samplesPerFrame = numChannels * frequency / fps;
//    auto audioBufferSize = numFrames * samplesPerFrame;
//
//    auto audioChunk = new Int16[ audioBufferSize ];
//
//    for( int frame = 0; frame < numFrames; frame++ )
//    {
//        Int16 sampleValue = static_cast< Int16 >( ( frame * 10 ) * audioScale );
//        auto audioBufferOffset = samplesPerFrame * frame;
//        
//        for( auto sample = 0; sample < samplesPerFrame; ++sample )
//        {
//            audioChunk[ audioBufferOffset + sample ] = sampleValue;
//        }
//    }
//
//    auto file = fopen( "AudioSync.raw", "w" );
//    if( file )
//    {
//        fwrite( audioChunk, sampleSize, audioBufferSize, file );
//        fclose( file );
//    }
//
//    delete[] audioChunk;
//
//    // Create images.
//
//    Path imagesDir = "VideoSync";
//    Dir::CreateDir( imagesDir.Str().c_str() );
//
//    auto videoChunk = MemoryChunk::Create( sizeof( UInt32 ) );
//
//    for( int frame = 0; frame < numFrames; frame++ )
//    {
//        auto sampleValue = 10 * frame;
//        ColorRGBA color( ( UInt8 )sampleValue, ( UInt8 )sampleValue, ( UInt8 )sampleValue, 255 );
//
//        color.Save( videoChunk->GetWritable() );
//
//        std::string frameStr = Convert::T2String( frame );
//        if( frameStr.size() == 1 )
//            frameStr = "0" + frameStr;
//
//        Path imagePath = imagesDir / ( "image" + frameStr + ".bmp" );
//        image::SaveBMPImage( imagePath.Str(), videoChunk, 1, 1, 32 );
//    }
//
//}



