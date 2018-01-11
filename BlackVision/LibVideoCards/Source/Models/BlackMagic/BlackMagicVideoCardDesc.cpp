#include "BlackMagicVideoCardDesc.h"
#include "BlackMagicVideoCard.h"

#include "Serialization/ConversionHelper.h"


namespace bv {
namespace videocards {
namespace blackmagic
{

// ***********************
//
std::string      VideoCardDesc::sUID = "BlackMagic";



//**************************************
//
VideoCardDesc::VideoCardDesc()
{}

//**************************************
//
IVideoCardPtr           VideoCardDesc::CreateVideoCard  () const
{
    if( VideoCard::AvailableVideoCards > 0 )
    {
        auto card = std::make_shared< VideoCard >( m_deviceID );

        for( auto & output : m_channelOutput )
        {
            // @todo Handle multiple video outputs.
            card->m_linkedVideoOutputID = m_linkedVideoOutputID;
            card->AddOutput( output );
        }

        bool result = card->InitOutput();
        {result; }

        VideoCard::AvailableVideoCards--;

        return card;
    }

    return nullptr;
}

// ***********************
//
void                    VideoCardDesc::Deserialize          ( const IDeserializer & deser )
{
    m_deviceID = Convert::String2T< UInt32 >( deser.GetAttribute( "deviceID" ), 0 );

    if( deser.EnterChild( "channels" ) )
    {
        if( deser.EnterChild( "channel" ) )
        {
            do
            {
                if( deser.EnterChild( "output" ) )
                {
                    ChannelOutputData output;
                    output.type = Convert::String2T< IOType >( deser.GetAttribute( "type" ) );
                    output.resolution = Convert::String2T< UInt32 >( deser.GetAttribute( "resolution" ), 1080 );
                    output.refresh = Convert::String2T< UInt32 >( deser.GetAttribute( "refresh" ), 5000 );
                    output.interlaced = Convert::String2T< bool >( deser.GetAttribute( "interlaced" ), false );
                    output.flipped = Convert::String2T< bool >( deser.GetAttribute( "flipped" ), false );
                    output.videoMode = ConvertVideoMode( output.resolution, output.refresh, output.interlaced );

                    m_linkedVideoOutputID = Convert::String2T< UInt32 >( deser.GetAttribute( "linkedVideoOutput" ) );

                    deser.ExitChild(); //output

                    m_channelOutput.push_back( output );
                }
            } while( deser.NextChild() );

            deser.ExitChild(); //channel
        }

        deser.ExitChild(); //channels
    }
}

//**************************************
//
const std::string &     VideoCardDesc::GetVideoCardUID  () const
{
    return UID();
}


}   // blackmagic
}   // videocards
}   // bv




