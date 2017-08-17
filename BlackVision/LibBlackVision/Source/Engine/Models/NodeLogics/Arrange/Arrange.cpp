#include "stdafx.h"

#include "Arrange.h"

#include "Mathematics/glm_inc.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/NodeLogics/NodeLogicHelper.h"


namespace bv {

namespace SerializationHelper {

std::pair< nodelogic::Arrange::ArrangmentType, const char* > ArrangeTypeMapping[] = 
{   std::make_pair( nodelogic::Arrange::ArrangmentType::Circle, "Circle" )
    , std::make_pair( nodelogic::Arrange::ArrangmentType::Grid2D, "Grid2D" )
    , std::make_pair( nodelogic::Arrange::ArrangmentType::Grid3D, "Grid3D" )
    , std::make_pair( nodelogic::Arrange::ArrangmentType::Line, "Line" )
    , std::make_pair( nodelogic::Arrange::ArrangmentType::Sphere, "Sphere" )
    , std::make_pair( nodelogic::Arrange::ArrangmentType::Total, "" )      // default
};

template<> nodelogic::Arrange::ArrangmentType   String2T        ( const std::string & s, const nodelogic::Arrange::ArrangmentType & defaultVal )    { return String2Enum( ArrangeTypeMapping, s, defaultVal ); }
template<> std::string                          T2String        ( const nodelogic::Arrange::ArrangmentType & t )                                    { return Enum2String( ArrangeTypeMapping, t ); }

}   // SerializationHelper

namespace nodelogic
{

const std::string       Arrange::m_type = "Arrange";

const std::string       Arrange::ACTION::LINE_ARRANGE       = "LineArrange";
const std::string       Arrange::ACTION::CIRCLE_ARRANGE     = "CircleArrange";
const std::string       Arrange::ACTION::SPHERE_ARRANGE     = "SphereArrange";
const std::string       Arrange::ACTION::GRID2D_ARRANGE     = "Grid2DArrange";
const std::string       Arrange::ACTION::GRID3D_ARRANGE     = "Grid3DArrange";
const std::string       Arrange::ACTION::GET_PARAMETERS     = "GetParameters";
const std::string       Arrange::ACTION::ARRANGE_AFTER_LOAD = "ArrangeAfterLoad";

const std::string       Arrange::PARAMETERS::LINE_START_POINT       = "Line_StartPoint";
const std::string       Arrange::PARAMETERS::LINE_END_POINT         = "Line_EndPoint";

const std::string       Arrange::PARAMETERS::CIRCLE_RADIUS          = "Circle_Radius";
const std::string       Arrange::PARAMETERS::CIRCLE_ROTATION        = "Circle_Rotation";
const std::string       Arrange::PARAMETERS::CIRCLE_CENTER          = "Circle_Center";

const std::string       Arrange::PARAMETERS::SPHERE_RADIUS          = "Sphere_Radius";
const std::string       Arrange::PARAMETERS::SPHERE_ROTATION        = "Sphere_Rotation";
const std::string       Arrange::PARAMETERS::SPHERE_CENTER          = "Sphere_Center";
const std::string       Arrange::PARAMETERS::SPHERE_ROWS            = "Sphere_Rows";
const std::string       Arrange::PARAMETERS::SPHERE_COLUMNS         = "Sphere_Columns";

const std::string       Arrange::PARAMETERS::GRID2D_ROTATION        = "Grid2D_Rotation";
const std::string       Arrange::PARAMETERS::GRID2D_CENTER          = "Grid2D_Center";
const std::string       Arrange::PARAMETERS::GRID2D_ROWS            = "Grid2D_Rows";
const std::string       Arrange::PARAMETERS::GRID2D_COLUMNS         = "Grid2D_Columns";
const std::string       Arrange::PARAMETERS::GRID2D_INTERSPACES     = "Grid2D_Interspaces";

const std::string       Arrange::PARAMETERS::GRID3D_ROTATION        = "Grid3D_Rotation";
const std::string       Arrange::PARAMETERS::GRID3D_CENTER          = "Grid3D_Center";
const std::string       Arrange::PARAMETERS::GRID3D_ROWS            = "Grid3D_Rows";
const std::string       Arrange::PARAMETERS::GRID3D_COLUMNS         = "Grid3D_Columns";
const std::string       Arrange::PARAMETERS::GRID3D_LAYERS          = "Grid3D_Layers";
const std::string       Arrange::PARAMETERS::GRID3D_INTERSPACES     = "Grid3D_Interspaces";

const std::string       Arrange::PARAMETERS::ARRANGE_AFTER_LOAD     = "ArrangeAfterLoad";


// ***********************
//
const std::string &     Arrange::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     Arrange::GetType             () const
{
    return Type();
}

// ***********************
//
Arrange::Arrange             ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
{
    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::LINE_START_POINT, glm::vec3( -1.0, 0.0, 0.0 ) );
    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::LINE_END_POINT, glm::vec3( 1.0, 0.0, 0.0 ) );

    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::CIRCLE_CENTER, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::CIRCLE_ROTATION, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddFloatParam( m_paramValModel, timeEvaluator, PARAMETERS::CIRCLE_RADIUS, 1.0f );

    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::SPHERE_CENTER, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::SPHERE_ROTATION, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddFloatParam( m_paramValModel, timeEvaluator, PARAMETERS::SPHERE_RADIUS, 1.0f );
    model::AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::SPHERE_COLUMNS, 3 );
    model::AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::SPHERE_ROWS, 3 );

    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::GRID2D_CENTER, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::GRID2D_ROTATION, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::GRID2D_ROWS, 3 );
    model::AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::GRID2D_COLUMNS, 3 );
    model::AddVec2Param( m_paramValModel, timeEvaluator, PARAMETERS::GRID2D_INTERSPACES, glm::vec2( 0.2, 0.2 ) );

    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::GRID3D_CENTER, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::GRID3D_ROTATION, glm::vec3( 0.0, 0.0, 0.0 ) );
    model::AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::GRID3D_ROWS, 3 );
    model::AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::GRID3D_COLUMNS, 3 );
    model::AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::GRID3D_LAYERS, 3 );
    model::AddVec3Param( m_paramValModel, timeEvaluator, PARAMETERS::GRID3D_INTERSPACES, glm::vec3( 0.2, 0.2, 0.2 ) );

    model::AddBoolParam( m_paramValModel, timeEvaluator, PARAMETERS::ARRANGE_AFTER_LOAD, true );
}

