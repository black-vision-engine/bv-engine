#include "SceneExampleBuilder.h"

#include "Prototypes/Engine/Common/SimpleNodeBuilder.h"


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
        case 1:
            return Scene1();
            break;
        case 2:
            return Scene2();
            break;
        default:
            return Scene0();
    }
}

// *****************************
//
SceneNode * SceneExampleBuilder::Scene0     ()
{
    return SimpleNodeBuilder::CreateRectNodeTexture( 1.f, 1.f, -1.f, "rsrcy/tex0.jpg" );
    // return SimpleNodeBuilder::CreateRectNodeSolidColor( 1.f, 1.f, -1.f );
}

// *****************************
//
SceneNode * SceneExampleBuilder::Scene1     ()
{
    return Scene0();
}

// *****************************
//
SceneNode * SceneExampleBuilder::Scene2     ()
{
    return Scene0();
}

} // bv
