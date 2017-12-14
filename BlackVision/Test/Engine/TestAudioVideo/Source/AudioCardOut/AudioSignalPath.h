#pragma once

#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "Utils/Mocks/FakeVideoCard/FakeVideoCard.h"
#include "Utils/Mocks/Plugins/Audio/FakeAudioPlayerPlugin.h"
#include "Utils/Nodes/TestNodesCreator.h"


#include <algorithm>

#undef max

using namespace bv;


// ***********************
// This test sends generated sawtooth audio signal from fake plugin and reads results value from
// video card.
class AudioSignalPathTest : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO( AudioSignalPathTest )

public:

    explicit AudioSignalPathTest()
        : m_fakeAudio( nullptr )
        , m_fakeVideoCard( nullptr )
    {}

public:

    virtual void        PreEvents           () override;
    virtual void        PostRender          () override;

protected:

    model::FakeAudioPlayerPluginPtr     m_fakeAudio;
    videocards::FakeVideoCardPtr        m_fakeVideoCard;

};



// ========================================================================= //
// Helpers
// ========================================================================= //


// ***********************
//
inline MemoryChunkPtr      GenerateTestSignal  ( UInt32 frequency )
{
    const UInt32 samplesRate = 44000;
    UInt32 samplesToGen = samplesRate / frequency;
    UInt32 bytesPerSample = 2;

    std::vector< UInt16 > samplesVec;
    samplesVec.reserve( samplesToGen );

    for( UInt32 i = 0; i < samplesToGen; ++i )
    {
        UInt16 sample = static_cast< UInt16 >( 0.8 * std::numeric_limits< UInt16 >::max() * ( float )i / ( float )samplesToGen );
        samplesVec.push_back( sample );
    }

    auto chunk = MemoryChunk::Create( samplesToGen * bytesPerSample );
    memcpy( chunk->GetWritable(), samplesVec.data(), samplesToGen * bytesPerSample );

    return chunk;
}




// ========================================================================= //
// Tests
// ========================================================================= //



// ***********************
//
inline void            AudioSignalPathTest::PreEvents   ()
{
    if( GetFrameNumber() == 0 )
    {
        GetProjectEditor()->AddScene( "FirstScene" );

        auto scene = GetProjectEditor()->GetModelScene( "FirstScene" );
        auto audioNode = TestNodesCreator::FakeAudioPlugin( GetProjectEditor()->GetSceneDefaultTimeline( scene ), "AudioNode" );

        ASSERT_TRUE( GetProjectEditor()->AddChildNode( scene, scene->GetRootNode(), audioNode ) );

        m_fakeAudio = std::static_pointer_cast< model::FakeAudioPlayerPlugin >( audioNode->GetPlugin( "FakeAudioPlugin" ) );
        m_fakeVideoCard = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 0 ) );

        ASSERT_NE( m_fakeAudio, nullptr );
        ASSERT_NE( m_fakeVideoCard, nullptr );

        m_fakeAudio->SetSignalSource( GenerateTestSignal( 200 ) );

        EndTestAfterThisFrame( false );
    }
    else if( GetFrameNumber() == 110 )
    {
        EndTestAfterThisFrame( true );
    }
}



// ***********************
//
inline void            AudioSignalPathTest::PostRender   ()
{
    // Note: We wait 100 frames to empty queue from previous tests.
    if( GetFrameNumber() >= 100 )
    {
        auto & outputs = m_fakeVideoCard->AccessOutputs();
        auto & avFrame = outputs.Outputs.begin()->second;

        auto audioChunk = avFrame->m_audioData;
        auto mem = audioChunk->Get();

        std::vector< UInt16 > samplesVec;
        samplesVec.resize( audioChunk->Size() / sizeof( UInt16 ) );

        memcpy( samplesVec.data(), mem, audioChunk->Size() );

        // Find max sample value. Since we use sawtooth signal, max value will apear in each period.
        auto maxValIter = std::max_element( samplesVec.begin(), samplesVec.end() );
        ASSERT_TRUE( maxValIter != samplesVec.end() );

        auto maxVal = *maxValIter;

        // Find first zero sample, ommit everything before it. Ignoring first samples shouldn't cause any problem.
        // If there's big distortion in signal it should be visible in all samples.
        auto zeroIter = std::find( samplesVec.begin(), samplesVec.end(), 0 );
        auto maxIter = std::find( zeroIter, samplesVec.end(), maxVal );

        ASSERT_NE( zeroIter, samplesVec.end() );
        ASSERT_NE( maxIter, samplesVec.end() );

        maxIter++;

        UInt32 numIterations = 0;

        while( zeroIter != samplesVec.end() )
        {
            // Ignore last not complete part of signal.
            if( *std::max_element( zeroIter, maxIter ) != maxVal )
                break;

            auto length = std::distance( zeroIter, maxIter ) - 1;

            for( auto iter = zeroIter; iter != maxIter; ++iter )
            {
                auto i = std::distance( zeroIter, iter );

                Int32 actualSample = static_cast< Int32 >( *iter );
                Int32 expectedSample = static_cast< Int32 >( ( double )maxVal * ( double )i / ( double )length );

                EXPECT_LE( actualSample, ( expectedSample + 1 ) );
                EXPECT_GE( actualSample, ( expectedSample - 1 ) );
            }

            // Find new period ranges.
            zeroIter = std::find( maxIter, samplesVec.end(), 0 );
            maxIter = std::find( zeroIter, samplesVec.end(), maxVal );
            maxIter = maxIter != samplesVec.end() ? ++maxIter : maxIter;    // Include biggest sample.

            numIterations++;
        }


        // Check how many iteration are made. We expect that this number can be less then computed,
        // because we ommit samples from beginning and end.
        // Note: multiplication by 2 is from stereo channels. We ignored it while generating data.
        const UInt32 numSamplesPerFrame = 2 * 44000 / 50;
        const UInt32 expectedNumIter = numSamplesPerFrame / 200 - 1;

        EXPECT_GE( numIterations, expectedNumIter );
    }
}

