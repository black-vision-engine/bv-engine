#include "ScenePrototype0.h"

namespace bv {

// **************************
//
void                ScenePrototype0::Update              ( TimeType t )
{
    auto root = GetRootNode();

    float s = 40.f * t;

    //SetLocalTransform( root->GetChild( 0 ), Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), -s ) );
    //SetLocalTransform( root->GetChild( 0 )->GetChild( 0 ), Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), s / 5.f ) );

    SetWorldTransform( root, Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), s ) );
}

// **************************
//
void                ScenePrototype0::OnKey               ( unsigned char c )
{
    printf( "Pressed %c\n", c );


}

// **************************
//
SceneNode *         ScenePrototype0::BuildSceneImpl      ()
{
    return RootNode();
}

// **************************
//
void    ScenePrototype0::ToggleNode332          ()
{
    // FIXME: implement
    if( m_root && m_root->GetChild( 2 ) && m_root->GetChild( 2 )->GetChild( 2 ) && m_root->GetChild( 2 )->GetChild( 2 )->GetChild( 2 ) )
    {
        assert( false );
    }
}

// **************************
// Structure:
//
//  root->AddChildNode( n1 );
//  root->AddChildNode( n2 );
//      n2->AddChildNode( n21 );
//      n2->AddChildNode( n22 );
//  root->AddChildNode( n3 );
//      n3->AddChildNode( n31 );
//      n3->AddChildNode( n32 );
//      n3->AddChildNode( n33 );
//          n33->AddChildNode( n331 );
//          n33->AddChildNode( n332 );
//
SceneNode * ScenePrototype0::RootNode           ()
{
    SceneNode * root = SimpleNodeBuilder::CreateRectNodeTexture( 1.f, 1.f, -1.1f, "rsrcy/tex0.jpg" );
    SetLocalTransform( root, Transformations::Scale( glm::vec3( 2.f, 2.f, 1.0f ) ) );

    root->AddChildNode( Node1() );
    root->AddChildNode( Node2() );
    root->AddChildNode( Node3() );

    return root;
}

// **************************
//
SceneNode * ScenePrototype0::Node1              ()
{
    auto n1 = SimpleNodeBuilder::CreateRectNodeSolidColor( .3f, .3f, -1.2f );
    SetLocalTransform( n1, Transformations::Translation( glm::vec3( -0.35, 0.3f, 0.0f ) ) );
    SetPixelShaderParam( n1, "color", glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );

    return n1;
}

// **************************
//
SceneNode * ScenePrototype0::Node2              ()
{
    auto n2 = SimpleNodeBuilder::CreateRectNodeSolidColor( .3f, .3f, -1.2f );
    SetLocalTransform( n2, Transformations::Translation( glm::vec3( 0.0f, 0.3f, 0.0f ) ) );
    SetPixelShaderParam( n2, "color", glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );

    n2->AddChildNode( Node21() );
    n2->AddChildNode( Node22() );

    return n2;
}

// **************************
//
SceneNode * ScenePrototype0::Node3              ()
{
    auto n3 = SimpleNodeBuilder::CreateRectNodeSolidColor( .3f, .3f, -1.2f );
    SetLocalTransform( n3, Transformations::Translation( glm::vec3( 0.35f, 0.3f, 0.0f ) ) );
    SetPixelShaderParam( n3, "color", glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );

    n3->AddChildNode( Node31() );
    n3->AddChildNode( Node32() );
    n3->AddChildNode( Node33() );

    return n3;
}

// **************************
//
SceneNode * ScenePrototype0::Node21             ()
{
    auto n21 = SimpleNodeBuilder::CreateRectNodeTexture( .14f, .14f, -1.3f, "rsrcy/tex1.jpg" );
    SetLocalTransform( n21, Transformations::Translation( glm::vec3( -0.08f, 0.1f, 0.0f ) ) );

    return n21;
}

// **************************
//
SceneNode * ScenePrototype0::Node22             ()
{
    auto n22 = SimpleNodeBuilder::CreateRectNodeTexture( .14f, .14f, -1.3f, "rsrcy/tex0.jpg" );
    SetLocalTransform( n22, Transformations::Translation( glm::vec3( 0.08f, 0.1f, 0.0f ) ) );

    return n22;
}

// **************************
//
SceneNode * ScenePrototype0::Node31             ()
{
    auto n31 = SimpleNodeBuilder::CreateRectNodeTexture( .1f, .1f, -1.3f, "rsrcy/tex1.jpg" );
    SetLocalTransform( n31, Transformations::Translation( glm::vec3( -0.12f + 0.028f, 0.1f, 0.0f ) ) );

    return n31;
}

// **************************
//
SceneNode * ScenePrototype0::Node32             ()
{
    auto n32 = SimpleNodeBuilder::CreateRectNodeTexture( .1f, .1f, -1.3f, "rsrcy/tex0.jpg" );
    SetLocalTransform( n32, Transformations::Translation( glm::vec3( 0.0f + 0.028f, 0.1f, 0.0f ) ) );

    return n32;
}

// **************************
//
SceneNode * ScenePrototype0::Node33             ()
{
    auto n33 = SimpleNodeBuilder::CreateRectNodeTexture( .1f, .1f, -1.3f, "rsrcy/tex1.jpg" );
    SetLocalTransform( n33, Transformations::Translation( glm::vec3( 0.12f + 0.028f, 0.1f, 0.0f ) ) );

    n33->AddChildNode( Node331() );
    n33->AddChildNode( Node332() );

    return n33;
}

// **************************
//
SceneNode * ScenePrototype0::Node331            ()
{
    auto n331 = SimpleNodeBuilder::CreateRectNodeSolidColor( .05f, .05f, -1.4f );
    SetLocalTransform( n331, Transformations::Translation( glm::vec3( 0.013f, 0.05f, 0.0f ) ) );
    SetPixelShaderParam( n331, "color", glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) );

    return n331;
}

// **************************
//
SceneNode * ScenePrototype0::Node332            ()
{
    auto n332 = SimpleNodeBuilder::CreateRectNodeSolidColor( .05f, .05f, -1.4f );
    SetLocalTransform( n332, Transformations::Translation( glm::vec3( 0.065f, 0.05f, 0.0f ) ) );
    SetPixelShaderParam( n332, "color", glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );

    return n332;
}

} //bv
