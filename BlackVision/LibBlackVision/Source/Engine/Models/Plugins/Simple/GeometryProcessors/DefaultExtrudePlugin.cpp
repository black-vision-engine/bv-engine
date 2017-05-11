#include "stdafx.h"
#include "DefaultExtrudePlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/HelperIndexedGeometryConverter.h"
#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include <glm/gtx/vector_angle.hpp>

#include <cmath>
#include <map>
#include <algorithm>

#include "UseLoggerLibBlackVision.h"


#include "Memory/MemoryLeaks.h"



DEFINE_ENUM_PARAMETER_CREATOR( bv::model::DefaultExtrudePlugin::ExtrudeCurveType );
DEFINE_ENUM_PARAMETER_CREATOR( bv::model::DefaultExtrudePlugin::BevelCurveType );


namespace bv { namespace model {




const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR           = "extrude vector";
const std::string        DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE   = "smooth threshold angle";
const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_CURVE            = "extrude curve";
const std::string        DefaultExtrudePlugin::PARAMS::CURVE_SCALE              = "curve scale";
const std::string        DefaultExtrudePlugin::PARAMS::COSINUS_CURVE_PERIOD     = "cosinus curve period";
const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_TESSELATION      = "tesselation";

const std::string        DefaultExtrudePlugin::PARAMS::BEVEL_HEIGHT             = "bevel height";
const std::string        DefaultExtrudePlugin::PARAMS::BEVEL_TESSELATION        = "bevel tesselation";
const std::string        DefaultExtrudePlugin::PARAMS::BEVEL_DEPTH_FRONT        = "bevel depth front";
const std::string        DefaultExtrudePlugin::PARAMS::BEVEL_DEPTH_BACK         = "bevel depth back";
const std::string        DefaultExtrudePlugin::PARAMS::BEVEL_CURVE_FRONT        = "bevel curve front";
const std::string        DefaultExtrudePlugin::PARAMS::BEVEL_CURVE_BACK         = "bevel curve back";
const std::string        DefaultExtrudePlugin::PARAMS::SYMETRICAL_BEVEL         = "symetrical bevel";



typedef ParamEnum< DefaultExtrudePlugin::ExtrudeCurveType > ParamCurveType;
typedef ParamEnum< DefaultExtrudePlugin::BevelCurveType > ParamBevelCurveType;




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
    
    helper.SetOrCreateVacModel();
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR, glm::vec3( 0.0, 0.0, -0.9 ), true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE, 160.0f, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::CURVE_SCALE, 0.2f, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::EXTRUDE_TESSELATION, 40, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::COSINUS_CURVE_PERIOD, 1, true, true );

    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::BEVEL_HEIGHT, 0.0f, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::BEVEL_TESSELATION, 1, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::BEVEL_DEPTH_FRONT, 0.3f, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::BEVEL_DEPTH_BACK, 0.3f, true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::SYMETRICAL_BEVEL, false, true, true );

    helper.AddEnumParam( DefaultExtrudePlugin::PARAMS::EXTRUDE_CURVE, DefaultExtrudePlugin::ExtrudeCurveType::Circle, true, true );
    helper.AddEnumParam( DefaultExtrudePlugin::PARAMS::BEVEL_CURVE_FRONT, DefaultExtrudePlugin::BevelCurveType::HalfSinus, true, true );
    helper.AddEnumParam( DefaultExtrudePlugin::PARAMS::BEVEL_CURVE_BACK, DefaultExtrudePlugin::BevelCurveType::Line, true, true );

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
// Processing
// ========================================================================= //

// ***********************
//
void        DefaultExtrudePlugin::ProcessConnectedComponent       ( model::ConnectedComponentPtr & currComponent,
                                                                    std::vector< IConnectedComponentPtr > & /*allComponents*/,
                                                                    PrimitiveType topology )
{
// ***********************
//
    // Get parameters values.
    glm::vec3 translate     = QueryTypedValue< ValueVec3Ptr >( GetValue( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR ) )->GetValue();
    float cornerThreshold   = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE ) )->GetValue();
    m_tesselation           = QueryTypedValue< ValueIntPtr >( GetValue( DefaultExtrudePlugin::PARAMS::EXTRUDE_TESSELATION ) )->GetValue();
    m_curveScale            = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::CURVE_SCALE ) )->GetValue();
    m_cosinusPeriod         = QueryTypedValue< ValueIntPtr >( GetValue( DefaultExtrudePlugin::PARAMS::COSINUS_CURVE_PERIOD ) )->GetValue();
    
    ExtrudeCurveType sideCurve      = QueryTypedParam< std::shared_ptr< ParamCurveType > >( GetParameter( PARAMS::EXTRUDE_CURVE ) )->Evaluate();
    BevelCurveType frontBevelCurve  = QueryTypedParam< std::shared_ptr< ParamBevelCurveType > >( GetParameter( PARAMS::BEVEL_CURVE_FRONT ) )->Evaluate();
    BevelCurveType backBevelCurve   = QueryTypedParam< std::shared_ptr< ParamBevelCurveType > >( GetParameter( PARAMS::BEVEL_CURVE_BACK ) )->Evaluate();
    
    m_bevelHeight                   = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::BEVEL_HEIGHT ) )->GetValue();
    float frontBevelDepth           = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::BEVEL_DEPTH_FRONT ) )->GetValue();
    float backBevelDepth            = QueryTypedValue< ValueFloatPtr >( GetValue( DefaultExtrudePlugin::PARAMS::BEVEL_DEPTH_BACK ) )->GetValue();
    int frontBevelTesselation       = QueryTypedValue< ValueIntPtr >( GetValue( DefaultExtrudePlugin::PARAMS::BEVEL_TESSELATION ) )->GetValue();
    int backBevelTesselation        = QueryTypedValue< ValueIntPtr >( GetValue( DefaultExtrudePlugin::PARAMS::BEVEL_TESSELATION ) )->GetValue();
    bool symetricalBevel            = QueryTypedValue< ValueBoolPtr >( GetValue( DefaultExtrudePlugin::PARAMS::SYMETRICAL_BEVEL ) )->GetValue();


