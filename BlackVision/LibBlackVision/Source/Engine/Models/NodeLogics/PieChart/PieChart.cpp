#include "stdafx.h"

#include "PieChart.h"

#include <random>
#include <iomanip>
#include "Mathematics/glm_inc.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCylinderPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultMaterialPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"

#include "Serialization/BV/BVDeserializeContext.h"
#include "Tools/StringHeplers.h"

#include "Engine/Editors/BVProjectEditor.h"


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


// *******************************


namespace nodelogic {


const std::string           PieChart::m_type = "pie_chart";

// *******************************

const std::string           PieChart::ACTION::UPDATE_PIECHART     = "UpdatePieChart";
const std::string           PieChart::ACTION::UPDATE_PIESLICE     = "UpdatePieSlice";
const std::string           PieChart::ACTION::ADD_PIESLICE        = "AddPieSlice";
const std::string           PieChart::ACTION::REMOVE_PIESLICE     = "RemovePieSlice";
    
// *******************************

const std::string           PieChart::PLUGIN::TRANSFORM           = "transform";
const std::string           PieChart::PLUGIN::CYLINDER            = "cylinder";
const std::string           PieChart::PLUGIN::COLOR               = "solid color";
const std::string           PieChart::PLUGIN::MATERIAL            = "material";
const std::string           PieChart::PLUGIN::TEXT                = "text";


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
void                        PieChart::PieSliceDesc::Serialize       ( ISerializer & ser ) const
{
    ser.SetAttribute( "percent", SerializationHelper::T2String( percent ) );
    ser.SetAttribute( "offset", SerializationHelper::T2String( offset ) );
}

// *******************************
//
PieChartPtr                 PieChart::Create                ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, PieChartType chartType, bool textEnabled )
{
    return std::make_shared< PieChart >( parent, timeEval, chartType, textEnabled );
}

// *******************************
//
PieChart::PieChart          ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, PieChartType chartType, bool textEnabled )
    : m_parentNode( parent )
    , m_timeEval( timeEval )
    , m_chartType( chartType )
    , m_textEnabled( textEnabled )
    , m_totalPercent( 0.f )
{
}

// *******************************
//
PieChart::~PieChart     ()
{
}

// *******************************
//
void                PieChart::Update            ( TimeType )
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
        ser.SetAttribute( "textEnabled", SerializationHelper::T2String( m_textEnabled ) );

        auto timeline = model::TimelineManager::GetInstance()->GetTimelinePath( m_timeEval );
        ser.SetAttribute( "timelinePath", timeline );
    }

    ser.ExitChild();
}

// ***********************
//
PieChartPtr             PieChart::Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parent )
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
        
            auto chartType = SerializationHelper::String2T< PieChartType >( deser.GetAttribute( "pieChartType" ), PieChartType::PST_COLOR );
            auto textEnabled = SerializationHelper::String2T< bool >( deser.GetAttribute( "textEnabled" ), true );

            return PieChart::Create( parent, timeEval, chartType, textEnabled );
        }
    }
    return nullptr;
}

// ***********************
//
bool                    PieChart::HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    std::string action = eventSer.GetAttribute( "Action" );

    auto context = static_cast< BVDeserializeContext * >( eventSer.GetDeserializeContext() );
    auto scene = editor->GetModelScene( context->GetSceneName() );

    if( action == ACTION::UPDATE_PIECHART )
    {
        UpdateChart();
        return true;
    }
    else if( action == ACTION::UPDATE_PIESLICE )
    {
        auto sliceDescIdx = SerializationHelper::String2T< UInt32 >( eventSer.GetAttribute( "pieSliceIdx" ), 0 );
        if( RemoveSlice( scene, sliceDescIdx, editor ) )
        {
            auto sliceDesc = SerializationHelper::DeserializeObject< PieSliceDesc >( eventSer, "pieSlice" );
            if( sliceDesc )
            {
                AddSlice( scene, sliceDesc, sliceDescIdx, editor );
                UpdateChart();
                return true;
            }
        }
        return false;
    }
    else if( action == ACTION::ADD_PIESLICE )
    {
        auto sliceDesc = SerializationHelper::DeserializeObject< PieSliceDesc >( eventSer, "pieSlice" );
        if( sliceDesc )
        {
            AddSlice( scene, sliceDesc, ( UInt32 )m_slicesDesc.size(), editor );
            UpdateChart();
            return true;
        }
        return false;
    }
    else if( action == ACTION::REMOVE_PIESLICE )
    {
        auto sliceDescIdx = SerializationHelper::String2T< UInt32 >( eventSer.GetAttribute( "pieSliceIdx" ), 0 );
        if( RemoveSlice( scene, sliceDescIdx, editor ) )
        {
            UpdateChart();
            return true;
        }
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
void                        PieChart::AddSlice           ( model::SceneModelPtr scene, PieSliceDescPtr sliceDesc, UInt32 idx, BVProjectEditor * editor )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        auto node = CreateSlice( sliceDesc, ( UInt32 )m_slicesDesc.size() );
        if( node )
        {
            editor->AddChildNode( scene, parentNode, node );
            if( idx < ( UInt32 )m_slicesDesc.size() )
            {
                editor->MoveNode( scene, parentNode, idx, scene, parentNode, node );
            }

            m_slicesDesc.push_back( sliceDesc );
        }
    }
}

