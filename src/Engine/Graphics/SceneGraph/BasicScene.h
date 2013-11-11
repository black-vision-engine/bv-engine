#pragma once

#include <map>

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Graphics/SceneGraph/IScene.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"


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