// ***********************
//
// Check parameters values and set preconditions.

    // Check tesselation. 
    if( m_tesselation <= 0 )
        m_tesselation = 1;

    if( frontBevelTesselation <= 0 )
        frontBevelTesselation = 1;

    if( backBevelTesselation <= 0 )
        backBevelTesselation = 1;

    // Check curves types
    if( sideCurve >= ExtrudeCurveType::Total )
        sideCurve = ExtrudeCurveType::None;

    if( frontBevelCurve >= BevelCurveType::Total )
        frontBevelCurve = BevelCurveType::Line;

    if( backBevelCurve >= BevelCurveType::Total )
        backBevelCurve = BevelCurveType::Line;

    // Check bevel height. Should be greater then zero.
    if( m_bevelHeight - std::numeric_limits< float >::epsilon() < 0.0f )
    {
        // We don't need bevel curves. Line is enough.
        m_bevelHeight = 0.0f;
        frontBevelTesselation = 1;
        backBevelTesselation = 1;

        frontBevelCurve = BevelCurveType::Line;
        backBevelCurve = BevelCurveType::Line;
    }

    // Check bevel depth. Should be greater then zero.
    if( frontBevelDepth - std::numeric_limits< float >::epsilon() < 0.0f )
    {
        frontBevelCurve = BevelCurveType::Line;
        frontBevelTesselation = 1;
        m_bevelHeight = 0;
    }

    if( backBevelDepth - std::numeric_limits< float >::epsilon() < 0.0f )
    {
        backBevelCurve = BevelCurveType::Line;
        backBevelTesselation = 1;
        m_bevelHeight = 0;
    }

    // Apply symetry.
    if( symetricalBevel )
    {
        backBevelDepth = frontBevelDepth;
        backBevelTesselation = frontBevelTesselation;
        backBevelCurve = frontBevelCurve;
    }


// ***********************
//
// Get previous plugin geometry channels. Index geometry.
    auto connComp = ConnectedComponent::Create();

    Float3AttributeChannelPtr newPositions = CreatePositionsChannel( currComponent, connComp );
    Float3AttributeChannelPtr normalsChannel = CreateNormalsChannel( currComponent, connComp );
    Float2AttributeChannelPtr uvsChannel = CreateUVsChannel( currComponent, connComp );


    IndexedGeometry mesh;
    IndexedGeometryConverter converter;
    converter.RememberConversionIndicies( true );   // Use this converter for other channels

    auto positions = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );
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
        LOG_MESSAGE( SeverityLevel::error ) << "[PluginExtrude] input topology: [" << SerializationHelper::T2String( topology ) << "] is not supported.";
        //assert( !"This primitive topology is not supported" );
        return;
    }

    if( mesh.GetVerticies().size() == 0 )
        return;

    m_numUniqueExtrudedVerticies = ( int )mesh.GetVerticies().size();
    m_numExtrudedVerticies = ( int )mesh.GetIndicies().size();


// ***********************
//
// Extract edges and corners from flat geometry.
    auto edges = ExtractEdges( mesh );
    auto corners = ExtractCorners( mesh, edges, cornerThreshold, translate );

    //DebugPrintToFile( "ExtrudeDebug.txt", mesh.GetVerticies(), edges, corners );


// ***********************
//
// Copy uvs from previous vertex attributes channel or generate defaults.
    std::vector< glm::vec2 > uvs;
    uvs.reserve( mesh.GetVerticies().size() );

    auto prevUVs = std::static_pointer_cast< Float2AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) );
    if( prevUVs )
    {
        uvs = converter.ConvertFromMemory( prevUVs );
        DefaultUVs( mesh, uvs, true );
        CopyUVsOnSideFaces( uvs, edges, corners.Indicies );
    }
    else
    {
        DefaultUVs( mesh, uvs, false );
        CopyUVsOnSideFaces( uvs, edges, corners.Indicies );
    }


