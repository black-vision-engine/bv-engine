#include "stdafx.h"

#include "Scene.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/LightsLayout.h"
#include "Engine/Graphics/SceneGraph/CameraLayout.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv {


// ********************************
//
                    Scene::Scene                    ( unsigned int outputIdx )
    : m_root( nullptr )
    , m_camera( std::unique_ptr< Camera >( new Camera() ) )
    , m_outputChannelIdx( outputIdx )
{
    auto lightsLayout = LightsLayout::Instance().GetBlockLayout();
    m_lightsBuffer = std::unique_ptr< UniformBuffer >( new UniformBuffer( lightsLayout, DataBuffer::Semantic::S_DYNAMIC ) );

    auto cameraLayout = CameraLayout::Instance().GetBlockLayout();
    m_cameraBuffer = std::unique_ptr< UniformBuffer >( new UniformBuffer( cameraLayout, DataBuffer::Semantic::S_DYNAMIC ) );
}

// ********************************
//
                    Scene::~Scene                   ()
{}

// ********************************
//
SceneNode *         Scene::GetRoot              () const
{
    return m_root;
}

// ********************************
//
void                Scene::SetRoot              ( SceneNode * node )
{
    m_root = node;
}

// ***********************
//
void            Scene::SetOutputChannelIdx      ( unsigned int channelIdx )
{
    m_outputChannelIdx = channelIdx;
}

// ***********************
//
unsigned int    Scene::GetOutputChannelIdx      () const
{
    return m_outputChannelIdx;
}

// ***********************
//
UniformBuffer *     Scene::GetCameraBuffer      () const
{
    return m_cameraBuffer.get();
}

// ********************************
//
UniformBuffer *     Scene::GetLightsBuffer          () const
{
    return m_lightsBuffer.get();
}

// ***********************
//
Camera *            Scene::GetCamera                () const
{
    return m_camera.get();
}


} //bv
