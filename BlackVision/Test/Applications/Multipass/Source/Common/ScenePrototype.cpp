#include "ScenePrototype.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Common/Scenes/ScenePrototype0.h"
#include "Common/Scenes/ScenePrototype1.h"
#include "Common/Scenes/ScenePrototype2.h"
#include "Common/Scenes/ScenePrototype3.h"


namespace  bv {

// **************************
//
ScenePrototype *    ScenePrototype::CreateNewPrototype  ( unsigned int i, Renderer * renderer )
{
    ScenePrototype * res = nullptr;

    switch( i )
    {
        case 0:
            res = new ScenePrototype0( renderer );
            break;
        case 1:
            res = new ScenePrototype1( renderer );
            break;
        case 2:
            res = new ScenePrototype2( renderer );
            break;
		case 3:
			res = new ScenePrototype3( renderer );
			break;
        default:
            ;
    }

    if( res )
    {
        res->BuildScene();
    }

    return res;
}

// **************************
//
void    ScenePrototype::SetLocalTransform   ( SceneNode * node, const Transform & transform )
{
    auto transformable = node->GetTransformable();

    transformable->SetLocalTransform( transform );
}

// **************************
//
void    ScenePrototype::SetWorldTransform   ( SceneNode * node, const Transform & transform )
{
    node->Update( transform );
}

// **************************
//
void    ScenePrototype::SetPixelShaderParam ( SceneNode *, const std::string &, const glm::vec4 & )
{
    //auto renderable = static_cast< RenderableEntity * >( node->GetTransformable() );

    //auto psParam = renderable->GetRenderableEffect()->GetPass( 0 )->GetPixelShader()->GetParameters()->AccessParam( name );

    //if( psParam )
    //{
    //    //auto val = static_cast< ValueVec4 * >( psParam->AccessValuePtr() );
    //    //val->SetValue( param );
    //}
}

// **************************
//
ScenePrototype::ScenePrototype  ( Renderer * renderer )
    : m_root( nullptr )
    , m_renderer( renderer )
{
}

// **************************
//
ScenePrototype::~ScenePrototype ()
{
    delete m_root;
}

// **************************
//
void ScenePrototype::Update              ( TimeType t )
{
    { t; }
    // default implementation is empty
}

// **************************
//
void ScenePrototype::OnKey               ( unsigned char c )
{
    { c; }
    // default implementation is empty
}

// **************************
//
void ScenePrototype::BuildScene ()
{
    m_root = BuildSceneImpl();
}

// **************************
//
SceneNode * ScenePrototype::GetRootNode ()
{
    return m_root;
}

// **************************
//
void        ScenePrototype::SetRootNode ( SceneNode * node )
{
    m_root = node;
}

// **************************
//
Renderer *  ScenePrototype::GetRenderer ()
{
    return m_renderer;
}

// **************************
//
void        ScenePrototype::SetWorldTransform   ( const Transform & transform )
{
    if( m_root )
    {
        SetWorldTransform( m_root, transform );
    }
}

} //bv