// ***********************
//
Arrange::~Arrange()
{}

// ***********************
// Empty for purpose. Base class updates model here, but Arrange logic doesn't need this.
void	                    Arrange::Update			( TimeType )
{}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        Arrange::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
    ser.SetAttribute( "type", m_type );

    if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
    {
        NodeLogicBase::Serialize( ser );
        ser.SetAttribute( "ArrangementType", SerializationHelper::T2String( m_lastArrangementType ) );
    }

    ser.ExitChild();    // logic
}

// ***********************
//
ArrangePtr              Arrange::Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Arrange >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ***********************
//
void                    Arrange::Deserialize     ( const IDeserializer & ser )
{
    NodeLogicBase::Deserialize( ser );
    m_paramValModel->Update();

    bool arrangeAfterLoad = QueryTypedValue< ValueBoolPtr >( m_paramValModel->GetValue( PARAMETERS::ARRANGE_AFTER_LOAD ) )->GetValue();
    ArrangmentType type = SerializationHelper::String2T( ser.GetAttribute( "ArrangementType" ), ArrangmentType::Grid2D );

    if( arrangeAfterLoad )
    {
        if( type == ArrangmentType::Circle )
        {
            m_lastArrangement = CircleArrangeParams::Create( m_paramValModel );
            ArrangeChildren( ArrangmentType::Circle, m_lastArrangement );
        }
        else if( type == ArrangmentType::Line )
        {
            m_lastArrangement = LineArrangeParams::Create( m_paramValModel );
            ArrangeChildren( ArrangmentType::Line, m_lastArrangement );
        }
        else if( type == ArrangmentType::Grid2D )
        {
            m_lastArrangement = Grid2DArrangeParams::Create( m_paramValModel );
            ArrangeChildren( ArrangmentType::Grid2D, m_lastArrangement );
        }
        else if( type == ArrangmentType::Grid3D )
        {
            m_lastArrangement = Grid3DArrangeParams::Create( m_paramValModel );
            ArrangeChildren( ArrangmentType::Grid3D, m_lastArrangement );
        }
        else if( type == ArrangmentType::Sphere )
        {
            m_lastArrangement = SphereArrangeParams::Create( m_paramValModel );
            ArrangeChildren( ArrangmentType::Sphere, m_lastArrangement );
        }
    }
    else
    {
        m_lastArrangement = nullptr;
        return;
    }
}


