#include "stdafx.h"

#include "Scene.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/LightsLayout.h"
#include "Engine/Graphics/SceneGraph/CameraLayout.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv {


// ********************************
//
                    Scene::Scene                    ()
    :   m_root( nullptr )
    ,   m_gridLines( nullptr )
    ,   m_gridLinesVisible( false )
    ,   m_camera( std::unique_ptr< Camera >( new Camera() ) )
{
    auto lightsLayout = LightsLayout::Instance().GetBlockLayout();
    m_lightsBuffer = std::unique_ptr< UniformBuffer >( new UniformBuffer( lightsLayout, DataBuffer::Semantic::S_DYNAMIC ) );

    auto cameraLayout = CameraLayout::Instance().GetBlockLayout();
    m_cameraBuffer = std::unique_ptr< UniformBuffer >( new UniformBuffer( cameraLayout, DataBuffer::Semantic::S_DYNAMIC ) );
}

// ********************************
//
                    Scene::~Scene                   ()
{
    delete m_gridLines;
}

// ********************************
//
SceneNode *         Scene::GetRoot                  () const
{
    return m_root;
}

// ********************************
//
void                Scene::SetRoot                  ( SceneNode * node )
{
    m_root = node;
}

// ***********************
//
UniformBuffer *     Scene::GetCameraBuffer         () const
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

// ***********************
//
RenderableEntity *  Scene::GetGridLines             () const
{
    return m_gridLines;
}

// ***********************
//
void                Scene::SetGridLinesRenderable  ( RenderableEntity * renderable )
{
    m_gridLines = renderable;
}

// ***********************
//
void                Scene::SetGridLinesVisible ( bool visibility )
{
    m_gridLinesVisible = visibility;
}

} //bv
