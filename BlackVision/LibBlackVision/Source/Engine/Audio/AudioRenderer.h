#pragma once

#include <hash_map>
#include <set>
#include "Mathematics/glm_inc.h"
#include "Engine/Audio/Resources/AudioBuffer.h"


namespace bv { namespace audio {

class PdrAudioBuffersQueue;
class PdrListener;
class PdrSource;
class AudioEntity;


class AudioRenderer
{
private:

    typedef std::hash_map< const AudioEntity *, PdrSource * >       PdrSourceMap;
    typedef std::hash_map< PdrSource *, PdrAudioBuffersQueue * >    PdrAudioBufferMap;
    
    typedef std::hash_map< const AudioEntity *, UInt32 >			AudioEntityUpdateIDMap;

private:

    PdrSourceMap            m_sources;
    PdrAudioBufferMap       m_bufferMap;

    AudioEntityUpdateIDMap  m_audioEntityUpdateIDMap;

    AudioFormat             m_format;
    Int32                   m_frequency;

	Float32					m_gain;

public:

                        AudioRenderer       ();
                        ~AudioRenderer      ();

    void	            Initialize			();

    void	            Terminate			();

    void                Proccess            ( AudioEntity * audio );

    void                Play                ( AudioEntity * audio );
    void                Pause               ( AudioEntity * audio );
    void                Stop                ( AudioEntity * audio );
    void                EndOfFile           ( AudioEntity * audio );

	void				SetGain				( Float32 gain );

    void                DeletePDR           ( const AudioEntity * audio );

    AudioBufferConstPtr GetBufferedData     ( MemoryChunkPtr data, const std::set< const audio::AudioEntity * > & audioEnts );

    UInt32              GetChannels         () const;
    Int32               GetFrequency        () const;
    UInt32              GetChannelDepth     () const;

private:

    PdrSource *             GetPdrSource                ( const AudioEntity * audio, bool autoCreate = true );
    PdrAudioBuffersQueue *  GetPdrAudioBuffersQueue     ( PdrSource * source, const AudioEntity * audio,  bool autoCreate = true );

    bool                    IsAnySourcePlaying          () const;
    bool                    IsAnyBufferReady            ( SizeType requestedBufferSize ) const;

private:

    template< typename MapType >
    void                    DeleteSinglePDR         ( MapType & resMap, typename MapType::key_type & key );

};

} // audio
} // bv