// ***********************
//
// Extrude front plane and add side planes.
    AddSymetricalPlane( mesh, translate );
    AddSidePlanes( mesh, edges, corners );

    SizeType sideContourLength = ComputeContourLength( edges, corners );     // Number of verticies in one contour of side surface.
    SizeType sidePlanesOffset = 2 * m_numUniqueExtrudedVerticies;           // First free space after symetrical planes.
    SizeType curOffset = sidePlanesOffset + 2 * sideContourLength;

    SizeType curveOffsets[ 6 ];                                             // Offsets of start and end contours for curves.

    const int FrontBevel1 = 0;
    const int FrontBevel2 = 1;
    const int MiddleExtrude1 = 2;
    const int MiddleExtrude2 = 3;
    const int BackBevel1 = 4;
    const int BackBevel2 = 5;


    if( frontBevelCurve != BevelCurveType::Total )
    {
        glm::vec3 translation = glm::normalize( translate ) * frontBevelDepth;

        curveOffsets[ FrontBevel1 ] = sidePlanesOffset;
        curveOffsets[ FrontBevel2 ] = curOffset;
        curOffset += sideContourLength;      // We will add one contour.

        // Note: we will connect first side plane with contour generated here.
        CopyTranslate( mesh, translation, sidePlanesOffset, sideContourLength );
    }

    if( sideCurve != ExtrudeCurveType::Total )
    {
        glm::vec3 frontTranslate = glm::normalize( translate ) * frontBevelDepth;
        glm::vec3 backTranslate = translate - glm::normalize( translate ) * backBevelDepth;

        curveOffsets[ MiddleExtrude1 ] = curOffset;
        curveOffsets[ MiddleExtrude2 ] = curOffset + sideContourLength;
        curOffset += 2 * sideContourLength;      // We will add two contours.

        // Translate always relative to first side plane.
        CopyTranslate( mesh, frontTranslate, sidePlanesOffset, sideContourLength );
        CopyTranslate( mesh, backTranslate, sidePlanesOffset, sideContourLength );
    }

    if( backBevelCurve != BevelCurveType::Total )
    {
        glm::vec3 backTranslate = translate - glm::normalize( translate ) * backBevelDepth;

        curveOffsets[ BackBevel1 ] = curOffset;
        curveOffsets[ BackBevel2 ] = sidePlanesOffset + sideContourLength;       // This is second side plane generated in AddSidePlanes function.
        curOffset += sideContourLength;      // We will add one contour.

        // We will connect contour generated here with back side plane.
        // Note: we translate front side plane.
        CopyTranslate( mesh, backTranslate, sidePlanesOffset, sideContourLength );
    }

    // Connect planes into triangles. We need this step for normals generation.
    // These indicies wiil be in future replaced with curved surfaces.
    // Note: In previous versions this step was performed by AddSidePlanes function, but only for two rows of verticies.
    for( SizeType i = 0; i < 6; i += 2 )
    {
        // Note: offset1 and offset2 must be relative to first side plane. Values in curveOffsets are relative to beginning of verticies vector.
        int offset1 = int( curveOffsets[ i ] - sidePlanesOffset );
        int offset2 = int( curveOffsets[ i + 1 ] - sidePlanesOffset );

        ConnectVerticies( mesh.GetIndicies(), edges, offset1, offset2 );
    }
    

// ***********************
//
// Copy normals from previous vertex attributes channel or generate defaults.
    IndexedGeometry normals;
    normalsChannel->GetVertices().reserve( mesh.GetVerticies().size() );

    auto prevNormals = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_NORMAL ) );
    if( prevNormals )
    {
        normals.GetVerticies() = converter.ConvertFromMemory( prevNormals );
        DefaultNormals( mesh, normals.GetVerticies(), true );
        FillWithNormals( mesh, normals.GetVerticies(), edges, corners );
        FillCornerNormals( mesh, normals.GetVerticies(), edges, corners );
    }
    else
    {
        DefaultNormals( mesh, normals.GetVerticies(), false );
        FillWithNormals( mesh, normals.GetVerticies(), edges, corners );
        FillCornerNormals( mesh, normals.GetVerticies(), edges, corners );
    }



// ***********************
//
// Generate curved side surfaces.

    auto & indicies = mesh.GetIndicies();
    indicies.erase( indicies.begin() + ( 2 * m_numExtrudedVerticies ), indicies.end() );


    // Generate bevel front surface
    GenerateSideFace( frontBevelCurve, mesh, normals, edges, corners, curveOffsets[ FrontBevel1 ], curveOffsets[ FrontBevel2 ], frontBevelTesselation, m_bevelHeight, false );

    // Generate side surface
    GenerateSideFace( sideCurve, mesh, normals, edges, corners, curveOffsets[ MiddleExtrude1 ], curveOffsets[ MiddleExtrude2 ], m_tesselation, m_curveScale, m_bevelHeight );

    // Generate back surface
    GenerateSideFace( backBevelCurve, mesh, normals, edges, corners, curveOffsets[ BackBevel1 ], curveOffsets[ BackBevel2 ], backBevelTesselation, m_bevelHeight, true );


    ClampNormVecToDefaults( normals );
    FillWithNormals( mesh, normals.GetVerticies(), edges, corners );

// ***********************
//
// Deal with UVs

    FillRestUVs( mesh, uvs );

// ***********************
//
// Convert indexed geometry to triangles.
    converter.MakeTriangles( mesh, newPositions );
    converter.MakeTriangles( normals.GetVerticies(), mesh.GetIndicies(), normalsChannel );
    converter.MakeTriangles( uvs, mesh.GetIndicies(), uvsChannel );
    
    m_vaChannel->AddConnectedComponent( connComp );
}

// ***********************
//
void    DefaultExtrudePlugin::AddSymetricalPlane      ( IndexedGeometry& mesh, glm::vec3 translate )
{
    auto & vertices = mesh.GetVerticies();
    auto & indices = mesh.GetIndicies();
    auto numVerticies = vertices.size();

    // Add symetrical verticies
    for( int i = 0; i < m_numUniqueExtrudedVerticies; ++i )
    {
        vertices.push_back( translate + vertices[ i ] );
    }

    for( int i = 0; i < m_numExtrudedVerticies; i += 3 )
    {
        indices.push_back( indices[ i ] + (IndexType)numVerticies );
        indices.push_back( indices[ i + 2 ] + (IndexType)numVerticies );
        indices.push_back( indices[ i + 1 ] + (IndexType)numVerticies );
    }
}

// ***********************
// Function assumes that someone used function AddSymetricalPlane.
void    DefaultExtrudePlugin::AddSidePlanes           ( IndexedGeometry & mesh, std::vector< IndexType > & edges, CornersInfo & corners )
{
    // We will create new corners vector containing indicies to new verticies which will
    // be created in this function. Because each corner will have two separate verticies
    // new vector consist of pairs of indicies.
    CornersInfo cornerPairs;
    cornerPairs.Indicies.resize( 2 * corners.Indicies.size() );
    cornerPairs.IsConvex.resize( 2 * corners.Indicies.size() );
    cornerPairs.Normals.resize( corners.Normals.size() );

    //auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();
    int numVerticies = (int)verticies.size();

    int symPlaneOffset = numVerticies / 2;                  // Symmetrical plane verticies offset from beginning of vertex buffer.
    int edgeRowLength = (int)edges.size() / 2 + 3 * (int)corners.Indicies.size();  // Number of verticies in edge row.

    // In future we must add normals. That means we must add verticies too, bacause
    // edge is sharp and normals can't be the same.
    verticies.reserve( numVerticies + 2 * edgeRowLength );

// First row of verticies
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        verticies.push_back( verticies[ edges[ i ] ] );
    }

    for( int i = 0; i < 3; ++i )
    {
        // Duplicate corner verticies. We need 3 times verticies more. At first we split vertex into two seperate verticies
        // that will be moved along normal (first row). Last two rows will create corner (we need two, because we have separate normal vector).
        for( auto corner : corners.Indicies )
        {
            verticies.push_back( verticies[ corner ] );
        }
    }

