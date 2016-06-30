#include "stdafx.h"
#include "DefaultExtrudePlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/HelperIndexedGeometryConverter.h"

#include <glm/gtx/vector_angle.hpp>
#include <map>


namespace bv { namespace model {


const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR           = "extrude vector";
const std::string        DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE   = "smooth threshold angle";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultExtrudePluginDesc::DefaultExtrudePluginDesc                          ()
    : DefaultGeometryProcessorDescBase( UID(), "extrude" )
{}

// *******************************
//
IPluginPtr              DefaultExtrudePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultExtrudePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultExtrudePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model = helper.GetModel();
    
    helper.CreateVacModel();
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR, glm::vec3( 0.4, 0.0, -0.9 ), true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE, 160.0f, true, true );

    return model;
}

// *******************************
//
std::string             DefaultExtrudePluginDesc::UID                       ()
{
    return "DEFAULT_EXTRUDE_PLUGIN";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultExtrudePlugin::DefaultExtrudePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultGeometryProcessorBase( name, uid, prev, model )
{
    SetPrevPlugin( prev );
}

// *************************************
// 
DefaultExtrudePlugin::~DefaultExtrudePlugin         ()
{}

// ***********************
//
void        DefaultExtrudePlugin::ProcessConnectedComponent       ( model::ConnectedComponentPtr & currComponent,
                                                                    std::vector< IConnectedComponentPtr > & /*allComponents*/,
                                                                    PrimitiveType topology )
{
    // Get parameters values.
    glm::vec3 translate     = QueryTypedValue< ValueVec3Ptr >( GetValue( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR ) )->GetValue();
    float cornerThreshold   = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE ) )->GetValue();


    // Get previous plugin geometry channels
    auto positions = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );
    //auto uvs = std::static_pointer_cast< Float2AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) );

    assert( positions );    if( !positions ) return;
    
    auto posChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_PROCESSOR );
    auto newPositions = std::make_shared< model::Float3AttributeChannel >( posChannelDesc, positions->GetName(), true );

    auto connComp = ConnectedComponent::Create();
    connComp->AddAttributeChannel( newPositions );

    IndexedGeometry mesh;
    IndexedGeometryConverter converter;
    converter.RememberConversionIndicies( true );   // Use this converter for other channels


    if( topology == PrimitiveType::PT_TRIANGLE_STRIP )
    {
        mesh = converter.MakeIndexGeomFromStrips( positions );
    }
    else if( topology == PrimitiveType::PT_TRIANGLES )
    {
        mesh = converter.MakeIndexGeomFromTriangles( positions );
    }
    else
    {
        assert( !"This primitive topology is not supported yet" );
    }

    auto edges = ExtractEdges( mesh );
    auto corners = ExtractCorners( mesh, edges, cornerThreshold );

    AddSymetricalPlane( mesh, translate );
    AddSidePlanes( mesh, edges );


    IndexedGeometry normals;

    auto normChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_PROCESSOR );
    Float3AttributeChannelPtr normalsChannel = std::make_shared< Float3AttributeChannel >( normChannelDesc, normChannelDesc->SuggestedDefaultName( 0 ), true );
    normalsChannel->GetVertices().reserve( mesh.GetVerticies().size() );

    connComp->AddAttributeChannel( normalsChannel );

    auto prevNormals = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_NORMAL ) );
    if( prevNormals )
    {
        normals.GetVerticies() = converter.ConvertFromMemory( prevNormals );
        FillWithNormals( mesh, normals.GetVerticies(), translate, false );
    }
    else
    {
        FillWithNormals( mesh, normals.GetVerticies(), translate, true );
    }


    converter.MakeTriangles( mesh, newPositions );
    converter.MakeTriangles( normals.GetVerticies(), mesh.GetIndicies(), normalsChannel );
    
    m_vaChannel->AddConnectedComponent( connComp );
}

// ***********************
//
void    DefaultExtrudePlugin::AddSymetricalPlane      ( IndexedGeometry& mesh, glm::vec3 translate )
{
    auto & vertices = mesh.GetVerticies();
    auto & indices = mesh.GetIndicies();
    int numVerticies = (int)vertices.size();
    auto numIndicies = indices.size();

    m_numUniqueExtrudedVerticies = numVerticies;
    m_numExtrudedVerticies = (int)numIndicies;

    // Add symetrical verticies
    for( int i = 0; i < numVerticies; ++i )
    {
        vertices.push_back( translate + vertices[ i ] );
    }

    for( int i = 0; i < numIndicies; i += 3 )
    {
        indices.push_back( indices[ i ] + (INDEX_TYPE)numVerticies );
        indices.push_back( indices[ i + 2 ] + (INDEX_TYPE)numVerticies );
        indices.push_back( indices[ i + 1 ] + (INDEX_TYPE)numVerticies );
    }
}

