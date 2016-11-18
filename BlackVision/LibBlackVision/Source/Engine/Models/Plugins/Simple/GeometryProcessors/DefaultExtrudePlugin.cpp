#include "stdafx.h"
#include "DefaultExtrudePlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/HelperIndexedGeometryConverter.h"

#include <glm/gtx/vector_angle.hpp>
#include <map>




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {




const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR           = "extrude vector";
const std::string        DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE   = "smooth threshold angle";
const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_CURVE            = "extrude curve";
const std::string        DefaultExtrudePlugin::PARAMS::CURVE_SCALE              = "curve scale";
const std::string        DefaultExtrudePlugin::PARAMS::COSINUS_CURVE_PERIOD     = "cosinus curve period";
const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_TESSELATION      = "tesselation";


typedef ParamEnum< DefaultExtrudePlugin::ExtrudeCurveType> ParamCurveType;


// ***********************
//
VoidPtr    ParamCurveType::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

// ***********************
//
template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultExtrudePlugin::ExtrudeCurveType >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultExtrudePlugin::ExtrudeCurveType >( name, timeline );
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"



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
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::CURVE_SCALE, 0.2f, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::EXTRUDE_TESSELATION, 40, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::COSINUS_CURVE_PERIOD, 1, true, true );


    helper.AddParam< IntInterpolator, DefaultExtrudePlugin::ExtrudeCurveType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamCurveType >
        ( DefaultExtrudePlugin::PARAMS::EXTRUDE_CURVE, DefaultExtrudePlugin::ExtrudeCurveType::None, true, true );

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

// ========================================================================= //
// Extrude curve functions
// ========================================================================= //

// ***********************
// param is float value between 0 and 1.
float       DefaultExtrudePlugin::PeriodicCosinusCurve ( float param )
{
    int period = m_cosinusPeriod;
    float arg = 2.0f * period * glm::pi< float >() * param;
    float result = cos( arg ) / 2.0f + 0.5f;
    return 1.0f - result;
}

// ***********************
//
float       DefaultExtrudePlugin::ParabolaCurve         ( float param )
{
    return -param * ( param - 1.0f );
}

// ***********************
// Gauss function with center in 0.5. Function reaches 0 near param == 0.0 and param == 1.0
float       DefaultExtrudePlugin::GaussCurve            ( float param )
{
    float exponent = -( param - 0.5f ) * ( param - 0.5f ) / ( 2.0f * 0.166666f * 0.166666f );
    return exp( exponent );
}

// ***********************
//
float       DefaultExtrudePlugin::CircleCurve           ( float param )
{
    return sqrt( 0.25f - ( param - 0.5f ) * ( param - 0.5f ) );
}

// ========================================================================= //
// Processing
// ========================================================================= //