// Second (translated) row of verticies
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        verticies.push_back( verticies[ edges[ i ] + symPlaneOffset ] );
    }

    for( int i = 0; i < 3; ++i )
    {
        // Duplicate corner verticies. We need 3 times verticies more. At first we split vertex into two seperate verticies
        // that will be moved along normal (first row). Last two rows will create corner (we need two, because we have separate normal vector).
        for( auto corner : corners.Indicies )
        {
            verticies.push_back( verticies[ corner + symPlaneOffset ] );
        }
    }


    // Side plane should contain only edge. We must remove what's inside of contour.
    // This code replaces edges indicies.
    // Edges array contains closed curves. Thats mean that every index occures two times.
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
        for( int k = 0; k < (int)corners.Indicies.size(); k++ )
        {
            if( idx == corners.Indicies[ k ] )
            {
                // Additional corner verticies are at the end of array.
                edges[ i + 1 ] = numVerticies + ( (int)edges.size() >> 1 ) + k;

                // Here we complete new corners array.
                int cornerIdx = 2 * k + 1;
                cornerPairs.Indicies[ cornerIdx ] = edges[ i + 1 ];
                cornerPairs.IsConvex[ cornerIdx ] = corners.IsConvex[ k ];
                cornerPairs.Normals[ cornerIdx ] = corners.Normals[ 2 * k ];        // Note: every corner before duplication has two normals.
                
                cornerIdx = 2 * k;
                cornerPairs.Indicies[ cornerIdx ] = numVerticies + j / 2;
                cornerPairs.IsConvex[ cornerIdx ] = corners.IsConvex[ k ];
                cornerPairs.Normals[ cornerIdx ] = corners.Normals[ 2 * k + 1 ];    // Note: every corner before duplication has two normals.
            }
        }
    }

    // Now we replace first vertex in pair.
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        edges[ i ] = numVerticies + i / 2;
    }

    // Add edges connecting additional corner verticies.
    IndexType numEdgesVerticies = ( IndexType )edges.size() / 2;
    for( int i = 0; i < cornerPairs.Indicies.size(); i += 2 )
    {
        IndexType idx = i / 2;

        IndexType newCornerIdx1 = ( IndexType )numVerticies + numEdgesVerticies + ( IndexType )corners.Indicies.size() + idx;
        IndexType newCornerIdx2 = ( IndexType )numVerticies + numEdgesVerticies + 2 * ( IndexType )corners.Indicies.size() + idx;

        edges.push_back( newCornerIdx1 );
        edges.push_back( cornerPairs.Indicies[ i ] );

        edges.push_back( cornerPairs.Indicies[ i + 1 ] );
        edges.push_back( newCornerIdx2 );

        cornerPairs.Indicies[ i ] = newCornerIdx1;
        cornerPairs.Indicies[ i + 1 ] = newCornerIdx2;
    }

    // Replace content of old vector with new created corner vector.
    corners = std::move( cornerPairs );
}

// ***********************
//
void    DefaultExtrudePlugin::CopyTranslate           ( IndexedGeometry & mesh, glm::vec3 translate, SizeType referenceOffset, SizeType numVerticies )
{
    auto & verticies = mesh.GetVerticies();

    for( SizeType i = 0; i < numVerticies; i++ )
    {
        verticies.push_back( verticies[ referenceOffset + i ] + translate );
    }
}

