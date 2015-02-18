#pragma once

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Prototypes/Engine/Common/Transformations.h"
#include "Prototypes/Engine/Common/SimpleNodeBuilder.h"


namespace  bv {

class ScenePrototype
{
protected:

    SceneNode *     m_root;

public:

    static  ScenePrototype *    CreateNewPrototype  ( unsigned int i );

    static  void                SetLocalTransform   ( SceneNode * node, const Transform & transform );
    static  void                SetWorldTransform   ( SceneNode * node, const Transform & transform );
    static  void                SetPixelShaderParam ( SceneNode * node, const std::string & name, const glm::vec4 & param );
                            
                                ScenePrototype      ();
    virtual                     ~ScenePrototype     ();

    virtual void                Update              ( TimeType t );
    virtual void                OnKey               ( unsigned char c );

            void                BuildScene          ();
            SceneNode *         GetRootNode         ();

            void                SetWorldTransform   ( const Transform & transform );

private:

    virtual SceneNode *         BuildSceneImpl      () = 0;

};

} //bv
