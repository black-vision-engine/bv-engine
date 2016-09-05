#pragma once

#include <hash_map>

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

public:

                        AudioRenderer       ();
                        ~AudioRenderer      ();

    void	            Initialize			();

    void	            Terminate			();

    void                Play                ( AudioEntity * audio );

    void                Pause               ( AudioEntity * audio );
    void                Stop                ( AudioEntity * audio );


    void                DeletePDR                   ( const AudioEntity * audio );

private:

    PdrSource *             GetPdrSource                ( const AudioEntity * audio, bool autoCreate = true );
    PdrAudioBuffersQueue *  GetPdrAudioBuffersQueue     ( PdrSource * source, const AudioEntity * audio,  bool autoCreate = true );

private:

    template< typename MapType >
    void                    DeleteSinglePDR         ( MapType & resMap, typename MapType::key_type & key );

};

} // audio
} // bv