// ***********************
//
void        DefaultExtrudePlugin::ProcessConnectedComponent       ( model::ConnectedComponentPtr & currComponent,
                                                                    std::vector< IConnectedComponentPtr > & /*allComponents*/,
                                                                    PrimitiveType topology )
{
    // Get parameters values.
    glm::vec3 translate     = QueryTypedValue< ValueVec3Ptr >( GetValue( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR ) )->GetValue();
    float cornerThreshold   = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE ) )->GetValue();
    m_tesselation           = QueryTypedValue< ValueIntPtr >( GetValue( DefaultExtrudePlugin::PARAMS::EXTRUDE_TESSELATION ) )->GetValue();
    m_curveScale            = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::CURVE_SCALE ) )->GetValue();
    m_cosinusPeriod         = QueryTypedValue< ValueIntPtr >( GetValue( DefaultExtrudePlugin::PARAMS::COSINUS_CURVE_PERIOD ) )->GetValue();
    ExtrudeCurveType curve  = QueryTypedParam< std::shared_ptr< ParamEnum< ExtrudeCurveType > > >( GetParameter( PARAMS::EXTRUDE_CURVE ) )->Evaluate();

    if( m_tesselation < 0 )
        m_tesselation = 1;

    // Get previous plugin geometry channels
    auto positions = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );
    assert( positions );    if( !positions ) return;
    
    auto posChannelDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_PROCESSOR );
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
    AddSidePlanes( mesh, edges, corners );


    IndexedGeometry normals;

    auto normChannelDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_PROCESSOR );
    Float3AttributeChannelPtr normalsChannel = std::make_shared< Float3AttributeChannel >( normChannelDesc, normChannelDesc->SuggestedDefaultName( 0 ), true );
    normalsChannel->GetVertices().reserve( mesh.GetVerticies().size() );

    connComp->AddAttributeChannel( normalsChannel );

    auto prevNormals = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_NORMAL ) );
    if( prevNormals )
    {
        normals.GetVerticies() = converter.ConvertFromMemory( prevNormals );
        DefaultNormals( mesh, normals.GetVerticies(), true );
        FillWithNormals( mesh, normals.GetVerticies() );
    }
    else
    {
        DefaultNormals( mesh, normals.GetVerticies(), false );
        FillWithNormals( mesh, normals.GetVerticies() );
    }

    if( curve != ExtrudeCurveType::None )
    {
        switch( curve )
        {
        case ExtrudeCurveType::Parabola:
            ApplyFunction( &DefaultExtrudePlugin::ParabolaCurve, mesh, normals, edges, corners );
            break;
        case ExtrudeCurveType::Cosinus:
            ApplyFunction( &DefaultExtrudePlugin::PeriodicCosinusCurve, mesh, normals, edges, corners );
            break;
        case ExtrudeCurveType::Gauss:
            ApplyFunction( &DefaultExtrudePlugin::GaussCurve, mesh, normals, edges, corners );
            break;
        case ExtrudeCurveType::Circle:
            ApplyFunction( &DefaultExtrudePlugin::CircleCurve, mesh, normals, edges, corners );
            break;
        default:
            assert( !"Shouldn't be here" );
        }

        ClampNormVecToDefaults( normals );
        FillWithNormals( mesh, normals.GetVerticies() );
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
    auto numIndicies = (int)indices.size();

    m_numUniqueExtrudedVerticies = numVerticies;
    m_numExtrudedVerticies = (int)numIndicies;

    // Add symetrical verticies
    for( int i = 0; i < numVerticies; ++i )
    {
        vertices.push_back( translate + vertices[ i ] );
    }

    for( int i = 0; i < numIndicies; i += 3 )
    {
        indices.push_back( indices[ i ] + (IndexType)numVerticies );
        indices.push_back( indices[ i + 2 ] + (IndexType)numVerticies );
        indices.push_back( indices[ i + 1 ] + (IndexType)numVerticies );
    }
}

// ***********************
// Function assumes that someone used function AddSymetricalPlane.
void    DefaultExtrudePlugin::AddSidePlanes           ( IndexedGeometry & mesh, std::vector< IndexType > & edges, std::vector< IndexType > & corners )
{
    // We will create new corners vector containing indicies to new verticies which will
    // be created in this function. Because each corner will have two separate verticies
    // new vector consist of pairs of indicies.
    std::vector< IndexType > cornerPairs;
    cornerPairs.resize( 2 * corners.size() );

    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();
    int numVerticies = (int)verticies.size();

    int symPlaneOffset = numVerticies / 2;                  // Symmetrical plane verticies offset from beginning of vertex buffer.
    int edgeRowLength = (int)edges.size() / 2 + (int)corners.size();  // Number of verticies in edge row.

    // In future we must add normals. That means we must add verticies too, bacause
    // edge is sharp and normals can't be the same.
    verticies.reserve( numVerticies + 2 * edgeRowLength );

// First row of verticies
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        verticies.push_back( verticies[ edges[ i ] ] );
    }
    // Duplicate corner verticies
    for( auto corner : corners )
    {
        verticies.push_back( verticies[ corner ] );
    }

// Second (translated) row of verticies
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        verticies.push_back( verticies[ edges[ i ] + symPlaneOffset ] );
    }
    // Duplicate corner verticies
    for( auto corner : corners )
    {
        verticies.push_back( verticies[ corner + symPlaneOffset ] );
    }


    // Replace edges indicies.
    // Edges array contains closed curves. Thats mean that every index occure two times.
    // We have to replace both with new indicies, but at first we take second vertex in pair.
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        auto idx = edges[ i + 1 ];

        // Find second appeariance of this vertex and replace it.
        int j = 0;
        for( ; j < (int)edges.size(); j += 2 )
        {
            if( edges[ j ] == idx )
            {
                edges[ i + 1 ] = numVerticies + j / 2;
                break;    // Consider using break here.
            }
        }

        // Corner verticies are separated from their pairs so we must check if current index isn't
        // corner vertex and replace it if it is. We replace always second index in pair (edges are directed!).
        for( int k = 0; k < (int)corners.size(); k++ )
        {
            if( idx == corners[ k ] )
            {
                // Additional corner verticies are at the end of array.
                edges[ i + 1 ] = numVerticies + ( (int)edges.size() >> 1 ) + k;

                // Here we complete new corners array.
                cornerPairs[ ( k << 1 ) + 1 ] = edges[ i + 1 ];
                cornerPairs[ ( k << 1 ) ] = numVerticies + j / 2;
            }
        }
    }

    // Now we replace first vertex in pair.
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        edges[ i ] = numVerticies + i / 2;
    }

    // Connect all verticies into triangles.
    ConnectVerticies( indices, edges, 0, edgeRowLength );

    // Replace content of old vector with new created corner vector.
    corners = std::move( cornerPairs );
}

