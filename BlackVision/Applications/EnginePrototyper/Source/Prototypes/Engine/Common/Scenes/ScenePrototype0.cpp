#include "ScenePrototype0.h"

#include "Prototypes/Engine/Common/ExperimentalNodeDeleter.h"


namespace bv {

// **************************
//
ScenePrototype0::ScenePrototype0     ( Renderer * renderer )
    : ScenePrototype( renderer )
    , m_detachedNode( nullptr )
{
    m_n0 = m_n1 = m_n2 = 0;
}

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
    unsigned int pos[3] = {m_n0, m_n1, m_n2};

    bool showloc = false;
    bool show = false;
    bool selected = false;
    bool attach = false;
    bool detach = false;
    bool del = false;
    
    bool c1 = false;
    bool c2 = false;
    bool c3 = false;

    switch( c )
    {
        case '1':
            m_n0 = 1;
            showloc = true;
            break;
        case '2':
            m_n0 = 2;
            showloc = true;
            break;
        case '3':
            m_n0 = 3;
            showloc = true;
            break;
        case '4':
            m_n0 = 0;
            showloc = true;
            break;
        case 'q':
            m_n1 = 1;
            showloc = true;
            break;
        case 'w':
            m_n1 = 2;
            showloc = true;
            break;
        case 'e':
            m_n1 = 3;
            showloc = true;
            break;
        case 'r':
            m_n1 = 0;
            showloc = true;
            break;
        case 'a':
            m_n2 = 1;
            showloc = true;
            break;
        case 's':
            m_n2 = 2;
            showloc = true;
            break;
        case 'd':
            m_n2 = 3;
            showloc = true;
            break;
        case 'f':
            m_n2 = 0;
            showloc = true;
            break;
        case 'i':
            c1 = true;
            break;
        case 'o':
            c2 = true;
            break;
        case 'p':
            c3 = true;
            break;
        case ' ':
            show = true;
            break;
        case '+':
            attach = true;
            break;
        case 13:
            showloc = true;
            selected = true;
            break;
        case '-':
            detach = true;
            break;
        case 8:
            del = true;
            break;
        default:
            ;
    };

    if( showloc )
    {
        printf( "Selected location -> [%d, %d, %d]\n", m_n0, m_n1, m_n2 );
    }

    if( show )
    {
        PrintStructure();
    }

    if( selected )
    {
        SceneNode * node = GetNode( GetRootNode(), pos, 3 );

        if( node )
        {
            PrintStructure( node, 0, 0 );
        }
        else
        {
            printf( "No node at specified location\n" );
        }
    }

    if( detach )
    {
        printf( "Detach location -> [%d, %d, %d]\n", m_n0, m_n1, m_n2 );

        SceneNode * node = GetNode( GetRootNode(), pos, 3 );

        if( node )
        {
            if( m_detachedNode )
            {
                printf( "Removing old detached node\n" );
                DeleteNodeExperimental( m_detachedNode );
            }

            m_detachedNode = node;

            bool dsuccess = DetachNode( GetRootNode(), node );

            assert( dsuccess );

            PrintStructure();
        }
        else
        {
            printf( "No node at specified location\n" );
        }
    }

    if( del )
    {
        printf( "Deleting previously detached node\n" );

        if( m_detachedNode )
        {
            DeleteNodeExperimental( m_detachedNode );
            m_detachedNode = nullptr;
        }
        else
        {
            printf( "No node was detached\n" );
        }
    }

    if( attach )
    {
        printf( "Attach at location -> [%d, %d, %d]\n", m_n0, m_n1, m_n2 );

        SceneNode * node = GetNode( GetRootNode(), pos, 3 );

        if( !node && m_n0 == 0 )
        {
            node = GetRootNode();
        }

        if( node )
        {
            if( m_detachedNode )
            {
                node->AddChildNode( m_detachedNode );
                m_detachedNode = nullptr;

                PrintStructure();
            }
            else
            {
                printf( "No previously detached node available\n" );
            }
        }
        else
        {
            printf( "No node at specified location\n" );
        }
    }

    if ( c1 || c2 || c3 )
    {
        printf( "Create (%d, %d, %d) at location -> [%d, %d, %d]\n", c1, c2, c3, m_n0, m_n1, m_n2 );

        SceneNode * node = GetNode( GetRootNode(), pos, 3 );

        if( !node && m_n0 == 0 )
        {
            node = GetRootNode();
        }
        
        if( node )
        {
            if( c1 )
            {
                node->AddChildNode( Node1() );
            }

            if( c2 )
            {
                node->AddChildNode( Node2() );
            }

            if( c3 )
            {
                node->AddChildNode( Node3() );
            }

            PrintStructure();
        }
        else
        {
            printf( "No node at specified location\n" );
        }
    }
}

// **************************
//
SceneNode *         ScenePrototype0::BuildSceneImpl      ()
{
    return RootNode();
}

// **************************
//
void                ScenePrototype0::PrintStructure      ()
{
    PrintStructure( GetRootNode(), 0, 0 );
}

// **************************
//
void                ScenePrototype0::PrintStructure      ( SceneNode * node, unsigned int idx, unsigned int indent )
{
    printf( "%*s", indent, "" );
    printf( "Node_%d\n", idx );

    for( unsigned int i = 0; i < node->NumChildNodes(); ++i )
    {
        SceneNode * child = node->GetChild( i );

        PrintStructure( child, i + 1, indent + 4 );
    }
}

// **************************
//
void                ScenePrototype0::DeleteNodeExperimental( SceneNode * node )
{
    ExperimentalDeleteAndCleanup( node, GetRenderer() );
}

// **************************
//
bool                ScenePrototype0::DetachNode          ( SceneNode * node, SceneNode * detachNode )
{
    for( unsigned int i = 0; i < node->NumChildNodes(); ++i )
    {
        if( node->GetChild( i ) == detachNode )
        {
            node->DetachChildNode( i );

            return true;
        }
    }

    for( unsigned int i = 0; i < node->NumChildNodes(); ++i )
    {
        if( DetachNode( node->GetChild( i ), detachNode ) )
        {
            return true;
        }        
    }

    return false;
}

// **************************
//
SceneNode *         ScenePrototype0::GetNode             ( SceneNode * node, unsigned int * pos, unsigned int numElts )
{
    if( numElts == 0 )
    {
        return nullptr;
    }

    if( *pos == 0 || *pos > node->NumChildNodes() )
    {
        return nullptr;
    }

    SceneNode * child = node->GetChild( *pos - 1 );
    SceneNode * res = GetNode( child, pos + 1, numElts - 1 );

    if ( !res )
    {
        if( pos[ 1 ] == 0 )
        {
            res = child;
        }
    }

    return res;
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
