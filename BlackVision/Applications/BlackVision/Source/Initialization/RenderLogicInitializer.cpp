#include "stdafxBVApp.h"

#include "RenderLogicInitializer.h"

#include "Engine/Graphics/Effects/Logic/RenderLogic.h"
#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderLogicDesc.h"


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

        do
        {
            auto id = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "id" ), 0 );
            auto enabled = SerializationHelper::String2T< bool >( deser.GetAttribute( "enabled" ), false );

            if( ( RenderChannelType ) id < RenderChannelType::RCT_TOTAL )
            {
                if( enabled )
                {
                    desc.SetEnabled ( ( RenderChannelType ) id );
                }
                else
                {
                    desc.SetDisabled( ( RenderChannelType ) id );
                }
            }


        }
        while( deser.NextChild() );

        deser.ExitChild(); // RenderChannel
    }
}

// *********************************
//
void            RenderLogicInitializer::Initialize      ( OutputLogicDesc & desc, const BVConfig & cfg )
{
    OutputDesc prvDesc;
    OutputDesc shmDesc;
    OutputDesc vidDesc;

    InitializeDefaultPrv( prvDesc, cfg );

    if( cfg.ReadbackFlag() )
    {
        InitializeDefaultShm( shmDesc, cfg );
        InitializeDefaultVid( vidDesc, cfg );
    }

    desc.AppendDesc( prvDesc );
    
    if( cfg.ReadbackFlag() )
    {
        desc.AppendDesc( shmDesc );
        desc.AppendDesc( vidDesc );
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
    // FIXME: nrl - implement it by reading cfg instance
    { cfg; }
    
    desc.SetWidth( 1920 );
    desc.SetHeight( 1080 );
    // Uncomment to make it BUG crash
    //desc.SetWidth( 1920 / 2 );
    //desc.SetHeight( 1080 / 2 );
    desc.SetOutputChannelMapping( OutputChannelMapping::OCM_RGBA );
    desc.SetRepresentedOutputType( CustomOutputType::COT_STREAM );
    desc.SetSelectedRenderedChannel( RenderChannelType::RCT_OUTPUT_1 );
    desc.SetEnabled( false );

    // FIXME: nrl - append additional properties if necessary
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
    
    auto & deser = cfg.GetNode( 3, "config", "videocards", "RenderChannels" );

    if( deser.EnterChild( "RenderChannel" ) )
    {
        do
        {
            auto rdID = deser.GetAttribute( "id" );
            if( deser.EnterChild( "Output" ) )
            {
                std::hash_map< std::string, std::string > prop;

                do
                {
                    prop[ "outputID" ] = deser.GetAttribute( "id" );
                    prop[ "width" ] = deser.GetAttribute( "width" );
                    prop[ "height" ] = deser.GetAttribute( "height" );
                    prop[ "renderChannelID" ] = rdID;

                    props.push_back( prop );
                }
                while( deser.NextChild() );

                deser.ExitChild(); // Output
            }
        }
        while( deser.NextChild() );

        deser.ExitChild(); // RenderChannel
    }
}


} // bv
