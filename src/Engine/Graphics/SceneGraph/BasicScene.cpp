#include "BasicScene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Engine/Graphics/Shaders/Parameters.h"
#include "Engine/Graphics/Renderers/OGLrenderer/vborect.h"

#include <map>

namespace bv {

BasicScene::BasicScene()
    : m_treeSceneRoot(nullptr)
    , m_camera(nullptr)
{}

void BasicScene::initScene()
{
//    m_treeSceneRoot->init();
}

void BasicScene::update( float t )
{
//    m_treeSceneRoot->update(t);
}

void BasicScene::render( Renderer* renderer )
{
    assert(m_camera);
//    m_treeSceneRoot->render(renderer, *m_camera, Transform());
}

SceneNode* BasicScene::getTreeSceneRoot()
{
    return m_treeSceneRoot;
}

void BasicScene::setTreeSceneRoot(SceneNode* sceneNode)
{
    m_treeSceneRoot = sceneNode;
}

void BasicScene::resize( int w, int h )
{
    // TODO: Implement
}

void BasicScene::setCamera(Camera* cam)
{
    m_camera = cam;
}

} // bv