// ***********************
//
void    DefaultExtrudePlugin::ApplyFunction           ( ExtrudeCurve curve,
                                                        IndexedGeometry & mesh,
                                                        IndexedGeometry & normalsVec,
                                                        std::vector< IndexType > & edges,
                                                        CornersInfo & cornerPairs,
                                                        SizeType beginContourOffset,
                                                        SizeType endContourOffset,
                                                        int tesselation,
                                                        float scaleCurve,
                                                        float offsetCurve,
                                                        bool mirrorFunction
                                                      )
{
    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();
    auto & normals = normalsVec.GetVerticies();

    SizeType curveOffset = verticies.size();
    int edgeRowLength = ( int )ComputeContourLength( edges, cornerPairs );  // Number of verticies in single edge. Note: corner vector's size is double size in comparision to previous functions.

    // Edges and corners are already offseted relative to first contour row.
    SizeType relativeBeginContourOffset = beginContourOffset - 2 * m_numUniqueExtrudedVerticies;
    SizeType relativeEndContourOffset = endContourOffset - 2 * m_numUniqueExtrudedVerticies;
    SizeType relativeCurveOffset = curveOffset - 2 * m_numUniqueExtrudedVerticies;


    // Add verticies between extruded planes.
    float delta = 1.0f / ( tesselation + 1 );
    for( int i = 1; i < tesselation + 1; ++i )
    {
        float division = i * delta;
        float samplePoint = mirrorFunction ? 1.0f - division : division;        // X coordinate of curve.

        float moveCoeff = ( this->*curve )( samplePoint ) * scaleCurve + offsetCurve;

        for( int j = 0; j < edgeRowLength; j++ )
        {
            glm::vec3 newVertex = verticies[ beginContourOffset + j ] * ( 1.0f - division ) + verticies[ endContourOffset + j ] * division;
            newVertex += normals[ beginContourOffset + j ] * moveCoeff;

            verticies.push_back( newVertex );
        }
    }

    // Move reference verticies of start contour and end contour.
    {
        float division = mirrorFunction ? 1.0f : 0.0f;
        float moveCoeff = ( this->*curve )( division ) * scaleCurve + offsetCurve;

        for( SizeType i = beginContourOffset; i < beginContourOffset + edgeRowLength; i++ )
        {
            verticies[ i ] += normals[ i ] * moveCoeff;
        }

        division = mirrorFunction ? 0.0f : 1.0f;
        moveCoeff = ( this->*curve )( division ) * scaleCurve + offsetCurve;

        for( SizeType i = endContourOffset; i < endContourOffset + edgeRowLength; i++ )
        {
            verticies[ i ] += normals[ i ] * moveCoeff;
        }
    }


    // Connect all verticies
    ConnectVerticies( indices, edges, (int)relativeBeginContourOffset, (int)relativeCurveOffset );
    for( int i = 2; i < tesselation + 1; ++i )
    {
        int offset1 = (int)relativeCurveOffset + ( i - 2 ) * edgeRowLength;
        int offset2 = offset1 + edgeRowLength;
        ConnectVerticies( indices, edges, offset1, offset2 );
    }
    ConnectVerticies( indices, edges, (int)relativeCurveOffset + ( tesselation - 1 ) * edgeRowLength, (int)relativeEndContourOffset );
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
// This function makes temporary normals for further processing only.
void    DefaultExtrudePlugin::FillCornerNormals     ( IndexedGeometry & mesh, std::vector< glm::vec3 > & normals, const std::vector< IndexType > & edges, const CornersInfo & cornerPairs )
{
    auto & verticies = mesh.GetVerticies();

    // Merge normals for corner verticies. We need this step to make special behavior of corner verticies possible.
    // Verticies will be moved along these normals. Proper normals will be recreated in next functions.
    for( UInt32 i = 0; i < ( UInt32 )cornerPairs.Indicies.size(); i += 2 )
    {
        SizeType idx1 = cornerPairs.Indicies[ i ];
        SizeType idx2 = cornerPairs.Indicies[ i + 1 ];

        glm::vec3 n1 = cornerPairs.Normals[ i ];
        glm::vec3 n2 = cornerPairs.Normals[ i + 1 ];

        glm::vec3 translateNormal = glm::vec3( 0.0, 0.0, 0.0 );
        glm::vec3 n1n2Sum = n1 + n2;

        if( n1n2Sum != glm::vec3( 0.0, 0.0, 0.0 ) )
            translateNormal = glm::normalize( n1n2Sum );

        float cos2Alpha = glm::dot( n1, n2 );
        float cosAlpha = glm::sqrt( 0.5f * cos2Alpha + 0.5f );
        float normalCoeff = glm::length( n1 ) / cosAlpha;

        if( !cornerPairs.IsConvex[ i ] )
        {
            SizeType adjVert1Idx = std::numeric_limits< SizeType >::max();;
            SizeType adjVert2Idx = std::numeric_limits< SizeType >::max();

            for( SizeType j = 0; j < edges.size(); ++j )
            {
                if( edges[ j ] == cornerPairs.Indicies[ i ] )
                    adjVert1Idx = j % 2 ? edges[ j - 1 ] : edges[ j + 1 ];

                if( edges[ j ] == cornerPairs.Indicies[ i + 1 ] )
                    adjVert2Idx = j % 2 ? edges[ j - 1 ] : edges[ j + 1 ];
            }

            assert( adjVert1Idx < verticies.size() );
            assert( adjVert2Idx < verticies.size() );

            glm::vec3 & cornerVertex = verticies[ cornerPairs.Indicies[ i ] ];
            glm::vec3 & adjVert1 = verticies[ adjVert1Idx ];
            glm::vec3 & adjVert2 = verticies[ adjVert2Idx ];

            float adjLength1 = glm::length( cornerVertex - adjVert1 );
            float adjLength2 = glm::length( cornerVertex - adjVert2 );

            float minLength = std::min< float >( adjLength1, adjLength2 );

            if( minLength < normalCoeff * m_bevelHeight )
                normalCoeff = 1.0f;
        }

        normals[ idx1 ] = normalCoeff * translateNormal;
        normals[ idx2 ] = normals[ idx1 ];
    }


    // Fill remaining contours with normals copied from first contour.
    SizeType edgeRowLength = ComputeContourLength( edges, cornerPairs );
    SizeType numContours = ( verticies.size() - 2 * m_numUniqueExtrudedVerticies ) / edgeRowLength;

    for( int i = 1; i < numContours; ++i )
    {
        for( int j = 0; j < cornerPairs.Indicies.size(); j += 2 )
        {
            SizeType idx1 = cornerPairs.Indicies[ j ];

            SizeType idx3 = i * edgeRowLength + cornerPairs.Indicies[ j ];
            SizeType idx4 = i * edgeRowLength + cornerPairs.Indicies[ j + 1 ];

            normals[ idx3 ] = normals[ idx1 ];
            normals[ idx4 ] = normals[ idx1 ];
        }
    }
}

// ***********************
//
void    DefaultExtrudePlugin::FillWithNormals         ( IndexedGeometry & mesh,
                                                        std::vector< glm::vec3 > & normals,
                                                        const std::vector< IndexType > & edges,
                                                        const CornersInfo & cornerPairs )
{
    auto & indices = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();

    normals.resize( verticies.size(), glm::vec3( 0.0, 0.0, 0.0 ) );

    { cornerPairs;  }
    { edges;  }

    SizeType numEdgeRowIndicies = 6 * edges.size();
    SizeType cornersOffset = 2 * m_numExtrudedVerticies + 3 * ( edges.size() - 2 * cornerPairs.Indicies.size() );


    for( int i = 2 * m_numExtrudedVerticies; i < (int)indices.size(); i += 6 )
    {
        if( i == cornersOffset )
        {
            // Ignore corners. They will introduce artifakts.
            i += 6 * ( int )cornerPairs.Indicies.size() - 6;
            cornersOffset += numEdgeRowIndicies;
            continue;
        }

        glm::vec3 edgeDir = verticies[ indices[ i + 2 ] ] - verticies[ indices[ i ] ];
        glm::vec3 extrudeDir = verticies[ indices[ i + 1 ] ] - verticies[ indices[ i ] ];

        glm::vec3 normal = -glm::cross( edgeDir, extrudeDir );
        if( normal != glm::vec3( 0.0, 0.0, 0.0 ) &&
            !glm::isnan( normal.x ) &&
            !glm::isnan( normal.y ) &&
            !glm::isnan( normal.z ) )
            normal = glm::normalize( normal );
        else
            normal = glm::vec3( 0.0, 0.0, 0.0 );

        normals[ indices[ i ] ] += normal;
        normals[ indices[ i + 1 ] ] += normal;
        normals[ indices[ i + 2 ] ] += normal;
        normals[ indices[ i + 4 ] ] += normal;
    }


    //// Fill remaining contours with normals copied from first contour.
    //SizeType edgeRowLength = ComputeContourLength( edges, cornerPairs );
    //SizeType numContours = ( verticies.size() - 2 * m_numUniqueExtrudedVerticies ) / edgeRowLength;

    //for( int i = 1; i < numContours; ++i )
    //{
    //    for( int j = 0; j < cornerPairs.Indicies.size(); j += 2 )
    //    {
    //        //SizeType idx1 = cornerPairs.Normals[ j ];
    //        //SizeType idx2 = cornerPairs.Normals[ j + 1 ];

    //        SizeType idx3 = i * edgeRowLength + cornerPairs.Indicies[ j ];
    //        SizeType idx4 = i * edgeRowLength + cornerPairs.Indicies[ j + 1 ];

    //        normals[ idx3 ] = cornerPairs.Normals[ j ];
    //        normals[ idx4 ] = cornerPairs.Normals[ j + 1 ];
    //    }
    //}

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
//
void    DefaultExtrudePlugin::DefaultUVs                ( IndexedGeometry & mesh, std::vector< glm::vec2 > & uvs, bool useExisting )
{
    if( !useExisting )
    {
        Helper::UVGenerator::GenerateUV( mesh.GetVerticies(), 0, m_numUniqueExtrudedVerticies, uvs, glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, 1.0, 0.0 ), true );
    }

    uvs.resize( 2 * m_numUniqueExtrudedVerticies, glm::vec2( 0.0, 0.0 ) );
    
    // Uvs have been copied from vertex attribute channel.
    // Copy them to fill back plane.
    for( int i = m_numUniqueExtrudedVerticies; i < 2 * m_numUniqueExtrudedVerticies; ++i )
    {
        uvs[ i ] = uvs[ i - m_numUniqueExtrudedVerticies ];
    }

}

// ***********************
//
void    DefaultExtrudePlugin::CopyUVsOnSideFaces            ( std::vector< glm::vec2 > & uvs, std::vector< IndexType > & edges, std::vector< IndexType > & corners )
{
    //auto first 2 * m_numUniqueExtrudedVerticies;

    // First row of verticies
    for( int i = 0; i < ( int )edges.size(); i += 2 )
    {
        uvs.push_back( uvs[ edges[ i ] ] );
    }

    for( int i = 0; i < 3; ++i )
    {
        // Duplicate corner verticies
        for( auto corner : corners )
        {
            uvs.push_back( uvs[ corner ] );
        }
    }
}

// ***********************
//
void    DefaultExtrudePlugin::FillRestUVs                   ( IndexedGeometry & mesh, std::vector< glm::vec2 > & uvs )
{
    SizeType numRemainingUVs = mesh.GetVerticies().size() - uvs.size();
    SizeType firstUVToCopy = 2 * m_numUniqueExtrudedVerticies;
    SizeType numUVsInContour = uvs.size() - firstUVToCopy;

    SizeType curUVIdx = 0;
    while( curUVIdx < numRemainingUVs )
    {
        uvs.push_back( uvs[ firstUVToCopy + curUVIdx % numUVsInContour ] );

        curUVIdx++;
    }
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

struct EdgeInfo
{
    glm::vec3       EdgeVec1;
    glm::vec3       EdgeVec2;
    IndexType       AdjacentVertex1;
    IndexType       AdjacentVertex2;
};

// ***********************
//
DefaultExtrudePlugin::CornersInfo       DefaultExtrudePlugin::ExtractCorners          ( IndexedGeometry & mesh, const std::vector< IndexType > & edges, float angleThreshold, glm::vec3 & extrudeDir )
{
    //float threshold = glm::radians( angleThreshold );
    float cosThreshold = cos( glm::radians( angleThreshold ) );

    auto & vertices = mesh.GetVerticies();
    CornersInfo corners;

    // This is very inefficeint way to do this. Map requires many memory allocations;
    std::map< IndexType, EdgeInfo > edgeVectors;

    // Compute edge vectors.
    for( int i = 0; i < (int)edges.size(); i += 2 )
    {
        IndexType idx1 = edges[ i ];
        IndexType idx2 = edges[ i + 1 ];

        glm::vec3 vert1 = vertices[ idx1 ];
        glm::vec3 vert2 = vertices[ idx2 ];

        glm::vec3 edgeVec = vert2 - vert1;

        edgeVectors[ idx1 ].EdgeVec2 = edgeVec;
        edgeVectors[ idx1 ].AdjacentVertex2 = idx2;
        edgeVectors[ idx2 ].EdgeVec1 = -edgeVec;
        edgeVectors[ idx2 ].AdjacentVertex1 = idx1;
    }

    // Compute angles between edge vectors and compare with threshold.
    for( auto iter = edgeVectors.begin(); iter != edgeVectors.end(); iter++ )
    {
        if( iter->second.EdgeVec1 == glm::vec3( 0.0, 0.0, 0.0 ) || iter->second.EdgeVec2 == glm::vec3( 0.0, 0.0, 0.0 ) )
            continue;

        glm::vec3 edge1 = glm::normalize( iter->second.EdgeVec1 );
        glm::vec3 edge2 = glm::normalize( iter->second.EdgeVec2 );

        float angle = glm::dot( edge1, edge2 );
        if( angle > cosThreshold )
        {
            corners.Indicies.push_back( iter->first );

            glm::vec3 edge1Normal = glm::normalize( -glm::cross( -edge1, extrudeDir ) );
            glm::vec3 edge2Normal = glm::normalize( -glm::cross( edge2, extrudeDir ) );
            
            corners.Normals.push_back( edge2Normal );
            corners.Normals.push_back( edge1Normal );

            if( glm::cross( edge1, edge2 ).z >= 0 )
            {
                corners.IsConvex.push_back( false );
                //{ corners; }
            }
            else
            {
                corners.IsConvex.push_back( true );
                
            }
        }
    }

    return corners;
}

// ***********************
//
SizeType                DefaultExtrudePlugin::ComputeContourLength      ( const std::vector< IndexType > & edges, const CornersInfo & corners ) const
{
    return edges.size() / 2 + corners.Indicies.size() / 2;
}


// ========================================================================= //
// Default Processing function override
// ========================================================================= //

// ***********************
// Function always converts geometry channel to PrimitiveType::PT_TRIANGLES.
// This is the only difference between base class function and this override.
void                                DefaultExtrudePlugin::ProcessVertexAttributesChannel  ()
{
    if( !( GetPrevPlugin() && GetPrevPlugin()->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = GetPrevPlugin()->GetVertexAttributesChannel();

    auto compVertDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    VertexAttributesChannelDescriptor vaChannelDesc;
    vaChannelDesc.AddAttrChannelDesc( compVertDesc );

    //auto normalChannelDesc = static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() )->GetAttrChannelDescriptor( AttributeSemantic::AS_NORMAL );
//    if( normalChannelDesc )
    {
        vaChannelDesc.AddAttrChannelDesc( std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR ) );
        vaChannelDesc.AddAttrChannelDesc( std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR ) );
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

// ***********************
//
void        DefaultExtrudePlugin::DebugPrintToFile  ( const std::string & fileName, const std::vector< glm::vec3 > & verticies, const std::vector< IndexType > & edges, const std::vector< IndexType > & corners )
{
    std::fstream file( fileName, std::ios_base::app );
    assert( !file.fail() );

    file << "Next contour: " << std::endl;

    // Print edges.
    file << "Edges: " << std::endl;
    for( int i = 0; i < edges.size(); i += 2 )
    {
        file << "( " << edges[ i ] << ", " << edges[ i + 1 ] << " ) - ";
        DebugPrint( file, verticies[ edges[ i ] ] );
        DebugPrint( file, verticies[ edges[ i + 1 ] ] );
        file << std::endl;
    }
    file << std::endl;

    // Print corners.
    file << "Corners: " << std::endl;
    for( int i = 0; i < corners.size(); ++i )
    {
        file << corners[ i ] << " - ";
        DebugPrint( file, verticies[ corners[ i ] ] );
        file << std::endl;
    }
    file << std::endl;

    file.close();
}

// ***********************
//
void        DefaultExtrudePlugin::DebugPrint    ( std::fstream & file, glm::vec3 vertex )
{
    file << "( " << vertex.x << ", " << vertex.y << ", " << vertex.z << " )";
}

// ***********************
//
Float3AttributeChannelPtr           DefaultExtrudePlugin::CreateNormalsChannel      ( ConnectedComponentPtr & /*prevComponent*/, ConnectedComponentPtr & newComponent )
{
    auto normChannelDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_PROCESSOR );
    Float3AttributeChannelPtr normalsChannel = std::make_shared< Float3AttributeChannel >( normChannelDesc, normChannelDesc->SuggestedDefaultName( 0 ), true );

    newComponent->AddAttributeChannel( normalsChannel );
    return normalsChannel;
}

// ***********************
//
Float3AttributeChannelPtr           DefaultExtrudePlugin::CreatePositionsChannel    ( ConnectedComponentPtr & prevComponent, ConnectedComponentPtr & newComponent )
{
    auto positions = std::static_pointer_cast< Float3AttributeChannel >( prevComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );
    assert( positions );    if( !positions ) return nullptr;

    auto posChannelDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_PROCESSOR );
    auto newPositions = std::make_shared< model::Float3AttributeChannel >( posChannelDesc, positions->GetName(), true );

    newComponent->AddAttributeChannel( newPositions );

    return newPositions;
}

// ***********************
//
Float2AttributeChannelPtr           DefaultExtrudePlugin::CreateUVsChannel          ( ConnectedComponentPtr & /*prevComponent*/, ConnectedComponentPtr & newComponent )
{
    auto uvsChannelDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    Float2AttributeChannelPtr uvsChannel = std::make_shared< Float2AttributeChannel >( uvsChannelDesc, uvsChannelDesc->SuggestedDefaultName( 0 ), true );

    newComponent->AddAttributeChannel( uvsChannel );
    return uvsChannel;
}


// ========================================================================= //
// Generation of side faces
// ========================================================================= //


// ***********************
//
void    DefaultExtrudePlugin::GenerateSideFace      (   BevelCurveType curve,
                                                        IndexedGeometry & mesh,
                                                        IndexedGeometry & normals,
                                                        std::vector< IndexType > & edges,
                                                        CornersInfo & corners,
                                                        SizeType beginContourOffset,
                                                        SizeType endContourOffset,
                                                        int tesselation,
                                                        float bevelHeight,
                                                        bool backBevelFace )
{
    switch( curve )
    {
    case DefaultExtrudePlugin::BevelCurveType::Line:
        ApplyFunction( &DefaultExtrudePlugin::LineCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    case DefaultExtrudePlugin::BevelCurveType::HalfSinus:
        ApplyFunction( &DefaultExtrudePlugin::HalfSinusCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    case DefaultExtrudePlugin::BevelCurveType::InverseHalfSinus:
        ApplyFunction( &DefaultExtrudePlugin::InverseHalfSinusCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    case DefaultExtrudePlugin::BevelCurveType::Sinus:
        ApplyFunction( &DefaultExtrudePlugin::SinusCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    case DefaultExtrudePlugin::BevelCurveType::Circle:
        ApplyFunction( &DefaultExtrudePlugin::CircleBevelCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    case DefaultExtrudePlugin::BevelCurveType::InverseCircle:
        ApplyFunction( &DefaultExtrudePlugin::InverseCircleBevelCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    case DefaultExtrudePlugin::BevelCurveType::Harmonic:
        ApplyFunction( &DefaultExtrudePlugin::HarmonicCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    case DefaultExtrudePlugin::BevelCurveType::InverseHarmonic:
        ApplyFunction( &DefaultExtrudePlugin::InverseHarmonicCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
        break;
    default:
        assert( !"Shouldn't be here" );
        ApplyFunction( &DefaultExtrudePlugin::LineCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, bevelHeight, 0, backBevelFace );
    }
}

// ***********************
//
void    DefaultExtrudePlugin::GenerateSideFace      (   ExtrudeCurveType curve,
                                                        IndexedGeometry & mesh,
                                                        IndexedGeometry & normals,
                                                        std::vector< IndexType > & edges,
                                                        CornersInfo & corners,
                                                        SizeType beginContourOffset,
                                                        SizeType endContourOffset,
                                                        int tesselation,
                                                        float curveScale,
                                                        float bevelHeight )
{
    switch( curve )
    {
    case ExtrudeCurveType::Parabola:
        ApplyFunction( &DefaultExtrudePlugin::ParabolaCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, curveScale, bevelHeight );
        break;
    case ExtrudeCurveType::Cosinus:
        ApplyFunction( &DefaultExtrudePlugin::PeriodicCosinusCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, curveScale, bevelHeight );
        break;
    case ExtrudeCurveType::Gauss:
        ApplyFunction( &DefaultExtrudePlugin::GaussCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, curveScale, bevelHeight );
        break;
    case ExtrudeCurveType::Circle:
        ApplyFunction( &DefaultExtrudePlugin::CircleCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, curveScale, bevelHeight );
        break;
    case ExtrudeCurveType::None:
        ApplyFunction( &DefaultExtrudePlugin::ZeroLineCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, curveScale, bevelHeight );
        break;
    default:
        assert( !"Shouldn't be here" );
        ApplyFunction( &DefaultExtrudePlugin::ZeroLineCurve, mesh, normals, edges, corners, beginContourOffset, endContourOffset, tesselation, curveScale, bevelHeight );
    }
}

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
float       DefaultExtrudePlugin::ZeroLineCurve         ( float /*param*/ )
{
    return 0.0f;
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
// Bevel extrude curve functions
// ========================================================================= //

// ***********************
//
float       DefaultExtrudePlugin::LineCurve                 ( float param )
{
    return param;
}

// ***********************
//
float       DefaultExtrudePlugin::HalfSinusCurve            ( float param )
{
    float arg = 0.5f * glm::pi< float >() * param;
    return sin( arg );
}

// ***********************
//
float       DefaultExtrudePlugin::SinusCurve                ( float param )
{
    float scaledParam = ( 2 * param ) - 1.0f;
    float arg = 0.5f * glm::pi< float >() * scaledParam;

    return ( sin( arg ) / 2.0f ) + 0.5f;
}

// ***********************
//
float       DefaultExtrudePlugin::InverseHalfSinusCurve     ( float param )
{
    float arg = 0.5f * glm::pi< float >() * ( param - 1.0f );
    return sin( arg ) + 1.0f;
}

// ***********************
//
float       DefaultExtrudePlugin::CircleBevelCurve          ( float param )
{
    float paramPow2 = ( param - 1.0f ) *  ( param - 1.0f );
    return sqrt( 1 - paramPow2 );
}

// ***********************
//
float       DefaultExtrudePlugin::InverseCircleBevelCurve   ( float param )
{
    return 1.0f - CircleBevelCurve( 1.0f - param );
}

// ***********************
//
float       DefaultExtrudePlugin::HarmonicCurve             ( float param )
{
    // Note: reciprocal never reaches 0.0 and 1.0. But we must avoid holes.
    if( param == 0.0f )
        return 0.0f;

    if( param == 1.0f )
        return 1.0f;

    float reversedParam = 1.0f - param;

    float scaledParam = reversedParam * 10.f;
    float result = 1 / scaledParam;
    result = result / 10.0f;

    // Make this function more convergent to 0.0.
    if( param <= 0.5 )
    {
        result -= 2.0f * ( 0.5f - param ) / 100.0f;        // param * ( 1 / 100 )
    }


    // Note: recoprocal never reaches 0.0 and 1.0. But we must avoid holes.
    if( result > 1.0f )
        result = 1.0f;

    return result;
}

// ***********************
//
float       DefaultExtrudePlugin::InverseHarmonicCurve      ( float param )
{
    return 1.0f - HarmonicCurve( 1.0f - param );
}


} // model
} // bv

