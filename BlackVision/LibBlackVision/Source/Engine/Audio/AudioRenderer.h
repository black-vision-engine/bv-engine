#pragma once

#include <hash_map>

#include "Mathematics/glm_inc.h"
#include "Engine/Audio/Resources/AudioBuffer.h"


namespace bv { 
    
class AudioEntity;
    
namespace audio {

class PdrAudioBuffersQueue;
class PdrListener;
class PdrSource;


class AudioRenderer
{

private:

    typedef std::hash_map< const AudioEntity *, PdrSource * >       PdrSourceMap;
    typedef std::hash_map< PdrSource *, PdrAudioBuffersQueue * >    PdrAudioBufferMap;

    PdrSourceMap        m_sources;
    PdrAudioBufferMap   m_bufferMap;

public:

                        AudioRenderer       ();
                        ~AudioRenderer      ();

    void	            Initialize			();

    void	            Terminate			();

    void                Play                ( AudioEntity * audio );

};

} // audio
} // bv
