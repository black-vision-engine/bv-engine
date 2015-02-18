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
void            SceneExampleBuilder::UpdateScene    ( unsigned int sceneNum, SceneNode * root, float t )
{
    switch ( sceneNum )
    {
        case 0:
            UpdateScene0( root, t );
            break;
        case 1:
            UpdateScene1( root, t );
            break;
        case 2:
            UpdateScene2( root, t );
            break;
        default:
            assert( false );
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
    auto root = SimpleNodeBuilder::CreateRectNodeTexture( 1.f, 1.f, -1.1f, "rsrcy/tex0.jpg" );
    auto n1 = SimpleNodeBuilder::CreateRectNodeSolidColor( .5f, .5f, -1.2f );
    auto n11 = SimpleNodeBuilder::CreateRectNodeTexture( .25f, .25f, -1.3f, "rsrcy/tex1.jpg" );

    root->AddChildNode( n1 );
    n1->AddChildNode( n11 );

    SetLocalTransform( n1, Transformations::Rotation( glm::vec3( 0.0f, 0.0f, -1.0f ), 10.5f ) );
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

// *****************************
//
void        SceneExampleBuilder::UpdateScene0            ( SceneNode * root, float t )
{
    float s = 40.f * t;
    SetLocalTransform( root->GetChild( 0 ), Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), -s ) );
    SetLocalTransform( root->GetChild( 0 )->GetChild( 0 ), Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), s / 5.f ) );
    SetWorldTransform( root, Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), s ) );
}

// *****************************
//
void        SceneExampleBuilder::UpdateScene1            ( SceneNode * root, float t )
{
    { root; t; }
    assert( false );
}

// *****************************
//
void        SceneExampleBuilder::UpdateScene2            ( SceneNode * root, float t )
{
    { root; t; }
    assert( false );
}

} // bv
