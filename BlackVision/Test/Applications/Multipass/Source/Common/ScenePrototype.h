#pragma once

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Common/Transformations.h"
#include "Common/SimpleNodeBuilder.h"
#include "Common/MultipassNodeBuilder.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace  bv {

class ScenePrototype
{
protected:

    SceneNode *     m_root;
    Renderer *      m_renderer;

public:

    static  ScenePrototype *    CreateNewPrototype  ( unsigned int i, Renderer * renderer );

    static  void                SetLocalTransform   ( SceneNode * node, const Transform & transform );
    static  void                SetWorldTransform   ( SceneNode * node, const Transform & transform );
    static  void                SetPixelShaderParam ( SceneNode * node, const std::string & name, const glm::vec4 & param );
                            
                                ScenePrototype      ( Renderer * renderer );
    virtual                     ~ScenePrototype     ();

    virtual void                Update              ( TimeType t );
    virtual void                OnKey               ( unsigned char c );

            void                BuildScene          ();

            SceneNode *         GetRootNode         ();
            void                SetRootNode         ( SceneNode * node );
            
            Renderer *          GetRenderer         ();

            void                SetWorldTransform   ( const Transform & transform );

private:

    virtual SceneNode *         BuildSceneImpl      () = 0;

};

} //bv
