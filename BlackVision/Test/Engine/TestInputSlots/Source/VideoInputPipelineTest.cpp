#include "Framework/FrameworkTest.h"

#include "Framework/BVTestAppLogic.h"
#include "Engine/Editors/BVProjectEditor.h"


#include "Utils/Scenes/TestScenesCreator.h"
#include "Utils/Mocks/FakeVideoCard/FakeVideoCard.h"

#include "Assets/Input/TextureInputAssetDesc.h"

#include <random>


using namespace bv;


// ***********************
//
class Engine_InputSlots_VideoInputPipeline : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( Engine_InputSlots_VideoInputPipeline )
public:

    virtual void        PreEvents           () override;
    virtual void        PostRender          () override;

private:

    void                Initialize          ();
    void                CheckOutput         ( SizeType frame );
    glm::vec4           SamplePoint         ( const char * mem, SizeType posX, SizeType posY );

private:

    videocards::FakeVideoCardPtr        m_fakeVideoCard;

};
REGISTER_FRAMEWORK_GTEST_INFO( Engine_InputSlots_VideoInputPipeline, Engine_InputSlots, VideoInputPipeline )


// ========================================================================= //
// Random generator
// ========================================================================= //


std::random_device randomDevice;
std::mt19937 generator( randomDevice() );

std::uniform_int_distribution<> widthDist( 0, 1919 );
std::uniform_int_distribution<> heightDist( 0, 1079 );


// ========================================================================= //
// Tests
// ========================================================================= //



// ***********************
//
void        Engine_InputSlots_VideoInputPipeline::Initialize            ()
{
    auto texturePlugin = TestScenesCreator::TexturedRectangle( GetProjectEditor(), "FirstScene", 1920, 1080 );

    auto desc = TextureInputAssetDesc::Create( InputSlotBinding( 0 ) );
    ASSERT_TRUE( texturePlugin->LoadResource( desc ) );

    GetProjectEditor()->AddScene( "FirstScene" );

    m_fakeVideoCard = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 1 ) );
    m_fakeVideoCard->ResetInputFrame( 0 );

    ASSERT_NE( m_fakeVideoCard, nullptr );

    EndTestAfterThisFrame( false );
}

// ***********************
//
void        Engine_InputSlots_VideoInputPipeline::CheckOutput           ( SizeType frame )
{
    const auto numColors = 4;
    const auto numTests = 500;

    glm::vec4 expectedColors[ numColors ] =
    {
        glm::vec4( 237.0f / 255.0f, 28.0f / 255.0f, 36.0f / 255.0f, 1 ),
        glm::vec4( 255.0f / 255.0f, 127.0f / 255.0f, 39.0f / 255.0f, 1 ),
        glm::vec4( 181.0f / 255.0f, 230.0f / 255.0f, 29.0f / 255.0f, 1 ),
        glm::vec4( 63.0f / 255.0f, 72.0f / 255.0f, 204.0f / 255.0f, 1 ),
    };

    auto & outputs = m_fakeVideoCard->AccessOutputs();
    auto & avFrame = outputs.Outputs.begin()->second;

    auto videoChunk = avFrame->m_videoData;
    auto mem = videoChunk->Get();

    glm::vec4 expectedColor = expectedColors[ frame % numColors ];

    auto numErrors = 0;

    for( SizeType i = 0; i < numTests; ++i )
    {
        auto posX = widthDist( generator );
        auto posY = heightDist( generator );

        if( SamplePoint( mem, posX, posY ) != expectedColor )
            numErrors++;
    }

    EXPECT_EQ( numErrors, 0 ) << "Num not equal samples: " << numErrors;
}

// ***********************
//
glm::vec4   Engine_InputSlots_VideoInputPipeline::SamplePoint           ( const char * mem, SizeType posX, SizeType posY )
{
    auto pixelIdx = posX * 4 + posY * 4 * 1920;
    
    unsigned char pixelR = mem[ pixelIdx + 2 ];
    unsigned char pixelG = mem[ pixelIdx + 1 ];
    unsigned char pixelB = mem[ pixelIdx ];
    unsigned char pixelA = mem[ pixelIdx + 3 ];

    return glm::vec4( pixelR / 255.0f, pixelG / 255.0f, pixelB / 255.0f, pixelA / 255.0f );
}

// ***********************
//
void        Engine_InputSlots_VideoInputPipeline::PreEvents             ()
{
    if( GetFrameNumber() == 0 )
    {
        Initialize();
    }
}

// ***********************
//
void        Engine_InputSlots_VideoInputPipeline::PostRender            ()
{
    if( GetFrameNumber() == 10 )
    {
        EndTestAfterThisFrame( true );
    }
    else
    {
        CheckOutput( GetFrameNumber() );
    }
}