// ***********************
//
void                            Arrange::CircleArrangeParams::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "CircleArrangeParams" );
    
    ser.SetAttribute( "Center", SerializationHelper::T2String( Center ) );
    ser.SetAttribute( "Radius", SerializationHelper::T2String( Radius ) );
    ser.SetAttribute( "Rotation", SerializationHelper::T2String( Rotation ) );

    ser.ExitChild();    // CircleArrangeParams
}

// ***********************
//
std::unique_ptr< Arrange::CircleArrangeParams >   Arrange::CircleArrangeParams::Create          ( const IDeserializer & deser )
{
    std::unique_ptr< CircleArrangeParams > params = std::unique_ptr< CircleArrangeParams >( new CircleArrangeParams );

    params->Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params->Radius = SerializationHelper::String2T( deser.GetAttribute( "Radius" ), 2.0f );
    params->Rotation = SerializationHelper::String2T( deser.GetAttribute( "Rotation" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    
    return params;
}

// ***********************
//
std::unique_ptr< Arrange::CircleArrangeParams >     Arrange::CircleArrangeParams::Create          ( model::DefaultParamValModelPtr & model )
{
    std::unique_ptr< CircleArrangeParams > params = std::unique_ptr< CircleArrangeParams >( new CircleArrangeParams );

    params->Center = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::CIRCLE_CENTER ) )->GetValue();
    params->Radius = QueryTypedValue< ValueFloatPtr >( model->GetValue( PARAMETERS::CIRCLE_RADIUS ) )->GetValue();
    params->Rotation = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::CIRCLE_ROTATION ) )->GetValue();
    
    return params;
}

// ***********************
//
void                            Arrange::LineArrangeParams::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "LineArrangeParams" );

    ser.SetAttribute( "StartPoint", SerializationHelper::T2String( StartPoint ) );
    ser.SetAttribute( "EndPoint", SerializationHelper::T2String( EndPoint ) );

    ser.ExitChild();    // LineArrangeParams
}

// ***********************
//
std::unique_ptr< Arrange::LineArrangeParams >     Arrange::LineArrangeParams::Create          ( const IDeserializer & deser )
{
    std::unique_ptr< LineArrangeParams > params = std::unique_ptr< LineArrangeParams >( new LineArrangeParams );

    params->StartPoint = SerializationHelper::String2T( deser.GetAttribute( "StartPoint" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params->EndPoint = SerializationHelper::String2T( deser.GetAttribute( "EndPoint" ), glm::vec3( 0.0, 0.0, 0.0 ) );

    return params;
}

// ***********************
//
std::unique_ptr< Arrange::LineArrangeParams >     Arrange::LineArrangeParams::Create          ( model::DefaultParamValModelPtr & model )
{
    std::unique_ptr< LineArrangeParams > params = std::unique_ptr< LineArrangeParams >( new LineArrangeParams );

    params->StartPoint = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::LINE_START_POINT ) )->GetValue();
    params->EndPoint = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::LINE_END_POINT ) )->GetValue();
    
    return params;
}

// ***********************
//
void                            Arrange::Grid2DArrangeParams::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "Grid2DArrangeParams" );

    ser.SetAttribute( "Rows", SerializationHelper::T2String( Rows ) );
    ser.SetAttribute( "Columns", SerializationHelper::T2String( Columns ) );

    ser.SetAttribute( "Center", SerializationHelper::T2String( Center ) );
    ser.SetAttribute( "Rotation", SerializationHelper::T2String( Rotation ) );
    ser.SetAttribute( "Interspaces", SerializationHelper::T2String( Interspaces ) );

    //ser.SetAttribute( "DistributeUniform", SerializationHelper::T2String( Uniform ) );

    ser.ExitChild();    // Grid2DArrangeParams
}

