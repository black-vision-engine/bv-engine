#pragma once

#include "BVAL.h"

#include "Engine/Audio/Resources/AudioBuffer.h"


namespace bv { namespace audio {


class PdrSource
{
private:

    ALuint m_sourceHandle;

public:

            PdrSource           ();
            ~PdrSource          ();

    ALuint  GetHandle           () const;

    void    Play                ();
    void    Pause               ();
    void    Stop                ();
    void    Rewind              ();

    bool    IsPlaying           () const;
    bool    IsPaused            () const;
    bool    IsStopped           () const;

};

} // audio
} // bv