#include "SceneExampleBuilder.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv {

// *****************************
//
SceneNode *  SceneExampleBuilder::BuildScene( unsigned int sceneNum )
{
    switch ( sceneNum )
    {
        case 0:
            return Scene0();
            break;
        default:
            return Scene0();
    }
}

// *****************************
//
SceneNode *  SceneExampleBuilder::Scene0    ()
{
    return nullptr;
}

} // bv