// ***********************
//
std::unique_ptr< Arrange::Grid2DArrangeParams >   Arrange::Grid2DArrangeParams::Create          ( const IDeserializer & deser )
{
    std::unique_ptr< Grid2DArrangeParams > params = std::unique_ptr< Grid2DArrangeParams >( new Grid2DArrangeParams );

    params->Rows = SerializationHelper::String2T( deser.GetAttribute( "Rows" ), 3 );
    params->Columns = SerializationHelper::String2T( deser.GetAttribute( "Columns" ), 3 );

    params->Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params->Rotation = SerializationHelper::String2T( deser.GetAttribute( "Rotation" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params->Interspaces = SerializationHelper::String2T( deser.GetAttribute( "Interspaces" ), glm::vec2( 1.0, 1.0 ) );

    //params->Uniform = SerializationHelper::String2T( deser.GetAttribute( "DistributeUniform" ), false );
    
    return params;
}

// ***********************
//
std::unique_ptr< Arrange::Grid2DArrangeParams >     Arrange::Grid2DArrangeParams::Create          ( model::DefaultParamValModelPtr & model )
{
    std::unique_ptr< Grid2DArrangeParams > params = std::unique_ptr< Grid2DArrangeParams >( new Grid2DArrangeParams );

    params->Center = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::GRID2D_CENTER ) )->GetValue();
    params->Rotation = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::GRID2D_ROTATION ) )->GetValue();
    params->Interspaces = QueryTypedValue< ValueVec2Ptr >( model->GetValue( PARAMETERS::GRID2D_INTERSPACES ) )->GetValue();

    params->Rows = QueryTypedValue< ValueIntPtr >( model->GetValue( PARAMETERS::GRID2D_ROWS ) )->GetValue();
    params->Columns = QueryTypedValue< ValueIntPtr >( model->GetValue( PARAMETERS::GRID2D_COLUMNS ) )->GetValue();
    
    return params;
}

// ***********************
//
void                            Arrange::Grid3DArrangeParams::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "Grid3DArrangeParams" );

    ser.SetAttribute( "Rows", SerializationHelper::T2String( Rows ) );
    ser.SetAttribute( "Columns", SerializationHelper::T2String( Columns ) );
    ser.SetAttribute( "Layets", SerializationHelper::T2String( Layers ) );

    ser.SetAttribute( "Center", SerializationHelper::T2String( Center ) );
    ser.SetAttribute( "Rotation", SerializationHelper::T2String( Rotation ) );
    ser.SetAttribute( "Interspaces", SerializationHelper::T2String( Interspaces ) );

    //ser.SetAttribute( "DistributeUniform", SerializationHelper::T2String( Uniform ) );

    ser.ExitChild();    // Grid3DArrangeParams
}

// ***********************
//
std::unique_ptr< Arrange::Grid3DArrangeParams >   Arrange::Grid3DArrangeParams::Create          ( const IDeserializer & deser )
{
    std::unique_ptr< Grid3DArrangeParams > params = std::unique_ptr< Grid3DArrangeParams >( new Grid3DArrangeParams );

    params->Rows = SerializationHelper::String2T( deser.GetAttribute( "Rows" ), 3 );
    params->Columns = SerializationHelper::String2T( deser.GetAttribute( "Columns" ), 3 );
    params->Layers = SerializationHelper::String2T( deser.GetAttribute( "Layers" ), 3 );

    params->Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params->Rotation = SerializationHelper::String2T( deser.GetAttribute( "Rotation" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params->Interspaces = SerializationHelper::String2T( deser.GetAttribute( "Interspaces" ), glm::vec3( 1.0, 1.0, 1.0 ) );

    //params->Uniform = SerializationHelper::String2T( deser.GetAttribute( "DistributeUniform" ), false );

    return params;
}

// ***********************
//
std::unique_ptr< Arrange::Grid3DArrangeParams >     Arrange::Grid3DArrangeParams::Create          ( model::DefaultParamValModelPtr & model )
{
    std::unique_ptr< Grid3DArrangeParams > params = std::unique_ptr< Grid3DArrangeParams >( new Grid3DArrangeParams );

    params->Center = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::GRID3D_CENTER ) )->GetValue();
    params->Rotation = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::GRID3D_ROTATION ) )->GetValue();
    params->Interspaces = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::GRID3D_INTERSPACES ) )->GetValue();

    params->Rows = QueryTypedValue< ValueIntPtr >( model->GetValue( PARAMETERS::GRID3D_ROWS ) )->GetValue();
    params->Columns = QueryTypedValue< ValueIntPtr >( model->GetValue( PARAMETERS::GRID3D_COLUMNS ) )->GetValue();
    params->Layers = QueryTypedValue< ValueIntPtr >( model->GetValue( PARAMETERS::GRID3D_LAYERS ) )->GetValue();
    
    return params;
}

