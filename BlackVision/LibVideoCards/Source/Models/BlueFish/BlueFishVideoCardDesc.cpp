#include "BlueFishVideoCardDesc.h"
#include "BlueFishVideoCard.h"

#include "Serialization/ConversionHelper.h"

#include "Mathematics/Core/mathfuncs.h"

#include "UseLoggerVideoModule.h"



namespace bv {
namespace videocards {
namespace bluefish
{

// ***********************
//
std::string      VideoCardDesc::sUID = "BlueFish";


//**************************************
//
VideoCardDesc::VideoCardDesc    ()
{}

//**************************************
//
IVideoCardPtr           VideoCardDesc::CreateVideoCard          () const
{
    auto card = std::make_shared< VideoCard >( m_deviceID );

    if( card->AttachVideoCard() )
    {
        for( auto & channelDesc : m_channels )
        {
            ChannelInputDataUPtr input = nullptr;
            ChannelOutputDataUPtr output = nullptr;

            // Copy input and output data from descriptor.
            if( channelDesc.InputChannelData )
                input = std::unique_ptr< ChannelInputData >( new ChannelInputData( *( channelDesc.InputChannelData.get() ) ) );

            if( channelDesc.OutputChannelData )
                output = std::unique_ptr< ChannelOutputData >( new ChannelOutputData( *( channelDesc.OutputChannelData.get() ) ) );

            card->AddChannel( new Channel( channelDesc.Name, input, output ) );
        }

        card->InitVideoCard();
        return card;
    }

    return nullptr;
}

// ***********************
//
void                    VideoCardDesc::Deserialize          ( const IDeserializer & deser )
{
    m_deviceID = Convert::String2T< UInt32 >( deser.GetAttribute( "deviceID" ), 0 );


    //check input / output count
    if( deser.EnterChild( "channels" ) )
    {
        if( deser.EnterChild( "channel" ) )
        {
            do
            {
                ChannelInputDataUPtr input = nullptr;
                ChannelOutputDataUPtr output = nullptr;

                auto name = Convert::String2T< ChannelName >( deser.GetAttribute( "name" ) );

                if( deser.EnterChild( "input" ) )
                {
                    input = std::unique_ptr< ChannelInputData >( new ChannelInputData() );
                    input->type = Convert::String2T< IOType >( deser.GetAttribute( "type" ) );
                    input->resolution = Convert::String2T< UInt32 >( deser.GetAttribute( "resolution" ), 1080 );
                    input->playthrough = Convert::String2T< bool >( deser.GetAttribute( "playthrough" ), false );
                    input->linkedVideoInput = Convert::String2T< UInt32 >( deser.GetAttribute( "linkedVideoInput" ), 0 );

                    input->updateFormat = UPD_FMT_FIELD;
                    input->memoryFormat = MEM_FMT_BGRA;

                    deser.ExitChild(); //input
                }

                if( deser.EnterChild( "output" ) )
                {
                    output = std::unique_ptr< ChannelOutputData >( new ChannelOutputData() );
                    output->type = Convert::String2T< IOType >( deser.GetAttribute( "type" ) );
                    output->resolution = Convert::String2T< UInt32 >( deser.GetAttribute( "resolution" ), 1080 );
                    output->refresh = Convert::String2T< UInt32 >( deser.GetAttribute( "refresh" ), 5000 );
                    output->interlaced = Convert::String2T< bool >( deser.GetAttribute( "interlaced" ), false );
                    output->flipped = Convert::String2T< bool >( deser.GetAttribute( "flipped" ), false );
                    output->referenceMode = ReferenceModeMap[ Convert::String2T< ReferenceMode >( deser.GetAttribute( "referenceMode" ) ) ];
                    output->referenceH = Convert::String2T< Int32 >( deser.GetAttribute( "referenceH" ), 0 );
                    output->referenceV = Convert::String2T< Int32 >( deser.GetAttribute( "referenceV" ), 0 );
                    output->videoMode = ConvertVideoMode( output->resolution, output->refresh, output->interlaced );
                    output->linkedVideoOutput = ( UInt32 )Convert::String2T< UInt32 >( deser.GetAttribute( "linkedVideoOutput" ), 0 );

                    //FIXME?
                    output->updateFormat = UPD_FMT_FIELD;
                    output->memoryFormat = MEM_FMT_BGRA;
                    output->timecodePresented = true;
                    output->autoGeneratedTimecode = true;
                    //FIXME? END

                    deser.ExitChild(); //output
                }

                m_channels.push_back( ChannelDesc( name, input, output ) );

            } while( deser.NextChild() );

            deser.ExitChild(); //channel
        }

        deser.ExitChild(); //channels
    }

}

//**************************************
//
const std::string &     VideoCardDesc::GetVideoCardUID      () const
{
    return UID();
}


}   // bluefish
}   // videocards
}   // bv



