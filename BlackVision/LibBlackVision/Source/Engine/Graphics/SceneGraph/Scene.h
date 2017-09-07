#pragma once

#include "Engine/Graphics/Resources/UniformBuffer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

class Renderer;
class SceneNode;
class RenderableEntity;

class Scene
{
private:

    SceneNode *                     m_root;

    UniformBufferUPtr               m_lightsBuffer;
    UniformBufferUPtr               m_cameraBuffer;
    CameraUPtr                      m_camera;

    //FIXME: channel can be set here, so it's pretty close to full scene->channel mapping
    //FIXME: nrl - output channel logic has to be specified now
    unsigned int                    m_outputChannelIdx;

public:

                            Scene                   ( unsigned int channelIdx = 0 );
                            ~Scene                  ();

public:

    SceneNode *             GetRoot                 () const;
    void                    SetRoot                 ( SceneNode * node );

    void                    SetOutputChannelIdx     ( unsigned int channelIdx );
    unsigned int            GetOutputChannelIdx     () const;

    UniformBuffer *         GetCameraBuffer         () const;
    UniformBuffer *         GetLightsBuffer         () const;
    Camera *                GetCamera               () const;
};

DEFINE_PTR_TYPE( Scene );
DEFINE_UPTR_TYPE( Scene );
DEFINE_CONST_PTR_TYPE( Scene );

typedef std::vector< Scene * > SceneVec;
typedef std::vector< SceneUPtr > SceneUPtrVec;

} // bv
