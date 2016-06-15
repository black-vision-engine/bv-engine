#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAudioChannel.h"

#include "DataTypes/Queue.h"


namespace bv { namespace model {

class DefaultAudioChannel;
DEFINE_PTR_TYPE(DefaultAudioChannel)

class DefaultAudioChannel : public IAudioChannel
{
protected:

    Int32                           m_frequency;
    AudioFormat                     m_format;

    Queue< MemoryChunkPtr >         m_packets;

public:

                                    DefaultAudioChannel     ( Int32 frequency, AudioFormat format );
    virtual                         ~DefaultAudioChannel    ();

    static  DefaultAudioChannelPtr  Create                  ( Int32 frequency, AudioFormat format );

    virtual void                    PushPacket              ( MemoryChunkPtr packet ) override;
    virtual MemoryChunkPtr          PopPacket               () override;

    virtual Int32                   GetFrequency            () const override;
    virtual AudioFormat             GetFormat               () const override;

};

} //model
} //bv
