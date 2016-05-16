#include "PieChart.h"

#include <random>
#include <glm/gtx/rotate_vector.hpp>

#include "Engine/Events/EventHandlerHelpers.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCylinderPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultMaterialPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"

#include "Engine/Models/BVProjectEditor.h"


namespace bv {

namespace SerializationHelper {

std::pair< nodelogic::PieChart::PieChartType, const char * > pct2s[] = 
{
    std::make_pair( nodelogic::PieChart::PieChartType::PST_COLOR, "color" ),
    std::make_pair( nodelogic::PieChart::PieChartType::PST_MATERIAL, "material" ),
    std::make_pair( nodelogic::PieChart::PieChartType::PST_TOTAL, "" )
};


template<> 
std::string                         T2String    ( const nodelogic::PieChart::PieChartType & pct )
{ 
    return Enum2String( pct2s, pct ); 
}

// *************************************
//
template<> 
nodelogic::PieChart::PieChartType              String2T    ( const std::string & s, const nodelogic::PieChart::PieChartType & defaultVal )
{
    auto type = String2Enum( pct2s, s );
    if( type == nodelogic::PieChart::PieChartType::PST_TOTAL )
    {
        return defaultVal;
    }
    return type;
}

template<> 
Expected< nodelogic::PieChart::PieChartType >  String2T    ( const std::string & s )
{ 
    return String2Enum( pct2s, s ); 
}

} // SerializationHelper



namespace nodelogic {

const std::string           PieChart::m_type = "pie_chart";

const std::string           PieChart::ACTION::LOAD                 = "Load";
const std::string           PieChart::ACTION::UPDATE_PIECHART     = "UpdatePieSlice";
const std::string           PieChart::ACTION::ADD_PIESLICE        = "AddPieSlice";
const std::string           PieChart::ACTION::REMOVE_PIESLICE     = "RemovePieSlice";
	

// *******************************
//
                            PieChart::PieSliceDesc::PieSliceDesc    ( Float32 p, Float32 o )
    : percent( p )
    , offset( o )
{
}

// *******************************
//
PieChart::PieSliceDescPtr   PieChart::PieSliceDesc::Create          ( const IDeserializer & deser )
{
    auto percent = SerializationHelper::String2T< Float32 >( deser.GetAttribute( "percent" ), 0.f );
    auto offset = SerializationHelper::String2T< Float32 >( deser.GetAttribute( "offset" ), 0.f );

	return std::make_shared< PieSliceDesc >( percent, offset );
}

// *******************************
//
void	                    PieChart::PieSliceDesc::Serialize		( ISerializer & ser ) const
{
    ser.SetAttribute( "percent", SerializationHelper::T2String( percent ) );
    ser.SetAttribute( "offset", SerializationHelper::T2String( offset ) );
}

// *******************************
//
PieChartPtr	                PieChart::Create				( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval, const std::vector< PieSliceDescPtr > & slicesDesc, PieChartType chartType )
{
	return std::make_shared< PieChart >( parent, timeEval, slicesDesc, chartType );
}

// *******************************
//
PieChart::PieChart          ( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval, const std::vector< PieSliceDescPtr > & slicesDesc, PieChartType chartType )
    : m_parentNode( parent )
    , m_timeEval( timeEval )
    , m_slicesDesc( slicesDesc )
    , m_chartType( chartType )
{
}

// *******************************
//
PieChart::~PieChart     ()
{
}

// *******************************
//
void		        PieChart::Update			( TimeType )
{
}

// ***********************
//
void                PieChart::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );

    ser.SetAttribute( "type", m_type );

    if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
    {
        /*ser.SetAttribute( "textureEnabled", SerializationHelper::T2String( m_textureEnabled ) );
        ser.SetAttribute( "materialEnabled", SerializationHelper::T2String( m_materialEnabled ) );*/

        ser.SetAttribute( "pieChartType", SerializationHelper::T2String( m_chartType ) );

        auto timeline = model::TimelineManager::GetInstance()->GetTimelinePath( m_timeEval );
        ser.SetAttribute( "timelinePath", timeline );
    }

    ser.ExitChild();
}

// ***********************
//
PieChartPtr             PieChart::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parent )
{
    auto timelinePath = deser.GetAttribute( "timelinePath" );

    if( !timelinePath.empty() )
    {
        auto deserContext = static_cast< BVDeserializeContext * >( deser.GetDeserializeContext() );
        if( deserContext )
        {
            model::ITimeEvaluatorPtr sceneTimeline = deserContext->GetSceneTimeline();
            if( !sceneTimeline )
            {
                sceneTimeline = model::TimelineManager::GetInstance()->GetRootTimeline();
            }
            auto timeEval = bv::model::TimelineHelper::GetTimeEvaluator( timelinePath, sceneTimeline );
        
            auto slicesDesc = SerializationHelper::DeserializeArray< PieSliceDesc >( deser, "pieSlices" );
            
            auto chartType = SerializationHelper::String2T< PieChartType >( deser.GetAttribute( "pieChartType" ), PieChartType::PST_COLOR );

            return PieChart::Create( parent, timeEval, slicesDesc, chartType );
        }
    }
    return nullptr;
}