// ***********************
//
void    DefaultExtrudePlugin::ApplyFunction           ( ExtrudeCurve curve, IndexedGeometry & mesh, IndexedGeometry & normalsVec, std::vector< IndexType > & edges, std::vector< IndexType > & cornerPairs )
{
    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();
    auto & normals = normalsVec.GetVerticies();

    int extrudeVertsBegin = 2 * m_numUniqueExtrudedVerticies;
    int edgeRowLength = (int)edges.size() / 2 + (int)cornerPairs.size() / 2;  // Number of verticies in single edge. Note: corner vector's size is double in comparision to previous functions.

    // Merge normals for corner verticies. These normals will be recreated in next functions.
    // We need this step to make special behavior of corner verticies possible.
    for( UInt32 i = 0; i < ( UInt32 )cornerPairs.size(); i += 2 )
    {
        int idx1 = cornerPairs[ i ];
        int idx2 = cornerPairs[ i + 1 ];

        glm::vec3 v1 = normals[ idx1 ];
        glm::vec3 v2 = normals[ idx2 ];

        glm::vec3 translateNormal = glm::normalize( v1 + v2 );

        float cos2Alpha = glm::dot( v1, v2 );
        float cosAlpha = glm::sqrt( 0.5f * cos2Alpha + 0.5f );
        float normalCoeff = glm::length( v1 ) / cosAlpha;

        normals[ idx1 ] = normalCoeff * translateNormal;
        normals[ idx2 ] = normals[ idx1 ];
    }

    // Add verticies between extruded planes.
    float delta = 1.0f / ( m_tesselation + 1 );
    for( int i = 1; i < m_tesselation + 1; ++i )
    {
        float division = i * delta;
        float moveCoeff = (this->*curve)( division ) * m_curveScale;

        for( int j = extrudeVertsBegin; j < extrudeVertsBegin + edgeRowLength; j++ )
        {
            glm::vec3 newVertex = verticies[ j ] * ( 1.0f - division ) + verticies[ j + edgeRowLength ] * division;
            newVertex += normals[ j ] * moveCoeff;

            verticies.push_back( newVertex );
        }
    }

    // Remove existing side plane.
    indices.erase( indices.begin() + ( 2 * m_numExtrudedVerticies ), indices.end() );

    // Connect all verticies
    ConnectVerticies( indices, edges, 0, 2 * edgeRowLength );
    for( int i = 2; i < m_tesselation + 1; ++i )
    {
        int offset1 = i * edgeRowLength;
        int offset2 = offset1 + edgeRowLength;
        ConnectVerticies( indices, edges, offset1, offset2 );
    }
    ConnectVerticies( indices, edges, ( m_tesselation + 1 ) * edgeRowLength, edgeRowLength );
}

// ***********************
//
void    DefaultExtrudePlugin::ConnectVerticies        ( std::vector< IndexType > & indicies, std::vector< IndexType > & edges, int offset1, int offset2 )
{
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        indicies.push_back( edges[ i ] + offset1 );
        indicies.push_back( edges[ i ] + offset2 );
        indicies.push_back( edges[ i + 1 ] + offset1 );

        indicies.push_back( edges[ i ] + offset2 );
        indicies.push_back( edges[ i + 1 ] + offset2 );
        indicies.push_back( edges[ i + 1 ] + offset1);
    }
}

