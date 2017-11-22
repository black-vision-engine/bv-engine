#pragma once

#include "Interfaces/IVideoCardDescriptor.h"

#include "FakeChannelsDescs.h"




namespace bv {
namespace videocards
{



// ***********************
//
struct FakeChannelDesc
{
    std::string                     Name;
    FakeInputChannelDataUPtr        InputChannelData;
    FakeOutputChannelDataUPtr       OutputChannelData;

    // ***********************
    //
    FakeChannelDesc( const std::string & name, FakeInputChannelDataUPtr & input, FakeOutputChannelDataUPtr & output )
        : Name( name )
        , InputChannelData( std::move( input ) )
        , OutputChannelData( std::move( output ) )
    {}
};



// ***********************
//
class FakeVideoCardDesc : public IVideoCardDesc
{
private:

    static std::string      sUID;

private:

    UInt32                          m_deviceID;
    std::vector< FakeChannelDesc >  m_channels;

public:

    virtual const std::string &     GetVideoCardUID     () const override;

    virtual void                    Deserialize         ( const IDeserializer & deser ) override;
    virtual IVideoCardPtr           CreateVideoCard     () const override;

    virtual                         ~FakeVideoCardDesc  () {}

public:

    static const std::string &      UID                     () { return sUID; }
    static IVideoCardDescPtr        CreateDescriptor        () { return std::make_shared< FakeVideoCardDesc >(); }

public:

    std::vector< FakeChannelDesc > &    AccessChannels      () { return m_channels; }

};


DEFINE_PTR_TYPE( FakeVideoCardDesc )

}   // videocards
}   // bv
