#include "stdafxBVApp.h"

#include "RenderLogicInitializer.h"

#include "Engine/Graphics/Effects/Logic/RenderLogic.h"
#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderLogicDesc.h"

#include "Engine/Graphics/InputSlots/Logic/InputLogic.h"
#include "Engine/Graphics/InputSlots/Logic/Handlers/VideoInputHandler.h"

#include "Assets/Input/Loaders/InputAssetLoader.h"
#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/Loaders/VideoInputAssetLoader.h"
#include "Assets/Input/Videoinput/VideoInputAssetDesc.h"
#include "Assets/Input/Videoinput/VideoInputTextureAssetDesc.h"
#include "Assets/Input/Videoinput/VideoInputAudioAssetDesc.h"
#include "Assets/Input/AudioInputAssetDesc.h"

#include "Services/BVServiceProvider.h"

#include "UseLoggerBVAppModule.h"


namespace bv { 

// *********************************
//
RenderLogic *   RenderLogicInitializer::CreateInstance ( const BVConfig & cfg )
{
    RenderLogicDesc desc;

    desc.SetMainWidth( 1920 );
    desc.SetMainHeight( 1080 );
    desc.SetNumTrackedRenderTargets( 2 );

    Initialize( desc.AccessRenderedChannelsDataDesc(), cfg );
    Initialize( desc.AccessOutputLogicDesc(), cfg );

    auto res = RenderLogic::Create( desc );

    return res;
}

// *********************************
// FIXME: nrl - implement
void            RenderLogicInitializer::Initialize      ( RenderedChannelsDataDesc & desc, const BVConfig & cfg )
{
    auto & deser = cfg.GetNode( 2, "config", "RenderChannels" );

    desc.SetNumTrackedRenderTargets( 2 );

    if( deser.EnterChild( "RenderChannel" ) )
    {
        std::hash_map< std::string, std::string > prop;
        std::set< UInt32 > processedChannels;

        do
        {
            Expected< UInt32 > expectedId = Convert::String2T< UInt32 >( deser.GetAttribute( "id" ) );
            auto enabled = Convert::String2T< bool >( deser.GetAttribute( "enabled" ), false );

            if( expectedId.IsValid() )
            {
                bool duplicated = processedChannels.find( expectedId.GetVal() ) != processedChannels.end();

                if( !duplicated )
                {
                    auto id = expectedId.GetVal();
                    processedChannels.insert( id );

                    if( ( RenderChannelType )id < RenderChannelType::RCT_TOTAL )
                    {
                        if( enabled )
                        {
                            desc.SetEnabled ( ( RenderChannelType )id );
                        }
                        else
                        {
                            desc.SetDisabled( ( RenderChannelType )id );
                        }
                    }
                }
                else
                {
                    LOG_MESSAGE( SeverityLevel::warning ) << "RenderChannel [" << expectedId.GetVal() << "] entry already existed in config and will be ignored.";
                }
            }
        }
        while( deser.NextChild() );

        deser.ExitChild(); // RenderChannel
    }

    // At least one chanel must be enabled.
    bool existEnabled = false;
    for( int i = 0; i < ( int )RenderChannelType::RCT_TOTAL; ++i )
    {
        if( desc.IsEnabled( ( RenderChannelType )i ) )
            existEnabled = true;
    }

    // If there're no enabled channels, we choose first by default.
    if( !existEnabled )
        desc.SetEnabled( RenderChannelType::RCT_OUTPUT_1 );
}

// *********************************
//
void            RenderLogicInitializer::Initialize      ( OutputLogicDesc & desc, const BVConfig & cfg )
{
    OutputDesc prvDesc;
    OutputDesc shmDesc;
    OutputDesc vidDesc;
    OutputDesc avFileDesc;

    InitializeDefaultPrv( prvDesc, cfg );

    if( cfg.ReadbackFlag() )
    {
        InitializeDefaultShm( shmDesc, cfg );
        InitializeDefaultVid( vidDesc, cfg );
        InitializeDefaultAVFile( avFileDesc, cfg );
    }

    desc.AppendDesc( prvDesc );
    
    if( cfg.ReadbackFlag() )
    {
        // Note and FIXME: initialization order matters. Check OutputLogic::GetOutput :(
        desc.AppendDesc( vidDesc );
        desc.AppendDesc( shmDesc );
        desc.AppendDesc( avFileDesc );
    }
}

// *********************************
//
void             RenderLogicInitializer::InitializeDefaultPrv( OutputDesc & desc, const BVConfig & cfg )
{
    // FIXME: nrl - implement it by reading cfg instance
    { cfg; }
    
    desc.SetWidth( 1920 );
    desc.SetHeight( 1080 );
    desc.SetOutputChannelMapping( OutputChannelMapping::OCM_RGBA );
    desc.SetRepresentedOutputType( CustomOutputType::COT_PREVIEW );
    desc.SetSelectedRenderedChannel( RenderChannelType::RCT_OUTPUT_1 );
    desc.SetEnabled( true );
    // FIXME: nrl - append additional properties if necessary
}

// *********************************
//
void             RenderLogicInitializer::InitializeDefaultShm( OutputDesc & desc, const BVConfig & cfg )
{
    auto width = cfg.SharedMemoryWidth();
    auto height = cfg.SharedMemoryHeight();
    auto name = cfg.SharedMemoryBufferName();

    desc.SetWidth( width );
    desc.SetHeight( height );

    std::hash_map< std::string, std::string > prop;
    prop[ "Name" ] = name;

    desc.AccessOutputProperties().push_back( prop );
    
    desc.SetOutputChannelMapping( OutputChannelMapping::OCM_RGBA );
    desc.SetRepresentedOutputType( CustomOutputType::COT_STREAM );
    desc.SetSelectedRenderedChannel( RenderChannelType::RCT_OUTPUT_1 );
    desc.SetEnabled( cfg.RenderToSharedMemory() );

    // FIXME: nrl - append additional properties if necessary
}

// *********************************
//
void             RenderLogicInitializer::InitializeDefaultAVFile( OutputDesc & desc, const BVConfig & cfg )
{
    // FIXME: nrl - implement it by reading cfg instance
    {
        cfg;
    }

    desc.SetWidth( 1920 );
    desc.SetHeight( 1080 );
    // Uncomment to make it BUG crash
    //desc.SetWidth( 1920 / 2 );
    //desc.SetHeight( 1080 / 2 );
    desc.SetOutputChannelMapping( OutputChannelMapping::OCM_RGBA );
    desc.SetRepresentedOutputType( CustomOutputType::COT_AV_FILE );
    desc.SetSelectedRenderedChannel( RenderChannelType::RCT_OUTPUT_1 );
    desc.SetEnabled( false );

    // FIXME: nrl - append additional properties if necessary
}

// ***********************
//
void            RenderLogicInitializer::InitializeVideoInput    ( InputLogic * inputLogic, videocards::VideoCardManager * videoCardManager )
{
    VideoInputHandlerPtr videoInputHandler = std::make_shared< VideoInputHandler >( videoCardManager, inputLogic->GetInputSlots() );

    inputLogic->AppendInputHandler( videoInputHandler );

    auto videoInputSlots = videoInputHandler->GetVideoInputSlots();

    AssetManager::GetInstance().RegisterLoader( VideoInputTextureAssetDesc::UID(), std::make_shared< VideoInputAssetLoader >( videoInputSlots ) );
    AssetManager::GetInstance().RegisterLoader( VideoInputAudioAssetDesc::UID(), std::make_shared< VideoInputAssetLoader >( videoInputSlots ) );
    AssetManager::GetInstance().RegisterLoader( VideoInputAssetDesc::UID(), std::make_shared< VideoInputAssetLoader >( videoInputSlots ) );
}

// ***********************
//
void            RenderLogicInitializer::InitializeInputSlots    ( RenderLogic * renderLogic, const BVConfig &, videocards::VideoCardManager * videoCardManager )
{
    auto inputLogic = renderLogic->GetInputLogic();
    auto inputSlots = inputLogic->GetInputSlots();

    AssetManager::GetInstance().RegisterLoader( TextureInputAssetDesc::UID(), std::make_shared< InputAssetLoader >( inputSlots ) );
    AssetManager::GetInstance().RegisterLoader( AudioInputAssetDesc::UID(), std::make_shared< InputAssetLoader >( inputSlots ) );

    InitializeVideoInput( inputLogic, videoCardManager );
}

// ***********************
//
videocards::VideoOutputChannelDesc*         FindOutputDescriptor    ( videocards::OutputChannelsDescsVec & descs, videocards::VideoOutputID outID )
{
    for( int i = 0; i < descs.size(); ++i )
    {
        if( descs[ i ].GetOutputID() == outID )
            return &descs[ i ];
    }

    return nullptr;
}


// *********************************
//
void             RenderLogicInitializer::InitializeDefaultVid( OutputDesc & desc, const BVConfig & cfg )
{   
    desc.SetWidth( 1920 );
    desc.SetHeight( 1080 );
    desc.SetOutputChannelMapping( OutputChannelMapping::OCM_RGBA );
    desc.SetRepresentedOutputType( CustomOutputType::COT_VIDEO );
    desc.SetSelectedRenderedChannel( RenderChannelType::RCT_OUTPUT_1 );
    desc.SetEnabled( true );

    // FIXME: nrl - append additional properties representing each separate video output
    // FIXME: nrl - based on the code from VideoCardManager::ReadConfig
    auto & props = desc.AccessOutputProperties();
    
    auto & deser = cfg.GetNode( 2, "config", "RenderChannels" );
    
    auto videoCardManager = BVServiceProvider::GetInstance().GetVideoCardManager();
    auto channelsDescs = videoCardManager->GetOutputChannelsDescs();


    // We need to ingore duplicated entries.
    std::set< std::string > processedChannels;

    if( deser.EnterChild( "RenderChannel" ) )
    {
        do
        {
            auto rdID = deser.GetAttribute( "id" );
            bool rcEnabled = Convert::String2T< bool >( deser.GetAttribute( "enabled" ), false );
            bool duplicated = processedChannels.find( rdID ) != processedChannels.end();

            if( !duplicated && deser.EnterChild( "VideoOutput" ) )
            {
                processedChannels.insert( rdID );

                // Ignore disabled render channels and don't create video outputs.
                if( rcEnabled )
                {
                    std::hash_map< std::string, std::string > prop;

                    do
                    {
                        auto outputIDString = deser.GetAttribute( "id" );
                        
                        prop[ "outputID" ] = outputIDString;
                        prop[ "renderChannelID" ] = rdID;

                        auto outputID = Convert::String2T< videocards::VideoOutputID >( outputIDString );
                        auto outDesc = FindOutputDescriptor( channelsDescs, outputID );
                        if( outDesc )
                        {
                            prop[ "width" ] = Convert::T2String( outDesc->GetDataDesc().width );
                            prop[ "height" ] = Convert::T2String( outDesc->GetDataDesc().height );

                            props.push_back( prop );
                        }
                        else
                        {
                            LOG_MESSAGE( SeverityLevel::warning ) << "There's no video card channel linked to VideoOutput " << outputIDString;
                        }

                    } while( deser.NextChild() );

                    deser.ExitChild(); // Output
                }
                else
                {
                    // Inform user that VideoOutput wasn't created.
                    LOG_MESSAGE( SeverityLevel::warning ) << "RenderChannel [" << rdID << "] is disabled. VideoOutputs are ignored.";
                }
            }
        }
        while( deser.NextChild() );

        deser.ExitChild(); // RenderChannel
    }
}


} // bv