// ***********************
//
void                            Arrange::SphereArrangeParams::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "SphereArrangeParams" );

    ser.SetAttribute( "Rows", SerializationHelper::T2String( Rows ) );
    ser.SetAttribute( "Columns", SerializationHelper::T2String( Columns ) );

    ser.SetAttribute( "Center", SerializationHelper::T2String( Center ) );
    ser.SetAttribute( "Radius", SerializationHelper::T2String( Radius ) );
    ser.SetAttribute( "Rotation", SerializationHelper::T2String( Rotation ) );

    //ser.SetAttribute( "DistributeUniform", SerializationHelper::T2String( Uniform ) );

    ser.ExitChild();
}

// ***********************
//
std::unique_ptr< Arrange::SphereArrangeParams >   Arrange::SphereArrangeParams::Create          ( const IDeserializer & deser )
{
    std::unique_ptr< SphereArrangeParams > params = std::unique_ptr< SphereArrangeParams >( new SphereArrangeParams);

    params->Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params->Radius = SerializationHelper::String2T( deser.GetAttribute( "Radius" ), 2.0f );
    params->Rotation = SerializationHelper::String2T( deser.GetAttribute( "Rotation" ), glm::vec3( 0.0, 0.0, 0.0 ) );

    params->Rows = SerializationHelper::String2T( deser.GetAttribute( "Rows" ), 4 );
    params->Columns = SerializationHelper::String2T( deser.GetAttribute( "Columns" ), 4 );

    //params->Uniform = SerializationHelper::String2T( deser.GetAttribute( "DistributeUniform" ), false );

    return params;
}

// ***********************
//
std::unique_ptr< Arrange::SphereArrangeParams >     Arrange::SphereArrangeParams::Create          ( model::DefaultParamValModelPtr & model )
{
    std::unique_ptr< SphereArrangeParams > params = std::unique_ptr< SphereArrangeParams >( new SphereArrangeParams );

    params->Center = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::SPHERE_CENTER ) )->GetValue();
    params->Rotation = QueryTypedValue< ValueVec3Ptr >( model->GetValue( PARAMETERS::SPHERE_ROTATION ) )->GetValue();

    params->Rows = QueryTypedValue< ValueIntPtr >( model->GetValue( PARAMETERS::SPHERE_ROWS ) )->GetValue();
    params->Columns = QueryTypedValue< ValueIntPtr >( model->GetValue( PARAMETERS::SPHERE_COLUMNS ) )->GetValue();

    params->Radius = QueryTypedValue< ValueFloatPtr >( model->GetValue( PARAMETERS::SPHERE_RADIUS ) )->GetValue();
    
    return params;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Arrange::HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * /*editor*/ )
{
    m_paramValModel->Update();      // Update model before arranging, bacause we don't do it in Update function.
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == Arrange::ACTION::CIRCLE_ARRANGE )
    {
        m_lastArrangement = CircleArrangeParams::Create( m_paramValModel );
        ArrangeChildren( ArrangmentType::Circle, m_lastArrangement );

        return true;
    }
    else if( action == Arrange::ACTION::LINE_ARRANGE )
    {
        m_lastArrangement = LineArrangeParams::Create( m_paramValModel );
        ArrangeChildren( ArrangmentType::Line, m_lastArrangement );

        return true;
    }
    else if( action == Arrange::ACTION::SPHERE_ARRANGE )
    {
        m_lastArrangement = SphereArrangeParams::Create( m_paramValModel );
        ArrangeChildren( ArrangmentType::Sphere, m_lastArrangement );

        return true;
    }
    else if( action == Arrange::ACTION::GRID2D_ARRANGE )
    {
        m_lastArrangement = Grid2DArrangeParams::Create( m_paramValModel );
        ArrangeChildren( ArrangmentType::Grid2D, m_lastArrangement );

        return true;
    }
    else if( action == Arrange::ACTION::GRID3D_ARRANGE )
    {
        m_lastArrangement = Grid3DArrangeParams::Create( m_paramValModel );
        ArrangeChildren( ArrangmentType::Grid3D, m_lastArrangement );

        return true;
    }
    else if( action == Arrange::ACTION::GET_PARAMETERS )
    {
        if( m_lastArrangement )
        {
            m_lastArrangement->Serialize( response );
            return true;
        }
    }

    return false;
}