// ***********************
//
bool                    PieChart::HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    std::string action = eventSer.GetAttribute( "Action" );

    if( action == ACTION::LOAD ) 
    {
        Load( eventSer, editor );
    }
    else if( action == ACTION::UPDATE_PIECHART )
    {

        //response.SetAttribute( "assetPath", m_assetDesc->GetPath() );
        return true;
    }
    else if( action == ACTION::ADD_PIESLICE )
    {
        //m_assetDesc = MeshAssetDesc::Create( eventSer.GetAttribute( "AssetPath" ), "", true );
        //if( m_assetDesc )
        //{
        //    m_asset = LoadTypedAsset< MeshAsset >( m_assetDesc );
        //    return ( m_asset != nullptr );
        //}
        return false;
    }
    else if( action == ACTION::REMOVE_PIESLICE )
    {
        //return MeshInfo( response );
        return false;
    }
    else 
    {
        response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Replicate' command." );
    }

    return false;
}

// ***********************
//
const std::string &         PieChart::Type                  ()
{
    return m_type;
}

// ***********************
//
const std::string &         PieChart::GetType               () const
{
    return Type();
}

// ***********************
//
void                        PieChart::Load                  ( IDeserializer & eventSer, BVProjectEditor * editor )
{
    auto context = static_cast< BVDeserializeContext * >( eventSer.GetDeserializeContext() );
    auto scene = editor->GetModelScene( context->GetSceneName() );
    
    Float32 angle = 0.f;

    for( UInt32 i = 0; i < ( UInt32 )m_slicesDesc.size(); ++i )
    {
        auto & sliceDesc = m_slicesDesc[ i ];
        auto sliceAngle = sliceDesc->percent * 360.f * 0.01f;
        auto midAngle = angle + ( sliceAngle / 2 );

        auto node = model::BasicNode::Create( m_parentNode->GetName() + "_pieSlice" + SerializationHelper::T2String( i ), m_timeEval );
        
        auto transformName = "transform";
        node->AddPlugin( model::DefaultTransformPluginDesc::UID(), transformName, m_timeEval );
        auto transformParam = node->GetPlugin( transformName )->GetParameter( model::DefaultTransformPlugin::PARAM::SIMPLE_TRANSFORM );
        SetParameterRotation( transformParam, 0.0f, glm::vec3( 90.f, 0.f, midAngle ) );
        SetParameterTranslation( transformParam, 0.0f, sliceDesc->offset * glm::vec3( glm::rotate( glm::vec2( 0.f, 1.f ), midAngle ), 0.f ) );


        auto cylinderName = "cylinder";
        node->AddPlugin( model::DefaultCylinder::DefaultCylinderPluginDesc::UID(), cylinderName, m_timeEval );
        auto cylinderPlugin = node->GetPlugin( cylinderName );
        SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::HEIGHT ), 0.0f, 0.1f );
        SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::TESSELATION ), 0.0f, 20 );
        SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::INNERRADIUS ), 0.0f, 0.f );
        SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::OUTERRADIUS ), 0.0f, 1.f );
        SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::OPENANGLE ), 0.f, ( 360.f - sliceAngle ) );
        SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::OPENANGLEMODE ), 0.0, bv::model::DefaultCylinder::DefaultPlugin::OpenAngleMode::SYMMETRIC );


        SetType( node );

        AddText( node, sliceDesc->percent );

        editor->AddChildNode( scene, m_parentNode, node );

        m_slices[ sliceDesc ] = node;

        angle += sliceAngle;
    }
}

// ***********************
//
void                        PieChart::SetType                   ( model::BasicNodePtr node )
{
    static std::random_device rd;
    static std::mt19937 re( rd() );
    static std::uniform_real_distribution< Float32 > ui( 0.f, 1.f );

    if( m_chartType == PieChartType::PST_COLOR )
    {
        auto colorName = "solid color";
        node->AddPlugin( model::DefaultColorPluginDesc::UID(), colorName, m_timeEval );
        SetParameter( node->GetPlugin( colorName )->GetParameter( model::DefaultColorPlugin::PARAM_COLOR ), 0.0f, glm::vec4( ui( re ), ui( re ), ui( re ), 1.f ) );
    }
    else if( m_chartType == PieChartType::PST_MATERIAL )
    {
        auto materialName = "material";
        node->AddPlugin( model::DefaultMaterialPluginDesc::UID(), materialName, m_timeEval );
        SetParameter( node->GetPlugin( materialName )->GetParameter( model::DefaultMaterialPlugin::PARAM::DIFFUSE ), 0.0f, glm::vec4( ui( re ), ui( re ), ui( re ), 1.f ) );
    }
}

// ***********************
//
void                        PieChart::AddText                   ( model::BasicNodePtr node, Float32 percent )
{
    auto textNode = model::BasicNode::Create( node->GetName() + "_label" , m_timeEval );
    
    textNode->AddPlugin( model::DefaultTransformPluginDesc::UID(), "transform", m_timeEval );
    
    auto colorName = "solid color";
    textNode->AddPlugin( model::DefaultColorPluginDesc::UID(), colorName, m_timeEval );
    SetParameter( textNode->GetPlugin( colorName )->GetParameter( model::DefaultColorPlugin::PARAM_COLOR ), 0.0f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );

    auto textName = "text";
    textNode->AddPlugin( model::DefaultTextPluginDesc::UID(), textName, m_timeEval );
    SetParameter( textNode->GetPlugin( textName )->GetParameter( "text" ), 0.f, SerializationHelper::T2String( percent ) );

    node->AddChildToModelOnly( textNode );
}

} //nodelogic
} //bv