#include "stdafxBVApp.h"

#include "RenderLogicInitializer.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/NRenderLogicDesc.h"


namespace bv { namespace nrl {

// *********************************
//
NRenderLogic *   RenderLogicInitializer::CreateInstance ( const BVConfig & cfg )
{
    NRenderLogicDesc desc;

    desc.SetMainWidth( 1920 );
    desc.SetMainHeight( 1080 );
    desc.SetNumTrackedRenderTargets( 2 );

    Initialize( desc.AccessRenderedChannelsDataDesc(), cfg );
    Initialize( desc.AccessOutputLogicDesc(), cfg );

    auto res = NRenderLogic::Create( desc );

    return res;
}

// *********************************
// FIXME: nrl - implement
void            RenderLogicInitializer::Initialize      ( RenderedChannelsDataDesc & desc, const BVConfig & cfg )
{
    { cfg; }

    desc.SetNumTrackedRenderTargets( 2 );

    desc.SetEnabled ( RenderChannelType::RCT_OUTPUT_1 );
    desc.SetDisabled( RenderChannelType::RCT_OUTPUT_2 );
    desc.SetDisabled( RenderChannelType::RCT_OUTPUT_3 );
    desc.SetDisabled( RenderChannelType::RCT_OUTPUT_4 );
}

// *********************************
//
void            RenderLogicInitializer::Initialize      ( OutputLogicDesc & desc, const BVConfig & cfg )
{
    OutputDesc prvDesc;
    OutputDesc shmDesc;
    OutputDesc vidDesc;

    InitializeDefaultPrv( prvDesc, cfg );
    InitializeDefaultShm( shmDesc, cfg );
    InitializeDefaultVid( vidDesc, cfg );

    desc.AppendDesc( prvDesc );
    desc.AppendDesc( shmDesc );
    desc.AppendDesc( vidDesc );
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
    desc.SetEnabled( true );

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

} // nrl
} // bv
