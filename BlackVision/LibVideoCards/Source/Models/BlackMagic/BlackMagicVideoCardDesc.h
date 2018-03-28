#pragma once

#include "Interfaces/IVideoCardDescriptor.h"

#include "BlackMagicUtils.h"


namespace bv {
namespace videocards {
namespace blackmagic
{



// ***************************** DESCRIPTOR **********************************
/// @ingroup BlackMagicVideoCard
class VideoCardDesc : public IVideoCardDesc
{
private:

    static std::string      sUID;

private:

    UInt32                              m_deviceID;
    UInt64                              m_linkedVideoOutputID;      ///< @todo Why this isn't in ChannelOutputData??
    std::vector< ChannelOutputData >    m_channelOutput;

public:

    VideoCardDesc       ();

    virtual Expected< IVideoCardPtr >       CreateVideoCard     () const override;
    virtual void                            Deserialize         ( const IDeserializer & deser ) override;

    virtual const std::string &             GetVideoCardUID     () const override;

    static const std::string &              UID                 () { return sUID; }

public:

    static IVideoCardDescPtr                CreateDescriptor    () { return std::make_shared< VideoCardDesc >(); }
};

DEFINE_PTR_TYPE( VideoCardDesc )


}   // blackmagic
}   // videocards
}   // bv