// ***********************
//
void    DefaultExtrudePlugin::FillWithNormals         ( IndexedGeometry & mesh,
                                                       std::vector< glm::vec3 > & normals )
{
    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();

    normals.resize( verticies.size(), glm::vec3( 0.0, 0.0, 0.0 ) );

    for( int i = 2 * m_numExtrudedVerticies; i < (int)indices.size(); i += 6 )
    {
        glm::vec3 edgeDir = verticies[ indices[ i + 2 ] ] - verticies[ indices[ i ] ];
        glm::vec3 extrudeDir = verticies[ indices[ i + 1 ] ] - verticies[ indices[ i ] ];

        glm::vec3 normal = -glm::cross( edgeDir, extrudeDir );
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
//
void    DefaultExtrudePlugin::DefaultNormals          ( IndexedGeometry & mesh, std::vector< glm::vec3 > & normals, bool useExisting )
{
    auto & verticies = mesh.GetVerticies();
    normals.resize( verticies.size(), glm::vec3( 0.0, 0.0, 0.0 ) );

    if( useExisting )
    {
        // Normals have been copied from vertex attribute channel.
        // Copy and negate them to fill back plane.
        for( int i = m_numUniqueExtrudedVerticies; i < 2 * m_numUniqueExtrudedVerticies; ++i )
        {
            normals[ i ] = -normals[ i - m_numUniqueExtrudedVerticies ];
        }
    }
    else
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
}

// ***********************
//
void    DefaultExtrudePlugin::ClampNormVecToDefaults   ( IndexedGeometry & normals )
{
    auto & norm = normals.GetVerticies();
    norm.erase( norm.begin() + ( 2 * m_numUniqueExtrudedVerticies ), norm.end() );
}

// ***********************
// Edge is a pair of verticies that builds only one triangle in whole mesh.
// Note: Edges have their direction. Order of verticies counts. It's used later
// to determine normal direction as cross product between edge vector and extrude vector.
std::vector< IndexType >           DefaultExtrudePlugin::ExtractEdges ( IndexedGeometry& mesh )
{
    std::vector< IndexType >   edges;

    auto & indicies = mesh.GetIndicies();
    auto numIndicies = (int)indicies.size();

    edges.reserve( numIndicies / 3 );

    for( int i = 0; i < numIndicies; i += 3 )
    {
        AddOrRemoveEdge( edges, indicies[ i ], indicies[ i + 1 ] );
        AddOrRemoveEdge( edges, indicies[ i + 1 ], indicies[ i + 2 ] );
        AddOrRemoveEdge( edges, indicies[ i + 2 ], indicies[ i ] );
    }

    // Remove duplicate edges.
    for( int i = 0; i < edges.size(); i += 2 )
    {
        if( edges[ i ] == edges[ i + 1 ] )
        {
            edges.erase( edges.begin() + i, edges.begin() + ( i + 2 ) );
            i -= 2;
        }
    }

    return edges;
}

// ***********************
// Returns max( SizeType ) if Edge coundn't be found.
int                            DefaultExtrudePlugin::FindEdge  ( const std::vector< IndexType > & indicies, IndexType idx1, IndexType idx2 )
{
    for( int i = (int)indicies.size() - 1; i >= 0; i -= 2 )
    {
        if( /*( indicies[ i ] == idx2 && indicies[ i - 1 ] == idx1 ) ||*/
            ( indicies[ i ] == idx1 && indicies[ i - 1 ] == idx2 ) )
            return i - 1;
    }
    return std::numeric_limits< int >::max();
}

// ***********************
//
void                            DefaultExtrudePlugin::AddOrRemoveEdge   ( std::vector< IndexType > & edges, IndexType idx1, IndexType idx2 )
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
std::vector< IndexType >       DefaultExtrudePlugin::ExtractCorners          ( IndexedGeometry & mesh, const std::vector< IndexType > & edges, float angleThreshold )
{
    //float threshold = glm::radians( angleThreshold );

    auto & vertices = mesh.GetVerticies();
    std::vector< IndexType > corners;

    // This is very inefficeint way to do this. Map requires many memory allocations;
    std::map< IndexType, std::pair< glm::vec3, glm::vec3 > > edgeVectors;

    // Compute edge vectors.
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        IndexType idx1 = edges[ i ];
        IndexType idx2 = edges[ i + 1 ];

        glm::vec3 vert1 = vertices[ idx1 ];
        glm::vec3 vert2 = vertices[ idx2 ];

        glm::vec3 edgeVec = vert2 - vert1;

        edgeVectors[ idx1 ].second = edgeVec;
        edgeVectors[ idx2 ].first = -edgeVec;
    }

    // Compute angles between edge vectors and compare with threshold.
    for( auto iter = edgeVectors.begin(); iter != edgeVectors.end(); iter++ )
    {
        if( iter->second.first == glm::vec3( 0.0, 0.0, 0.0 ) || iter->second.second == glm::vec3( 0.0, 0.0, 0.0 ) )
            continue;

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

    auto compVertDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    VertexAttributesChannelDescriptor vaChannelDesc;
    vaChannelDesc.AddAttrChannelDesc( compVertDesc );

    //auto normalChannelDesc = static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() )->GetAttrChannelDescriptor( AttributeSemantic::AS_NORMAL );
//    if( normalChannelDesc )
    {
        vaChannelDesc.AddAttrChannelDesc( std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR ) );
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

