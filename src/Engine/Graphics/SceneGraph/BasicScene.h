#pragma once

#include "IScene.h"
#include "Mathematics/Interpolators/Interpolators.h"
#include "Engine/Models/IPlugin.h"
#include <map>

namespace bv {

class SceneNode;

class BasicScene : public IScene
{
    SceneNode*  m_treeSceneRoot;
    Camera*     m_camera;
public:

    virtual void	        initScene();
    virtual void	        update( float t );
    virtual void            render( Renderer* renderer );
    virtual void	        resize( int w, int h );

    virtual void            setCamera(Camera* cam);

    SceneNode*  getTreeSceneRoot();
    void        setTreeSceneRoot(SceneNode* sceneNode);

    BasicScene();
};


} // bv