// ***********************
// Function assumes that someone used function AddSymetricalPlane.
void    DefaultExtrudePlugin::AddSidePlanes           ( IndexedGeometry & mesh, const std::vector< INDEX_TYPE > & edges )
{
    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();
    int numVerticies = (int)verticies.size();

    int symPlaneOffset = (int)verticies.size() / 2;

    // In future we must add normals. That means we must add verticies too, bacause
    // edge is sharp and normals can't be the same.
    // This solution produces unused verticies, because we copy not only edge verticies.
    // That's no problem. When indexed geometry is convertet to triangles, all unused verticies disapear.
    verticies.reserve( 2 * numVerticies );
    for( int i = 0; i < numVerticies; i++ )
    {
        verticies.push_back( verticies[ i ] );
    }


    for( int i = 0; i < edges.size(); i += 2 )
    {
        indices.push_back( numVerticies + edges[ i ] );
        indices.push_back( numVerticies + edges[ i ] + symPlaneOffset );
        indices.push_back( numVerticies + edges[ i + 1 ] );

        indices.push_back( numVerticies + edges[ i ] + symPlaneOffset );
        indices.push_back( numVerticies + edges[ i + 1 ] + symPlaneOffset );
        indices.push_back( numVerticies + edges[ i + 1 ] );
    }
}

// ***********************
//
void    DefaultExtrudePlugin::FillWithNormals         ( IndexedGeometry & mesh,
                                                       std::vector< glm::vec3 > & normals,
                                                       glm::vec3 translate,
                                                       bool fillDefaults )
{
    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();

    normals.resize( verticies.size(), glm::vec3( 0.0, 0.0, 0.0 ) );

    if( fillDefaults )
    {
        // Set default normals for both planes.
        for( int i = 0; i < m_numUniqueExtrudedVerticies; ++i )
        {
            normals[ i ] = glm::vec3( 0.0, 0.0, 1.0 );
        }

        for( int i = m_numUniqueExtrudedVerticies; i < 2 * m_numUniqueExtrudedVerticies; ++i )
        {
            normals[ i ] = glm::vec3( 0.0, 0.0, -1.0 );
        }
    }
    else
    {
        // Normals have been copied from vertex attribute channel.
        // Copy and negate them to fill back plane.
        for( int i = m_numUniqueExtrudedVerticies; i < 2 * m_numUniqueExtrudedVerticies; ++i )
        {
            normals[ i ] = glm::vec3( 0.0, 0.0, -1.0 );
        }
    }

    for( int i = 2 * m_numExtrudedVerticies; i < indices.size(); i += 6 )
    {
        glm::vec3 edgeDir = verticies[ indices[ i + 2 ] ] - verticies[ indices[ i ] ];

        glm::vec3 normal = -glm::cross( edgeDir, translate );
        if( normal != glm::vec3( 0.0, 0.0, 0.0 ) )
            normal = glm::normalize( normal );

        normals[ indices[ i ] ] += normal;
        normals[ indices[ i + 1 ] ] += normal;
        normals[ indices[ i + 2 ] ] += normal;
        normals[ indices[ i + 4 ] ] += normal;
    }

    for( auto & norm : normals )
    {
        if( norm != glm::vec3( 0.0, 0.0, 0.0 ) )
            norm = glm::normalize( norm );
    }

}

// ***********************
// Edge is a pair of verticies that builds only one triangle in whole mesh.
// Note: Edges have their direction. Order of verticies counts. It's used later
// to determine normal direction as cross product between edge vector and extrude vector.
std::vector< INDEX_TYPE >           DefaultExtrudePlugin::ExtractEdges ( IndexedGeometry& mesh )
{
    std::vector< INDEX_TYPE >   edges;

    auto & indicies = mesh.GetIndicies();
    auto numIndicies = indicies.size();

    edges.reserve( numIndicies / 3 );

    for( int i = 0; i < numIndicies; i += 3 )
    {
        AddOrRemoveEdge( edges, indicies[ i ], indicies[ i + 1 ] );
        AddOrRemoveEdge( edges, indicies[ i + 1 ], indicies[ i + 2 ] );
        AddOrRemoveEdge( edges, indicies[ i + 2 ], indicies[ i ] );
    }

    return edges;
}

