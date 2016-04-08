#include "stdafx.h"

#include "Scene.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/LightsLayout.h"
#include "Engine/Graphics/Resources/UniformBuffer.h"


namespace bv {


// ********************************
//
                    Scene::Scene                    ()
    : m_root( nullptr )
{
    auto lightsLayout = LightsLayout::Instance().GetBlockLayout();
    m_lightsBuffer =  new UniformBuffer( lightsLayout, DataBuffer::Semantic::S_DYNAMIC );
}

// ********************************
//
                    Scene::~Scene                   ()
{
    delete m_lightsBuffer;
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

// ********************************
//
UniformBuffer *     Scene::GetLightsBuffer          () const
{
    return m_lightsBuffer;
}

} //bv
