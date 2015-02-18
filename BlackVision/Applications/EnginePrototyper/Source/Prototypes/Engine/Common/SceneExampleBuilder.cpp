#include "SceneExampleBuilder.h"

#include "Prototypes/Engine/Common/SimpleNodeBuilder.h"
#include "Common/PrototyperCore.h"

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
void        SceneExampleBuilder::SetLocalTransform      ( SceneNode * node, const Transform & transform )
{
    auto transformable = node->GetTransformable();

    transformable->SetLocalTransform( transform );
}

// *****************************
//
void        SceneExampleBuilder::SetWorldTransform      ( SceneNode * node, const Transform & transform )
{
    std::vector<Transform> tr;
    tr.push_back( transform );

    node->Update( tr );
}

// *****************************
//
SceneNode * SceneExampleBuilder::Scene0     ()
{
    auto root = SimpleNodeBuilder::CreateRectNodeTexture( 1.f, 1.f, -1.f, "rsrcy/tex0.jpg" );
    auto n1 = SimpleNodeBuilder::CreateRectNodeSolidColor( .5f, .5f, -1.f );

    auto tr = Transform();
    tr.SetMatrix( glm::rotate( glm::mat4( 1.0f ), 10.5f, glm::vec3( 0.0f, 0.0f, -1.0f ) ) );
    SetLocalTransform( n1, tr );

    root->AddChildNode( n1 );

    SetWorldTransform( root, Transform() );

    return root;
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
