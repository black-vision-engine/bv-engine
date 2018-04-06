#pragma once

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"

#include "BlueFishUtils.h"
#include "Exceptions/ExceptionsList.h"

#include "Memory/AVFrame.h"
#include "Channel.h"


namespace bv {
namespace videocards {
namespace bluefish
{

DEFINE_PTR_TYPE( CBlueVelvet4 )


// ***********************
/// @ingroup BlueFishVideoCard
struct ChannelDesc
{
    ChannelName             Name;
    ChannelInputDataUPtr    InputChannelData;
    ChannelOutputDataUPtr   OutputChannelData;

// ***********************
//
    ChannelDesc( ChannelName name, ChannelInputDataUPtr & input, ChannelOutputDataUPtr & output )
        : Name( name )
        , InputChannelData( std::move( input ) )
        , OutputChannelData( std::move( output ) )
    {}
};



// ***************************** DESCRIPTOR **********************************
/// @ingroup BlueFishVideoCard
class VideoCardDesc : public IVideoCardDesc
{
private:

    static std::string      sUID;

private:

    UInt32                          m_deviceID;
    ReferenceMode                   m_referenceMode;
    std::vector< ChannelDesc >      m_channels;

public:

    VideoCardDesc           ();

    virtual Expected< IVideoCardPtr >       CreateVideoCard         () const override;
    virtual void                            Deserialize             ( const IDeserializer & deser ) override;

    virtual const std::string &             GetVideoCardUID         () const override;

    static const std::string &              UID                     () { return sUID; }

public:

    UInt32                              GetDeviceID     () const { return m_deviceID; }
    const std::vector< ChannelDesc > &  GetChannelsDescs() const { return m_channels; }


    ReturnResult                        Validate        () const;

public:

    static IVideoCardDescPtr            CreateDescriptor() { return std::make_shared< VideoCardDesc >(); }

private:

    ExceptionsListPtr                   ValidateUniqueChannelNames      () const;
    ExceptionsListPtr                   ValidateVideoMode               () const;
    ExceptionsListPtr                   ValidateReferenceMode           () const;
};

DEFINE_PTR_TYPE( VideoCardDesc )


}   // bluefish
}   // videocards
}   // bv