// ***********************
//
bool                        PieChart::RemoveSlice           ( model::SceneModelPtr scene, UInt32 sliceDescIdx, BVProjectEditor * editor )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        if( sliceDescIdx < m_slicesDesc.size() )
        {
            auto desc = m_slicesDesc[ sliceDescIdx ];
            auto sliceNode = parentNode->GetChild( sliceDescIdx );
            if( sliceNode )
            {
                editor->DeleteChildNode( scene, parentNode, sliceNode );
                m_slicesDesc.erase( m_slicesDesc.begin() + sliceDescIdx );

                return true;
            }
        }
    }

    return false;
}

// ***********************
//
model::BasicNodePtr         PieChart::CreateSlice           ( PieSliceDescPtr sliceDesc, UInt32 idx )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        if( sliceDesc->percent > 0 )
        {
            auto node = model::BasicNode::Create( SliceNodeName( parentNode->GetName(), idx ), m_timeEval );

            node->AddPlugin( model::DefaultTransformPluginDesc::UID(), PLUGIN::TRANSFORM, m_timeEval );
            node->AddPlugin( model::DefaultCylinder::DefaultCylinderPluginDesc::UID(), PLUGIN::CYLINDER, m_timeEval );

            AddShaderPlugin( node );

            if( m_textEnabled )
            {
                AddLabelNode( node, sliceDesc->percent );
            }

            auto cylinderPlugin = node->GetPlugin( PLUGIN::CYLINDER );
            SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::HEIGHT ), 0.0f, 0.1f );
            SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::TESSELATION ), 0.0f, 50 );
            SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::INNERRADIUS ), 0.0f, 0.f );
            SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::OUTERRADIUS ), 0.0f, 1.f );
            SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::OPENANGLEMODE ), 0.0, model::DefaultCylinder::DefaultPlugin::OpenAngleMode::SYMMETRIC );

            return node;
        }
    }

    return nullptr;
}

// ***********************
//
void                        PieChart::AddShaderPlugin           ( model::BasicNodePtr node )
{
    static std::random_device rd;
    static std::mt19937 re( rd() );
    static std::uniform_real_distribution< Float32 > ui( 0.f, 1.f );

    if( m_chartType == PieChartType::PST_COLOR )
    {
        node->AddPlugin( model::DefaultColorPluginDesc::UID(), PLUGIN::COLOR, m_timeEval );
        SetParameter( node->GetPlugin( PLUGIN::COLOR )->GetParameter( model::DefaultColorPlugin::PARAM::COLOR ), 0.0f, glm::vec4( ui( re ), ui( re ), ui( re ), 1.f ) );
    }
    else if( m_chartType == PieChartType::PST_MATERIAL )
    {
        node->AddPlugin( model::DefaultMaterialPluginDesc::UID(), PLUGIN::MATERIAL, m_timeEval );
        SetParameter( node->GetPlugin( PLUGIN::MATERIAL )->GetParameter( model::DefaultMaterialPlugin::PARAM::DIFFUSE ), 0.0f, glm::vec4( ui( re ), ui( re ), ui( re ), 1.f ) );
    }
}