// ========================================================================= //
// Arrangment
// ========================================================================= //

// ***********************
//
void            Arrange::ArrangeChildren     ( ArrangmentType type, std::unique_ptr< ArrangeParamsBase > & params )
{
    auto nodes = GetNodesToArrange();
    m_lastArrangementType = type;

    if( type == ArrangmentType::Circle )
    {
        CircleArrange( nodes, *static_cast< CircleArrangeParams* >( params.get() ) );
    }
    else if( type == ArrangmentType::Sphere )
    {
        SphereArrange( nodes, *static_cast< SphereArrangeParams* >( params.get() ) );
    }
    else if( type == ArrangmentType::Grid2D )
    {
        Grid2DArrange( nodes, *static_cast< Grid2DArrangeParams* >( params.get() ) );
    }
    else if( type == ArrangmentType::Grid3D )
    {
        Grid3DArrange( nodes, *static_cast< Grid3DArrangeParams* >( params.get() ) );
    }
    else if( type == ArrangmentType::Line )
    {
        LineArrange( nodes, *static_cast< LineArrangeParams* >( params.get() ) );
    }
}

// ***********************
//
void            Arrange::CircleArrange       ( std::vector< model::BasicNodePtr > & nodes, const CircleArrangeParams & params )
{
    auto numElements = nodes.size();

    if( numElements > 0 )
    {
        glm::mat3 rotate = glm::mat3( glm::eulerAngleYXZ( glm::radians( params.Rotation.y ), glm::radians( params.Rotation.x ), glm::radians( params.Rotation.z ) ) );

        float deltaAngle = 2.0f * glm::pi< float >() / numElements;
        for( int i = 0; i < numElements; i++ )
        {
            glm::vec3 position( 0 );
            position.x = params.Radius * cos( i * deltaAngle );
            position.y = params.Radius * sin( i * deltaAngle );

            // Transform position
            position = rotate * position;
            position += params.Center;

            SetNodePosition( nodes[ i ], position, TimeType( 0.0 ) );
        }
    }
}

// ***********************
//
void            Arrange::LineArrange         ( std::vector< model::BasicNodePtr > & nodes, const LineArrangeParams & params )
{
    auto numElements = nodes.size();

    if( numElements > 0 )
    {
        // Delta is scaled to [0.0, 1.0].This way it can be used in mix function.
        // First and last element should be at start and end point.
        float delta = 1.0f / ( numElements - 1 );
        for( int i = 0; i < numElements; i++ )
        {
            glm::vec3 position = glm::mix( params.StartPoint, params.EndPoint, delta * i );
            SetNodePosition( nodes[ i ], position, TimeType( 0.0 ) );
        }
    }
}

// ***********************
//
void            Arrange::Grid2DArrange       ( std::vector< model::BasicNodePtr > & nodes, const Grid2DArrangeParams & params )
{
    auto numElements = nodes.size();

    if( numElements > 0 )
    {
        glm::mat3 rotate = glm::mat3( glm::eulerAngleYXZ( glm::radians( params.Rotation.y ), glm::radians( params.Rotation.x ), glm::radians( params.Rotation.z ) ) );

        // @todo Zrobiæ równomierne rozmieszczanie elementów.

        int nodesCounter = 0;
        for( int row = 0; row < params.Rows && nodesCounter < numElements; ++row )
        {
            for( int col = 0; col < params.Columns && nodesCounter < numElements; col++ )
            {
                float xMult = (float)col - params.Columns / 2.0f + 0.5f;
                float yMult = (float)row - params.Rows / 2.0f + 0.5f;

                glm::vec3 position = glm::vec3( params.Interspaces * glm::vec2( xMult, -yMult ), 0.0 );
                
                position = rotate * position;
                position += params.Center;

                SetNodePosition( nodes[ nodesCounter ], position, TimeType( 0.0 ) );

                nodesCounter++;
            }
        }
    }
}

