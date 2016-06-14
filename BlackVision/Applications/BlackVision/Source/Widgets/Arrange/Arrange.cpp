#include "Arrange.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Widgets/NodeLogicHelper.h"

#include <glm/gtx/euler_angles.hpp>



namespace bv { namespace nodelogic
{

const std::string       Arrange::m_type = "Arrange";

const std::string       Arrange::ACTION::LINE_ARRANGE       = "LineArrange";
const std::string       Arrange::ACTION::CIRCLE_ARRANGE     = "CircleArrange";
const std::string       Arrange::ACTION::SPHERE_ARRANGE     = "SphereArrange";
const std::string       Arrange::ACTION::GRID2D_ARRANGE     = "Grid2DArrange";
const std::string       Arrange::ACTION::GRID3D_ARRANGE     = "Grid3DArrange";

//const std::string       Arrange::PARAMETERS::PARAMETER_NAME = "ParamName";


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
Arrange::Arrange             ( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    :   m_parentNode( parent )
{}

// ***********************
//
Arrange::~Arrange()
{}

// ***********************
//
void                        Arrange::Update			( TimeType /*t*/ )
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
    }

    ser.ExitChild();    // logic
}

// ***********************
//
ArrangePtr              Arrange::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Arrange >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
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
Arrange::CircleArrangeParams    Arrange::CircleArrangeParams::Create          ( const IDeserializer & deser )
{
    CircleArrangeParams params;

    params.Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params.Radius = SerializationHelper::String2T( deser.GetAttribute( "Radius" ), 2.0f );
    params.Rotation = SerializationHelper::String2T( deser.GetAttribute( "Rotation" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    
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
Arrange::LineArrangeParams      Arrange::LineArrangeParams::Create          ( const IDeserializer & deser )
{
    LineArrangeParams params;

    params.StartPoint = SerializationHelper::String2T( deser.GetAttribute( "StartPoint" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params.EndPoint = SerializationHelper::String2T( deser.GetAttribute( "EndPoint" ), glm::vec3( 0.0, 0.0, 0.0 ) );

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

    ser.SetAttribute( "DistributeUniform", SerializationHelper::T2String( Uniform ) );

    ser.ExitChild();    // Grid2DArrangeParams
}

// ***********************
//
Arrange::Grid2DArrangeParams    Arrange::Grid2DArrangeParams::Create          ( const IDeserializer & deser )
{
    Grid2DArrangeParams params;

    params.Rows = SerializationHelper::String2T( deser.GetAttribute( "Rows" ), 3 );
    params.Columns = SerializationHelper::String2T( deser.GetAttribute( "Columns" ), 3 );

    params.Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params.Rotation = SerializationHelper::String2T( deser.GetAttribute( "Rotation" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params.Interspaces = SerializationHelper::String2T( deser.GetAttribute( "Interspaces" ), glm::vec2( 1.0, 1.0 ) );

    params.Uniform = SerializationHelper::String2T( deser.GetAttribute( "DistributeUniform" ), false );
    
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

    ser.SetAttribute( "DistributeUniform", SerializationHelper::T2String( Uniform ) );

    ser.ExitChild();    // Grid3DArrangeParams
}

// ***********************
//
Arrange::Grid3DArrangeParams    Arrange::Grid3DArrangeParams::Create          ( const IDeserializer & deser )
{
    Grid3DArrangeParams params;

    params.Rows = SerializationHelper::String2T( deser.GetAttribute( "Rows" ), 3 );
    params.Columns = SerializationHelper::String2T( deser.GetAttribute( "Columns" ), 3 );
    params.Layers = SerializationHelper::String2T( deser.GetAttribute( "Layers" ), 3 );

    params.Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params.Rotation = SerializationHelper::String2T( deser.GetAttribute( "Rotation" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params.Interspaces = SerializationHelper::String2T( deser.GetAttribute( "Interspaces" ), glm::vec3( 1.0, 1.0, 1.0 ) );

    params.Uniform = SerializationHelper::String2T( deser.GetAttribute( "DistributeUniform" ), false );

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

    ser.SetAttribute( "DistributeUniform", SerializationHelper::T2String( Uniform ) );

    ser.ExitChild();
}

// ***********************
//
Arrange::SphereArrangeParams    Arrange::SphereArrangeParams::Create          ( const IDeserializer & deser )
{
    SphereArrangeParams params;

    params.Center = SerializationHelper::String2T( deser.GetAttribute( "Center" ), glm::vec3( 0.0, 0.0, 0.0 ) );
    params.Radius = SerializationHelper::String2T( deser.GetAttribute( "Radius" ), 2.0f );

    params.Rows = SerializationHelper::String2T( deser.GetAttribute( "Rows" ), 4 );
    params.Columns = SerializationHelper::String2T( deser.GetAttribute( "Columns" ), 4 );

    params.Uniform = SerializationHelper::String2T( deser.GetAttribute( "DistributeUniform" ), false );

    return params;
}


// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Arrange::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == Arrange::ACTION::CIRCLE_ARRANGE )
    {
        auto nodes = GetNodesToArrange();
        auto params = CircleArrangeParams::Create( eventDeser );
        CircleArrange( nodes, params );

        return true;
    }
    else if( action == Arrange::ACTION::LINE_ARRANGE )
    {
        auto nodes = GetNodesToArrange();
        auto params = LineArrangeParams::Create( eventDeser );
        LineArrange( nodes, params );

        return true;
    }
    else if( action == Arrange::ACTION::SPHERE_ARRANGE )
    {
        auto nodes = GetNodesToArrange();
        auto params = SphereArrangeParams::Create( eventDeser );
        SphereArrange( nodes, params );

        return true;
    }
    else if( action == Arrange::ACTION::GRID2D_ARRANGE )
    {
        auto nodes = GetNodesToArrange();
        auto params = Grid2DArrangeParams::Create( eventDeser );
        Grid2DArrange( nodes, params );

        return true;
    }
    else if( action == Arrange::ACTION::GRID3D_ARRANGE )
    {
        auto nodes = GetNodesToArrange();
        auto params = Grid3DArrangeParams::Create( eventDeser );
        Grid3DArrange( nodes, params );

        return true;
    }

    return false;
}


// ========================================================================= //
// Arrangment
// ========================================================================= //

// ***********************
//
void            Arrange::CircleArrange       ( std::vector< bv::model::BasicNodePtr > & nodes, const CircleArrangeParams & params )
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
void            Arrange::LineArrange         ( std::vector< bv::model::BasicNodePtr > & nodes, const LineArrangeParams & params )
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
void            Arrange::Grid2DArrange       ( std::vector< bv::model::BasicNodePtr > & nodes, const Grid2DArrangeParams & params )
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
void            Arrange::Grid3DArrange       ( std::vector< bv::model::BasicNodePtr > & nodes, const Grid3DArrangeParams & params )
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
void            Arrange::SphereArrange       ( std::vector< bv::model::BasicNodePtr > & /*nodes*/, const SphereArrangeParams & /*params*/ )
{

}

// ***********************
//
std::vector< bv::model::BasicNodePtr >  Arrange::GetNodesToArrange    ()
{
    auto numChildren = m_parentNode->GetNumChildren();
    std::vector< bv::model::BasicNodePtr > children;
    children.reserve( numChildren );

    for( unsigned int i = 0; i < numChildren; ++i )
    {
        children.push_back( m_parentNode->GetChild( i ) );
    }

    return children;
}

// ***********************
//
void                                    Arrange::SetNodePosition     ( bv::model::BasicNodePtr node, glm::vec3 position, TimeType keyTime )
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