// ***********************
//
void                        PieChart::AddLabelNode              ( model::BasicNodePtr node, Float32 percent )
{
    auto textNode = model::BasicNode::Create( LabelNodeName( node->GetName() ), m_timeEval );
    
    textNode->AddPlugin( model::DefaultTransformPluginDesc::UID(), PLUGIN::TRANSFORM, m_timeEval );
    textNode->AddPlugin( model::DefaultColorPluginDesc::UID(), PLUGIN::COLOR, m_timeEval );
    textNode->AddPlugin( model::DefaultTextPluginDesc::UID(), PLUGIN::TEXT, m_timeEval );

    SetLabelTransform( node, textNode );
    SetParameter( textNode->GetPlugin( PLUGIN::COLOR )->GetParameter( model::DefaultColorPlugin::PARAM::COLOR ), 0.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
    SetLabelText( textNode, percent );

    node->AddChildToModelOnly( textNode );
}

// ***********************
//
void                        PieChart::UpdateLabelNode           ( model::BasicNodePtr node, Float32 percent )
{
    auto textNode = std::static_pointer_cast< model::BasicNode >( node->GetChild( LabelNodeName( node->GetName() ) ) );
    if( textNode )
    {
        SetLabelTransform( node, textNode );
        SetLabelText( textNode, percent );
    }
}

// ***********************
//
void                        PieChart::SetLabelTransform         ( model::BasicNodePtr node, model::BasicNodePtr textNode )
{
    auto parentTransformPlugin = node->GetPlugin( PLUGIN::TRANSFORM );
    auto textTransformPlugin = textNode->GetPlugin( PLUGIN::TRANSFORM );
    if( parentTransformPlugin && textTransformPlugin )
    {
        auto parentTransformParam = parentTransformPlugin->GetParamTransform();
        auto parentRotation = parentTransformParam->GetTransform().GetRotation( 0.f );
      
        auto transformParam = textTransformPlugin->GetParameter( model::DefaultTransformPlugin::PARAM::SIMPLE_TRANSFORM );
        SetParameterRotation( transformParam, 0.f, glm::vec3( -parentRotation.x, -parentRotation.z, 0.f ) );
        SetParameterScale( transformParam, 0.f, glm::vec3( 2.f, 2.f, 2.f ) );
        SetParameterTranslation( transformParam, 0.f, glm::vec3( -0.1f, 0.f, -1.4f ) );
    }
}

// ***********************
//
void                        PieChart::SetLabelText              ( model::BasicNodePtr textNode, Float32 percent )
{
    auto textPlugin = textNode->GetPlugin( PLUGIN::TEXT );
    if( textPlugin )
    {
        std::ostringstream out;
        out << std::setprecision( 3 ) << percent;
        auto wtext = StringToWString( out.str() );
        SetParameter( textPlugin->GetParameter( model::DefaultTextPlugin::PARAM::TEXT ), 0.f, wtext.ham );
    }
}

// ***********************
//
void                        PieChart::UpdateChart               ()
{
    if( auto parentNode = m_parentNode.lock() )
    {
        m_totalPercent = 0.f;
        Float32 angle = 0.f;

        for( UInt32 i = 0; i < parentNode->GetNumChildren(); ++i )
        {
            angle = UpdateSlice( parentNode->GetChild( i ), m_slicesDesc[ i ], angle );
        }
    }
}

// ***********************
//
Float32                     PieChart::UpdateSlice               ( model::BasicNodePtr node, PieSliceDescPtr sliceDesc, Float32 angle )
{
    if( m_totalPercent + sliceDesc->percent > 100.f )
    {
        sliceDesc->percent = 100.f - m_totalPercent;
    }
    m_totalPercent += sliceDesc->percent;

    auto sliceAngle = sliceDesc->percent * 360.f * 0.01f;

    auto midAngle = angle + ( sliceAngle / 2 );
    if( angle == 0.f )
    {
        midAngle = 0.f;
    }

    auto translation = glm::vec3( glm::rotate( glm::vec2( 0.f, 1.f ), midAngle ), 0.f );
    
    auto transformPlugin = node->GetPlugin( PLUGIN::TRANSFORM );
    if( transformPlugin )
    {
        auto transformParam = transformPlugin->GetParameter( model::DefaultTransformPlugin::PARAM::SIMPLE_TRANSFORM );
        SetParameterRotation( transformParam, 0.0f, glm::vec3( 90.f, 0.f, midAngle ) );
        SetParameterTranslation( transformParam, 0.0f, sliceDesc->offset * translation );
    }

    auto cylinderPlugin = node->GetPlugin( PLUGIN::CYLINDER );
    if( cylinderPlugin )
    {
        SetParameter( cylinderPlugin->GetParameter( model::DefaultCylinder::PN::OPENANGLE ), 0.f, ( 360.f - sliceAngle ) );
    }

    if( m_textEnabled )
    {
        UpdateLabelNode( node, sliceDesc->percent );
    }

    if( angle == 0.f )
    {
        angle += sliceAngle / 2.f;
    }
    else
    {
        angle += sliceAngle;
    }

    return angle;
}

// ***********************
//
std::string                 PieChart::LabelNodeName             ( const std::string & parentNodeName )
{
    return parentNodeName + "_label";
}

// ***********************
//
std::string                 PieChart::SliceNodeName             ( const std::string & parentNodeName, UInt32 idx )
{
    return parentNodeName + "_pieSlice" + SerializationHelper::T2String( idx );
}

} //nodelogic
} //bv