// ***********************
//
void            Arrange::Grid3DArrange       ( std::vector< model::BasicNodePtr > & nodes, const Grid3DArrangeParams & params )
{
    auto numElements = nodes.size();

    if( numElements > 0 )
    {
        glm::mat3 rotate = glm::mat3( glm::eulerAngleYXZ( glm::radians( params.Rotation.y ), glm::radians( params.Rotation.x ), glm::radians( params.Rotation.z ) ) );

        // @todo Zrobiæ równomierne rozmieszczanie elementów.

        int nodesCounter = 0;
        for( int layer = 0; layer < params.Layers && nodesCounter < numElements; ++layer )
        {
            for( int row = 0; row < params.Rows && nodesCounter < numElements; ++row )
            {
                for( int col = 0; col < params.Columns && nodesCounter < numElements; col++ )
                {
                    float xMult = (float)col - params.Columns / 2.0f + 0.5f;
                    float yMult = (float)row - params.Rows / 2.0f + 0.5f;
                    float zMult = (float)layer - params.Layers / 2.0f + 0.5f;

                    glm::vec3 position = params.Interspaces * glm::vec3( xMult, -yMult, -zMult );
                
                    position = rotate * position;
                    position += params.Center;

                    SetNodePosition( nodes[ nodesCounter ], position, TimeType( 0.0 ) );

                    nodesCounter++;
                }
            }
        }
    }
}

// ***********************
//
void            Arrange::SphereArrange       ( std::vector< model::BasicNodePtr > & nodes, const SphereArrangeParams & params )
{
    auto numElements = nodes.size();

    if( numElements > 0 )
    {
        glm::mat3 rotate = glm::mat3( glm::eulerAngleYXZ( glm::radians( params.Rotation.y ), glm::radians( params.Rotation.x ), glm::radians( params.Rotation.z ) ) );

        int nodesCounter = 0;
        float colDelta = 2.0f * glm::pi< float >() / params.Columns;
        float rowDelta = glm::pi< float >() / ( params.Rows + 1 );

        for( int row = 0; row < params.Rows && nodesCounter < numElements; ++row )
        {
            float rowAngle = glm::half_pi< float >() - rowDelta * ( row + 1 );
            float posY = sin( rowAngle ) * params.Radius;
            float RcastXZ = cos( rowAngle ) * params.Radius;

            for( int col = 0; col < params.Columns && nodesCounter < numElements; col++ )
            {
                float colAngle = colDelta * col;

                glm::vec3 position = glm::vec3( RcastXZ * cos( colAngle ), posY, RcastXZ * sin( colAngle ) );
                
                position = rotate * position;
                position += params.Center;

                SetNodePosition( nodes[ nodesCounter ], position, TimeType( 0.0 ) );

                nodesCounter++;
            }
        }
    }
}

// ***********************
//
std::vector< model::BasicNodePtr >  Arrange::GetNodesToArrange    ()
{
    std::vector< model::BasicNodePtr > children;

    if( auto parentNode = m_parentNode.lock() )
    {
        auto numChildren = parentNode->GetNumChildren();
        children.reserve( numChildren );

        for( unsigned int i = 0; i < numChildren; ++i )
        {
            children.push_back( parentNode->GetChild( i ) );
        }
    }

    return children;
}

// ***********************
//
void                                    Arrange::SetNodePosition     ( model::BasicNodePtr node, glm::vec3 position, TimeType keyTime )
{
    if( node )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();

        auto center = transformParam->GetTransform().GetCenter( keyTime );
        transformParam->SetTranslation( position - center, keyTime );
    }
}

}   // nodelogic
}	// bv
