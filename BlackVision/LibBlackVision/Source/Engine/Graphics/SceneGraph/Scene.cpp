#include "stdafx.h"

#include "Scene.h"
#include "Engine/Graphics/SceneGraph/LightsLayout.h"
#include "Tools/Utils.h"


namespace bv {


// ********************************
//
                    Scene::Scene                    ()
{
    auto lightsLayout = LightsLayout< UniformBlockLayoutType::STD140 >::Instance().GetBlockLayout();
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
UniformBuffer *     Scene::GetLightsBuffer          ()
{
    return m_lightsBuffer;
}

// ********************************
//
void                Scene::Update                   ()
{
}

// ********************************
//
UInt32              Scene::GetMaxLightsNum          ()
{
    return LightsLayout< UniformBlockLayoutType::STD140 >::Instance().MaxLightsNum;
}


} //bv
