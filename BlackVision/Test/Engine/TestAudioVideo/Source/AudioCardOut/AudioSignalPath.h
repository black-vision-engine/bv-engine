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
#undef min

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
        , m_numTestFramesDuration( 10 )
        , m_numCleanBuffersFrames( 100 )
    {}

    explicit AudioSignalPathTest( UInt16 testFramesDuration )
        : m_fakeAudio( nullptr )
        , m_fakeVideoCard( nullptr )
        , m_numTestFramesDuration( testFramesDuration )
        , m_numCleanBuffersFrames( 100 )
    {}

public:

    virtual void        PreEvents           () override;
    virtual void        PostRender          () override;

protected:

    model::FakeAudioPlayerPluginPtr     m_fakeAudio;
    videocards::FakeVideoCardPtr        m_fakeVideoCard;

    MemoryChunkPtr                      m_referenceSignal;
    std::vector< UInt16 >               m_outputSignal;

    UInt16              m_numTestFramesDuration;
    UInt16              m_numCleanBuffersFrames;

protected:

    virtual void                AggregateOutputSignal   ( AVFrameConstPtr frame );
    virtual MemoryChunkPtr      GenerateTestSignal      ( UInt32 frequency );
    MemoryChunkPtr              GenerateEmptySignal     ();

    SizeType                    CompareSignals          ();
    void                        ValidateNumSamples      ();

    std::vector< UInt16 >       ChunkToVector           ( MemoryChunkPtr chunk );
};



// ========================================================================= //
// Helpers
// ========================================================================= //


const UInt32 samplesRate = 48000;


// ***********************
//
inline MemoryChunkPtr      AudioSignalPathTest::GenerateTestSignal  ( UInt32 frequency )
{
    UInt32 samplesToGen = samplesRate / frequency;
    UInt32 bytesPerSample = 2;

    std::vector< UInt16 > samplesVec;
    samplesVec.reserve( samplesToGen );

    for( UInt32 i = 0; i < samplesToGen; ++i )
    {
        UInt16 sample = static_cast< UInt16 >( 0.8 * std::numeric_limits< UInt16 >::max() * ( float )i / ( float )samplesToGen );
        
        // Generate 2 channels stereo with the same data.
        samplesVec.push_back( sample );
        samplesVec.push_back( sample );
    }

    auto chunk = MemoryChunk::Create( samplesToGen * bytesPerSample );
    memcpy( chunk->GetWritable(), samplesVec.data(), samplesToGen * bytesPerSample );

    return chunk;
}

// ***********************
//

inline MemoryChunkPtr   AudioSignalPathTest::GenerateEmptySignal    ()
{
    UInt32 samplesToGen = samplesRate / 50;
    UInt32 bytesPerSample = 2;

    return MemoryChunk::Create( samplesToGen * bytesPerSample );
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

        m_referenceSignal = GenerateTestSignal( 200 );
        m_fakeAudio->SetSignalSource( GenerateEmptySignal() );

        EndTestAfterThisFrame( false );
    }
    else if( GetFrameNumber() == 1 )
    {
        // First we must insert zero signal, after one frame we can send nullptr chunks to audio system.
        m_fakeAudio->SetSignalSource( nullptr );
    }
    else if( GetFrameNumber() == m_numCleanBuffersFrames )
    {
        m_fakeAudio->SetSignalSource( m_referenceSignal );
    }
    else if( GetFrameNumber() == m_numCleanBuffersFrames + m_numTestFramesDuration )
    {
        EndTestAfterThisFrame( true );
    }
}



// ***********************
//
inline void            AudioSignalPathTest::PostRender   ()
{
    // Note: We wait m_numCleanBuffersFrames frames to empty queue from previous tests.

    if( GetFrameNumber() >= m_numCleanBuffersFrames && GetFrameNumber() < m_numCleanBuffersFrames + m_numTestFramesDuration )
    {
        auto & outputs = m_fakeVideoCard->AccessOutputs();
        auto & avFrame = outputs.Outputs.begin()->second;

        AggregateOutputSignal( avFrame );
    }

    if( GetFrameNumber() == m_numCleanBuffersFrames + m_numTestFramesDuration )
    {
        EXPECT_EQ( CompareSignals(), 0 );
    }
}

// ***********************
//
inline void         AudioSignalPathTest::AggregateOutputSignal      ( AVFrameConstPtr frame )
{
    auto audioChunk = frame->m_audioData;
    auto mem = audioChunk->Get();

    auto outSize = m_outputSignal.size();
    m_outputSignal.resize( outSize + audioChunk->Size() / sizeof( UInt16 ) );

    memcpy( m_outputSignal.data() + outSize, mem, audioChunk->Size() );
}


// ***********************
//
inline SizeType         AudioSignalPathTest::CompareSignals     ()
{
    std::vector< UInt16 > reference = ChunkToVector( m_referenceSignal );

    ValidateNumSamples();


    SizeType j = 0;
    SizeType numErrors = 0;

    // Find first non zero sample.
    for( SizeType i = 0; i < m_outputSignal.size(); i++ )
    {
        if( m_outputSignal[ i ] > 0 )
        {
            EXPECT_GE( i, 1 );

            // Start with zer osample just before first non-zero sample. @Note We have stereo output.
            j = i - 2;
            break;
        }
    }

    // First sample should be in range of first frame.
    const UInt32 numSamplesPerFrame = 2 * samplesRate / 50;
    EXPECT_LE( j, numSamplesPerFrame );

    while( j < m_outputSignal.size() )
    {
        auto maxLoop = std::min( reference.size(), m_outputSignal.size() - j );

        for( SizeType i = 0; i < maxLoop; ++i )
        {
            auto outValue = m_outputSignal[ j + i ];
            auto refValue = reference[ i ];

            if( outValue != refValue )
                numErrors++;
        }

        j += reference.size();
    }


    return numErrors;
}

// ***********************
//
inline void                             AudioSignalPathTest::ValidateNumSamples ()
{
    const UInt32 numSamplesPerFrame = 2 * samplesRate / 50;
    auto expectedNumSamples = numSamplesPerFrame * m_numTestFramesDuration;

    EXPECT_EQ( m_outputSignal.size(), expectedNumSamples );
}

// ***********************
//
inline std::vector< UInt16 >            AudioSignalPathTest::ChunkToVector      ( MemoryChunkPtr chunk )
{
    std::vector< UInt16 > samplesVec;
    samplesVec.resize( chunk->Size() / sizeof( UInt16 ) );

    memcpy( samplesVec.data(), chunk->Get(), chunk->Size() );

    return samplesVec;
}