// ***********************
// Returns max( SizeType ) if Edge coundn't be found.
int                            DefaultExtrudePlugin::FindEdge  ( const std::vector< INDEX_TYPE > & indicies, INDEX_TYPE idx1, INDEX_TYPE idx2 )
{
    for( int i = (int)indicies.size() - 1; i >= 0; i -= 2 )
    {
        if( ( indicies[ i ] == idx2 && indicies[ i - 1 ] == idx1 ) ||
            ( indicies[ i ] == idx1 && indicies[ i - 1 ] == idx2 ) )
            return i - 1;
    }
    return std::numeric_limits< int >::max();
}

// ***********************
//
void                            DefaultExtrudePlugin::AddOrRemoveEdge   ( std::vector< INDEX_TYPE > & edges, INDEX_TYPE idx1, INDEX_TYPE idx2 )
{
    int searchResult = FindEdge( edges, idx1, idx2 );
    if( searchResult == std::numeric_limits< int >::max() )
    {
        edges.push_back( idx1 );
        edges.push_back( idx2 );
    }
    else
    {
        auto iter = edges.begin() + searchResult;
        edges.erase( iter, iter + 2 );
    }
}

// ***********************
//
std::vector< INDEX_TYPE >       DefaultExtrudePlugin::ExtractCorners          ( IndexedGeometry & mesh, const std::vector< INDEX_TYPE > & edges, float angleThreshold )
{
    //float threshold = glm::radians( angleThreshold );

    auto & vertices = mesh.GetVerticies();
    std::vector< INDEX_TYPE > corners;

    // This is very inefficeint way to do this. Map requires many memory allocations;
    std::map< INDEX_TYPE, std::pair< glm::vec3, glm::vec3 > > edgeVectors;

    // Compute edge vectors.
    for( int i = 0; i < edges.size(); i += 2 )
    {
        INDEX_TYPE idx1 = edges[ i ];
        INDEX_TYPE idx2 = edges[ i + 1 ];

        glm::vec3 vert1 = vertices[ idx1 ];
        glm::vec3 vert2 = vertices[ idx2 ];

        glm::vec3 edgeVec = vert2 - vert1;

        edgeVectors[ idx1 ].second = edgeVec;
        edgeVectors[ idx2 ].first = -edgeVec;
    }

    // Compute angles between edge vectors and compare with threshold.
    for( auto iter = edgeVectors.begin(); iter != edgeVectors.end(); iter++ )
    {
        float angle = glm::angle( glm::normalize( iter->second.first ), glm::normalize( iter->second.second ) );
        if( angle < angleThreshold )
            corners.push_back( iter->first );
    }

    return corners;
}

// ========================================================================= //
// Default Processing function override
// ========================================================================= //

// ***********************
// Function always converts geometry channel to PrimitiveType::PT_TRIANGLES.
// This is the only difference between base class function and this override.
void                                DefaultExtrudePlugin::ProcessVertexAttributesChannel  ()
{
	if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
	{
		m_vaChannel = nullptr;
		return;
	}

    auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();

    auto compVertDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    VertexAttributesChannelDescriptor vaChannelDesc;
    vaChannelDesc.AddAttrChannelDesc( compVertDesc );

    auto normalChannelDesc = static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() )->GetAttrChannelDescriptor( AttributeSemantic::AS_NORMAL );
    if( normalChannelDesc )
    {
        vaChannelDesc.AddAttrChannelDesc( new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR ) );
    }

	if( !m_vaChannel )
	{
        m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_TRIANGLES, vaChannelDesc );
	}
	else
	{
		m_vaChannel->ClearAll();
		m_vaChannel->SetDescriptor( vaChannelDesc );
	}

	auto prevComponents = prevGeomChannel->GetComponents();
    for( unsigned int i = 0; i < prevComponents.size(); ++i )
    {
        auto prevConnComp = std::static_pointer_cast< model::ConnectedComponent >( prevComponents[ i ] );
        ProcessConnectedComponent( prevConnComp, prevComponents, prevGeomChannel->GetPrimitiveType() );
    }
}

} // model
} // bv

