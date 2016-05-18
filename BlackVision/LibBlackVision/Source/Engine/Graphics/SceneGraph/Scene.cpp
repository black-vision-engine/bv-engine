#include "stdafx.h"

#include "Scene.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/LightsLayout.h"
#include "Engine/Graphics/SceneGraph/CameraLayout.h"
#include "Engine/Graphics/Resources/UniformBuffer.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

namespace bv {


// ********************************
//
                    Scene::Scene                    ()
    :   m_root( nullptr )
    ,   m_gridLines( nullptr )
    ,   m_gridLinesVisible( false )
    ,   m_camera( new Camera() )
{
    auto lightsLayout = LightsLayout::Instance().GetBlockLayout();
    m_lightsBuffer =  new UniformBuffer( lightsLayout, DataBuffer::Semantic::S_DYNAMIC );

    auto cameraLayout = CameraLayout::Instance().GetBlockLayout();
    m_cameraBuffer = new UniformBuffer( cameraLayout, DataBuffer::Semantic::S_DYNAMIC );
}

// ********************************
//
                    Scene::~Scene                   ()
{
    delete m_lightsBuffer;
    delete m_gridLines;
    delete m_camera;
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
    return m_cameraBuffer;
}

// ********************************
//
UniformBuffer *     Scene::GetLightsBuffer          () const
{
    return m_lightsBuffer;
}

// ***********************
//
Camera *            Scene::GetCamera                () const
{
    return m_camera;
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
