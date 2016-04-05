#pragma once

#include "Engine/Graphics/Resources/UniformBuffer.h"

#include "CoreDEF.h"


namespace bv {

class Renderer;


class Scene
{
private:

    UniformBuffer *         m_lightsBuffer;

public:

                            Scene               ();
                            ~Scene              ();

public:

    void                    Update              ();

    UniformBuffer *         GetLightsBuffer     ();
    static UInt32           GetMaxLightsNum     ();

};

DEFINE_PTR_TYPE( Scene );
DEFINE_CONST_PTR_TYPE( Scene );

typedef std::vector< Scene * > SceneVec;

} // bv
