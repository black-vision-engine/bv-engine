#pragma once

#include "Engine/Graphics/Resources/UniformBuffer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "CoreDEF.h"


namespace bv {

class Renderer;


class Scene
{
private:

    SceneNode *             m_root;

    UniformBuffer *         m_lightsBuffer;

public:

                            Scene               ();
                            ~Scene              ();

public:

    SceneNode *             GetRoot             () const;
    void                    SetRoot             ( SceneNode * node );

    UniformBuffer *         GetLightsBuffer     () const;
    static UInt32           GetMaxLightsNum     ();

};

DEFINE_PTR_TYPE( Scene );
DEFINE_CONST_PTR_TYPE( Scene );

typedef std::vector< Scene * > SceneVec;

} // bv
