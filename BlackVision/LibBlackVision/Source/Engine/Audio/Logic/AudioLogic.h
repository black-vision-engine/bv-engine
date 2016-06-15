#pragma once

#include <hash_map>

#include "Mathematics/glm_inc.h"
#include "Engine/Audio/Resources/AudioBuffer.h"
#include "Engine/Graphics/SceneGraph/Scene.h"


namespace bv { namespace audio {


class AudioRenderer;


class AudioLogic
{

public:

                        AudioLogic          ();
                        ~AudioLogic         ();

    void	            Play			    ( AudioRenderer * renderer, const SceneVec & scenes );
    void                Play                ( AudioRenderer * renderer, SceneNode * node );

};

} // audio
} // bv
