#include "SceneExampleBuilder.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

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
void        SceneExampleBuilder::OnKeyScene         ( unsigned int sceneNum, SceneNode * root, unsigned char c )
{
    switch ( sceneNum )
    {
        case 0:
            OnKeyScene0( root, c );
            break;
        case 1:
            OnKeyScene1( root, c );
            break;
        case 2:
            OnKeyScene2( root, c );
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
void        SceneExampleBuilder::SetPixelShaderParam    ( SceneNode * node, const std::string & name, const glm::vec4 & param )
{
    auto renderable = static_cast< RenderableEntity * >( node->GetTransformable() );

    auto psParam = renderable->GetRenderableEffect()->GetPass( 0 )->GetPixelShader()->GetParameters()->AccessParam( name );

    if( psParam )
    {
        auto val = static_cast<ValueVec4*>(psParam->AccessValuePtr());
        val->SetValue( param );
    }
}

// *****************************
//
SceneNode * SceneExampleBuilder::Scene0     ()
{
    auto root = SimpleNodeBuilder::CreateRectNodeTexture( 1.f, 1.f, -1.1f, "rsrcy/tex0.jpg" );
    SetLocalTransform( root, Transformations::Scale( glm::vec3( 2.f, 2.f, 1.0f ) ) );
    
    auto n1 = SimpleNodeBuilder::CreateRectNodeSolidColor( .3f, .3f, -1.2f );
    SetLocalTransform( n1, Transformations::Translation( glm::vec3( -0.35, 0.3f, 0.0f ) ) );
    SetPixelShaderParam( n1, "color", glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );

    auto n2 = SimpleNodeBuilder::CreateRectNodeSolidColor( .3f, .3f, -1.2f );
    SetLocalTransform( n2, Transformations::Translation( glm::vec3( 0.0f, 0.3f, 0.0f ) ) );
    SetPixelShaderParam( n2, "color", glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    
    auto n21 = SimpleNodeBuilder::CreateRectNodeTexture( .14f, .14f, -1.3f, "rsrcy/tex1.jpg" );
    SetLocalTransform( n21, Transformations::Translation( glm::vec3( -0.08f, 0.1f, 0.0f ) ) );

    auto n22 = SimpleNodeBuilder::CreateRectNodeTexture( .14f, .14f, -1.3f, "rsrcy/tex0.jpg" );
    SetLocalTransform( n22, Transformations::Translation( glm::vec3( 0.08f, 0.1f, 0.0f ) ) );

    auto n3 = SimpleNodeBuilder::CreateRectNodeSolidColor( .3f, .3f, -1.2f );
    SetLocalTransform( n3, Transformations::Translation( glm::vec3( 0.35f, 0.3f, 0.0f ) ) );
    SetPixelShaderParam( n3, "color", glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
   
    auto n31 = SimpleNodeBuilder::CreateRectNodeTexture( .1f, .1f, -1.3f, "rsrcy/tex1.jpg" );
    SetLocalTransform( n31, Transformations::Translation( glm::vec3( -0.12f + 0.028f, 0.1f, 0.0f ) ) );

    auto n32 = SimpleNodeBuilder::CreateRectNodeTexture( .1f, .1f, -1.3f, "rsrcy/tex0.jpg" );
    SetLocalTransform( n32, Transformations::Translation( glm::vec3( 0.0f + 0.028f, 0.1f, 0.0f ) ) );

    auto n33 = SimpleNodeBuilder::CreateRectNodeTexture( .1f, .1f, -1.3f, "rsrcy/tex1.jpg" );
    SetLocalTransform( n33, Transformations::Translation( glm::vec3( 0.12f + 0.028f, 0.1f, 0.0f ) ) );

    auto n331 = SimpleNodeBuilder::CreateRectNodeSolidColor( .05f, .05f, -1.4f );
    SetLocalTransform( n331, Transformations::Translation( glm::vec3( 0.013f, 0.05f, 0.0f ) ) );
    SetPixelShaderParam( n331, "color", glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) );

    auto n332 = SimpleNodeBuilder::CreateRectNodeSolidColor( .05f, .05f, -1.4f );
    SetLocalTransform( n332, Transformations::Translation( glm::vec3( 0.065f, 0.05f, 0.0f ) ) );
    SetPixelShaderParam( n332, "color", glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );

    root->AddChildNode( n1 );
    root->AddChildNode( n2 );
        n2->AddChildNode( n21 );
        n2->AddChildNode( n22 );
    root->AddChildNode( n3 );
        n3->AddChildNode( n31 );
        n3->AddChildNode( n32 );
        n3->AddChildNode( n33 );
            n33->AddChildNode( n331 );
            n33->AddChildNode( n332 );
 
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
    //SetLocalTransform( root->GetChild( 0 ), Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), -s ) );
    //SetLocalTransform( root->GetChild( 0 )->GetChild( 0 ), Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), s / 5.f ) );
    SetWorldTransform( root, Transformations::Rotation( glm::vec3( 0.f, 0.f, -1.f ), 0.f * s ) );
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

// *****************************
//
void        SceneExampleBuilder::OnKeyScene0             ( SceneNode * root, unsigned char c )
{
    { root; c; }

    printf( "Pressed %c\n", c );
}

// *****************************
//
void        SceneExampleBuilder::OnKeyScene1             ( SceneNode * root, unsigned char c )
{
    { root; c; }
    assert( false );
}

// *****************************
//
void        SceneExampleBuilder::OnKeyScene2             ( SceneNode * root, unsigned char c )
{
    { root; c; }
    assert( false );
}

} // bv
