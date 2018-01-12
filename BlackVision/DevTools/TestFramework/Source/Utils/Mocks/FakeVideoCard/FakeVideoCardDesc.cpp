#include "FakeVideoCardDesc.h"

#include "FakeVideoCard.h"

#include "Serialization/ConversionHelper.h"



namespace bv {
namespace videocards
{

// ***********************
//
std::string      FakeVideoCardDesc::sUID = "FakeVideoCard";


// ***********************
//
const std::string &             FakeVideoCardDesc::GetVideoCardUID      () const
{
    return UID();
}

// ***********************
//
void                            FakeVideoCardDesc::Deserialize          ( const IDeserializer & deser )
{
    m_deviceID = Convert::String2T< UInt32 >( deser.GetAttribute( "deviceID" ), 0 );


    //check input / output count
    if( deser.EnterChild( "channels" ) )
    {
        if( deser.EnterChild( "channel" ) )
        {
            do
            {
                FakeInputChannelDataUPtr input = nullptr;
                FakeOutputChannelDataUPtr output = nullptr;

                auto name = deser.GetAttribute( "name" );

                if( deser.EnterChild( "input" ) )
                {
                    input = std::unique_ptr< FakeInputChannelData >( new FakeInputChannelData() );

                    input->Height = Convert::String2T< UInt32 >( deser.GetAttribute( "height" ), 1080 );
                    input->Width = Convert::String2T< UInt32 >( deser.GetAttribute( "width" ), 1920 );
                    input->LinkedVideoInput = Convert::String2T< UInt32 >( deser.GetAttribute( "linkedVideoInput" ), 0 );
                    input->ImagesDirectory = deser.GetAttribute( "imagesDirectory" );

                    deser.ExitChild(); //input
                }

                if( deser.EnterChild( "output" ) )
                {
                    output = std::unique_ptr< FakeOutputChannelData >( new FakeOutputChannelData() );

                    output->Height = Convert::String2T< UInt32 >( deser.GetAttribute( "height" ), 1080 );
                    output->Width = Convert::String2T< UInt32 >( deser.GetAttribute( "width" ), 1920 );
                    output->LinkedVideoOutput = ( UInt32 )Convert::String2T< UInt32 >( deser.GetAttribute( "linkedVideoOutput" ), 0 );

                    deser.ExitChild(); //output
                }

                m_channels.push_back( FakeChannelDesc( name, input, output ) );

            } while( deser.NextChild() );

            deser.ExitChild(); //channel
        }

        deser.ExitChild(); //channels
    }
}

// ***********************
//
IVideoCardPtr                   FakeVideoCardDesc::CreateVideoCard      () const
{
    auto card = std::make_shared< FakeVideoCard >( m_deviceID );

    for( auto & channelDesc : m_channels )
    {
        FakeInputChannelDataUPtr input = nullptr;
        FakeOutputChannelDataUPtr output = nullptr;

        // Copy input and output data from descriptor.
        if( channelDesc.InputChannelData )
            input = std::unique_ptr< FakeInputChannelData >( new FakeInputChannelData( *( channelDesc.InputChannelData.get() ) ) );

        if( channelDesc.OutputChannelData )
            output = std::unique_ptr< FakeOutputChannelData >( new FakeOutputChannelData( *( channelDesc.OutputChannelData.get() ) ) );

        FakeChannelDesc newDesc( channelDesc.Name, input, output );
        card->AddChannel( newDesc );
    }

    return card;
}



}   // videocards
}   // bv

