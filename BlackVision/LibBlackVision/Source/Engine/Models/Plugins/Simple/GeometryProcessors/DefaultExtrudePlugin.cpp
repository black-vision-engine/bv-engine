#include "stdafx.h"
#include "DefaultExtrudePlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/HelperIndexedGeometryConverter.h"


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
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR, glm::vec3( 0.0, 0.0, -0.9 ), true, true );
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
    // Init parameters
    glm::vec3 translate = QueryTypedValue< ValueVec3Ptr >( GetValue( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR ) )->GetValue();


    // Get previous plugin geometry channels
    auto positions = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );
    //auto normals = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_NORMAL ) );
    //auto uvs = std::static_pointer_cast< Float2AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) );

    assert( positions );    if( !positions ) return;
    
    auto posChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    auto newPositions = std::make_shared< model::Float3AttributeChannel >( posChannelDesc, positions->GetName(), true );

    auto connComp = ConnectedComponent::Create();
    connComp->AddAttributeChannel( newPositions );

    IndexedGeometry mesh;
    IndexedGeometryConverter converter;

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

    AddSymetricalPlane( mesh, translate );
    AddSidePlanes( mesh, edges );

    converter.MakeTriangles( mesh, newPositions );


    //if( topology == PrimitiveType::PT_TRIANGLE_STRIP )
    //{
    //    int numVerticies = positions->GetNumEntries();

    //    newPositions->GetVertices().reserve( 2 * numVerticies + 2 );

    //    for( int i = 0; i < numVerticies; ++i )
    //    {
    //        newPositions->AddAttribute( positions->GetVertices()[ i ] );
    //    }

    //    newPositions->AddAttribute( positions->GetVertices()[ numVerticies - 1 ] );
    //    newPositions->AddAttribute( translate + positions->GetVertices()[ numVerticies - 1 ] );

    //    for( int i = numVerticies - 1; i >= 0; --i )
    //    {
    //        newPositions->AddAttribute( translate + positions->GetVertices()[ i ] );
    //    }

    //    if( normals )
    //    {
    //        auto normChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    //        auto newNormal = std::make_shared< model::Float3AttributeChannel >( normChannelDesc, normals->GetName(), true );
    //        newNormal->GetVertices().reserve( 2 * numVerticies + 2 );

    //        connComp->AddAttributeChannel( newNormal );

    //        for( int i = 0; i < numVerticies; ++i )
    //        {
    //            newNormal->AddAttribute( normals->GetVertices()[ i ] );
    //        }

    //        newNormal->AddAttribute( normals->GetVertices()[ numVerticies - 1 ] );
    //        newNormal->AddAttribute( -normals->GetVertices()[ numVerticies - 1 ] );

    //        for( int i = numVerticies - 1; i >= 0; --i )
    //        {
    //            newNormal->AddAttribute( -normals->GetVertices()[ i] );
    //        }
    //    }

    //    if( uvs )
    //    {
    //        auto uvChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    //        auto newUVs = std::make_shared< model::Float2AttributeChannel >( uvChannelDesc, uvs->GetName(), true );
    //        newUVs->GetVertices().reserve( 2 * numVerticies + 2 );

    //        connComp->AddAttributeChannel( newUVs );

    //        for( int i = 0; i < numVerticies; ++i )
    //        {
    //            newUVs->AddAttribute( uvs->GetVertices()[ i ] );
    //        }

    //        newUVs->AddAttribute( uvs->GetVertices()[ numVerticies - 1 ] );
    //        newUVs->AddAttribute( uvs->GetVertices()[ numVerticies - 1 ] );

    //        for( int i = numVerticies - 1; i >= 0; --i )
    //        {
    //            newUVs->AddAttribute( uvs->GetVertices()[ i ] );
    //        }
    //    }

    //}


    m_vaChannel->AddConnectedComponent( connComp );
}

// ***********************
//
void                                DefaultExtrudePlugin::AddSymetricalPlane      ( IndexedGeometry& mesh, glm::vec3 translate )
{
    auto & vertices = mesh.GetVerticies();
    auto & indices = mesh.GetIndicies();
    int numVerticies = (int)vertices.size();
    auto numIndicies = indices.size();

    // Add symetrical verticies
    for( int i = numVerticies - 1; i >= 0; --i )
    {
        vertices.push_back( translate + vertices[ numVerticies - i - 1 ] );
    }

    for( int i = 0; i < numIndicies; ++i )
    {
        indices.push_back( indices[ i ] + (INDEX_TYPE)numVerticies );
    }
}

// ***********************
// Function assumes that someone used function AddSymetricalPlane.
void                                DefaultExtrudePlugin::AddSidePlanes           ( IndexedGeometry & mesh, const std::vector< INDEX_TYPE > & edges )
{
    //auto & vertices = mesh.GetVerticies();
    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();

    int symPlaneOffset = (int)verticies.size() / 2;

    // @todo In future we must add normals. That means we must add verticies too, bacause
    // edge is sharp and normals can't be the same.

    for( int i = 0; i < edges.size(); i += 2 )
    {
        indices.push_back( edges[ i ] );
        indices.push_back( edges[ i ] + symPlaneOffset );
        indices.push_back( edges[ i + 1 ] );

        indices.push_back( edges[ i ] + symPlaneOffset );
        indices.push_back( edges[ i + 1 ] + symPlaneOffset );
        indices.push_back( edges[ i + 1 ] );
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
        if( indicies[ i ] == idx2 && indicies[ i - 1 ] == idx1 )
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
std::vector< INDEX_TYPE >       DefaultExtrudePlugin::ExtractCorners          ( IndexedGeometry & /*mesh*/, const std::vector< INDEX_TYPE > & /*edges*/, float /*angleThreshold*/ )
{
    //auto & vertices = mesh.GetVerticies();
    std::vector< INDEX_TYPE > corners;

    assert( !"Implement me" );

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

