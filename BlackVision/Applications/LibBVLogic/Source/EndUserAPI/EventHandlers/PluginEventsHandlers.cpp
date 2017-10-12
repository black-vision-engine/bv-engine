#include "stdafxBVApp.h"

#include "PluginEventsHandlers.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "BVAppLogic.h"
#include "UseLoggerBVAppModule.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Serialization/SerializationHelper.h"
#include "Engine/Events/EventManager.h"

#include "Engine/Editors/EditorVariables/ParametersDescriptors/EndUserParamsLogic.h"

#include "Tools/StringHeplers.h"

namespace bv
{

// *********************************
//
PluginEventsHandlers::PluginEventsHandlers( BVAppLogic * logic )
{
    m_projectEditor = logic->GetBVProject()->GetProjectEditor();
}

// *********************************
//
PluginEventsHandlers::~PluginEventsHandlers()
{
}

// *********************************
//
void PluginEventsHandlers::ParamHandler( IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != ParamKeyEvent::Type() )
    {
        return;
    }

    ParamKeyEventPtr setParamEvent = std::static_pointer_cast<ParamKeyEvent>( eventPtr );
        
    ParamKeyEvent::Command command  = setParamEvent->ParamCommand;
    ParameterAddress::TargetType targetType = setParamEvent->ParamAddress.ParamTargetType;

    std::string & nodeName     = setParamEvent->ParamAddress.NodeName;
    std::string & pluginName   = setParamEvent->ParamAddress.PluginName;
    std::string & paramName    = setParamEvent->ParamAddress.ParamName;
    std::string & paramSubName = setParamEvent->ParamAddress.ParamSubName;
    std::string & sceneName    = setParamEvent->ParamAddress.SceneName;
    std::string & value        = setParamEvent->Value;
    
    
    TimeType keyTime           = setParamEvent->Time;

    model::IParameterPtr param = nullptr;

    // FIXME: should be set externally
    bool enableUndo = false;

    if( command != ParamKeyEvent::Command::ListParameters )
    {
        //<------- preserve compatibility code - delete me later & uncomment code below -------
        if( paramSubName.empty() )
        {
            if( pluginName == "transform" )     // Hack for transformations. Maybe it's eternal hack.
            {
                param = GetPluginParameter( m_projectEditor, sceneName, nodeName, pluginName, "simple_transform" );
                paramSubName = paramName;
            }
            else if( pluginName == "texture" && ( paramName == "translation" || paramName == "scale" || paramName == "rotation" ) )     // Hack for transformations. Maybe it's eternal hack.
            {
                param = GetPluginParameter( m_projectEditor, sceneName, nodeName, pluginName, "txMat" );
                paramSubName = paramName;
            }
            else if( targetType == ParameterAddress::TargetType::ResourceParam )
                param = GetResourceParameter( m_projectEditor, sceneName, nodeName, pluginName, "Tex0", paramName );
            else if( targetType == ParameterAddress::TargetType::NodeLogicParam )
                param = GetNodeLogicParameter( m_projectEditor, sceneName, nodeName, paramName );

        }

        if( !param )
        {
            param = GetParameter( m_projectEditor, setParamEvent->ParamAddress );
            if( !param )
                param = GetPluginParameter( m_projectEditor, sceneName, nodeName, pluginName, paramName ); // Temporary for backward compatibility
        }
        // ------- preserve compatibility code ------->

        /* uncomment me on 'preserve compatibility code' deletion
        param = GetParameter( setParamEvent->ParamAddress );
        */

        if( param == nullptr )
        {
            SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Parameter not found" );
            return;
        }
    }

    bool result = false;

    bool keyTimeIsNaN = mathematics::IsNaN( keyTime );

    if( !keyTimeIsNaN && command == ParamKeyEvent::Command::AddKey )
    {
        if( param->GetType() == ModelParamType::MPT_TRANSFORM ) //FIXME: special case for transform param
        {
            result = AddTransformKey( param, paramSubName, keyTime, value );
        }
        else
        {
            result = AddParameter( param, value, keyTime );
        }
    }
    else if( !keyTimeIsNaN && command == ParamKeyEvent::Command::RemoveKey )
    {
        if( param->GetType() == ModelParamType::MPT_TRANSFORM ) // FIXME: special case for transform param
        {
            result = RemoveTransformKey( param, paramSubName, keyTime );
        }
        else
        {
            result = RemoveParameterKey( param, keyTime );
        }
    }
    else if( !keyTimeIsNaN && command == ParamKeyEvent::Command::MoveKey )
    {
        TimeType newKeyTime = SerializationHelper::String2T( value, std::numeric_limits<TimeType>::quiet_NaN() );

        if( newKeyTime != std::numeric_limits<TimeType>::quiet_NaN() )
        {
            if( param->GetType() == ModelParamType::MPT_TRANSFORM ) //FIXME: special case for transform param
            {
                result = MoveTransformKey( param, paramSubName, keyTime, newKeyTime );
            }
            else
            {
                result = MoveParameterKey( param, (TimeType)keyTime, newKeyTime );
            }
        }
    }
    else if( command == ParamKeyEvent::Command::SetInterpolatorType )
        result = BezierSetGlobalCurveType( param, SerializationHelper::String2T( value, CurveType::CT_BEZIER ) );
    else if( command == ParamKeyEvent::Command::SetAddedInterpolatorType )
        result = BezierSetAddedKeyCurveType( param, SerializationHelper::String2T( value, CurveType::CT_BEZIER ) );
    else if( command == ParamKeyEvent::Command::SetInterpolatorPreWrapMethod )
        result = SetWrapPreMethod( param, SerializationHelper::String2T( value, WrapMethod::clamp ) );
    else if( command == ParamKeyEvent::Command::SetInterpolatorPostWrapMethod )
        result = SetWrapPostMethod( param, SerializationHelper::String2T( value, WrapMethod::clamp ) );
    else if( command == ParamKeyEvent::Command::AssignTimeline )
    {
        result = m_projectEditor->AssignTimeline( sceneName, param, value, enableUndo );
    }
    else if( command == ParamKeyEvent::Command::SampleCurve )
    {
        auto params = Split( value, "," );
        
        if( params.size() == 3 )
        {
            auto start = SerializationHelper::String2T< TimeType >( params[ 0 ] );
            auto end = SerializationHelper::String2T< TimeType >( params[ 1 ] );
            auto steps = SerializationHelper::String2T< UInt32 >( params[ 2 ] );

            if( start.IsValid() && end.IsValid() & steps.IsValid() )
            {
                JsonSerializeObject responseJSON;

                responseJSON.EnterArray( "samples" );

                for( UInt32 i = 0; i < steps; i++ )
                {
                    TimeType t = start + ( end - start ) / steps * i;
                    responseJSON.EnterChild( "sample" );
                    auto val = EvaluateParamToString( param, t );
                    responseJSON.SetAttribute( "t", SerializationHelper::T2String( t ) );
                    responseJSON.SetAttribute( "val", SerializationHelper::T2String( val ) );
                    responseJSON.ExitChild(); // sample
                }
                responseJSON.ExitChild(); // quants

                SendResponse( responseJSON, setParamEvent->SocketID, setParamEvent->EventID );
            }
        }
        result = false;
    }
    else if( command == ParamKeyEvent::Command::ListKeys )
    {
        JsonSerializeObject responseJSON;
        PrepareResponseTemplate( responseJSON, command, setParamEvent->EventID, true );

        setParamEvent->ParamAddress.Serialize( responseJSON );

        if( param->GetType() == ModelParamType::MPT_TRANSFORM ) //FIXME: special case for transform param
        {
            auto transformKind = SerializationHelper::String2T( paramSubName, TransformKind::invalid );
            model::QueryTypedParam< model::ParamTransformPtr >( param )->Serialize( responseJSON, transformKind );
        }
        else
        {
            param->Serialize( responseJSON );
        }

        SendResponse( responseJSON, setParamEvent->SocketID, setParamEvent->EventID );
        
        return;
    }
    else if( command == ParamKeyEvent::Command::ListParameters )
    {
        Expected< std::vector< ParameterPtr > > paramsList = ListParameters( setParamEvent->ParamAddress );

        if( paramsList.IsValid() )
        {
            JsonSerializeObject responseJSON;
            PrepareResponseTemplate( responseJSON, command, setParamEvent->EventID, true );

            setParamEvent->ParamAddress.Serialize( responseJSON );

            responseJSON.EnterArray( "Parameters" );

            for( auto & listedParam : paramsList.GetVal() )
                listedParam->Serialize( responseJSON );

            responseJSON.ExitChild();   // Parameters

            SendResponse( responseJSON, setParamEvent->SocketID, setParamEvent->EventID );
            return;
        }
    }

    if( result )
    {
        LOG_MESSAGE( SeverityLevel::info ) << SerializationHelper::T2String( command ) + " Node [" + nodeName + "] Plugin [" + pluginName + "] Param [" + paramName + " " + paramSubName + "] : (" + SerializationHelper::T2String( value ) + ") key: " + SerializationHelper::T2String( keyTime ) + " s";
    }

    SendSimpleResponse( command, setParamEvent->EventID, setParamEvent->SocketID, result );
}

// ***********************
//
bool PluginEventsHandlers::AddParameter        ( std::shared_ptr< model::IParameter > & param, const std::string & stringValue, TimeType keyTime )
{
    auto paramType = param->GetType();

    switch( paramType )
    {
        case ModelParamType::MPT_FLOAT:
        {
            float floatValue = SerializationHelper::String2T( stringValue, 0.f );

            return SetParameter( param, ( TimeType )keyTime, floatValue );
        }
        case ModelParamType::MPT_VEC2:
        {
            glm::vec2 vec2Value = SerializationHelper::String2T( stringValue, glm::vec2( 0.f ) );

            return SetParameter( param, ( TimeType )keyTime, vec2Value );
        }
        case ModelParamType::MPT_VEC3:
        {
            glm::vec3 vec3Value = SerializationHelper::String2T( stringValue, glm::vec3( 0.f ) );

            return SetParameter( param, ( TimeType )keyTime, vec3Value );
        }
        case ModelParamType::MPT_VEC4:
        {
            glm::vec4 vec4Value = SerializationHelper::String2T( stringValue, glm::vec4( 0.f ) );

            return SetParameter( param, ( TimeType )keyTime, vec4Value );
        }
        case ModelParamType::MPT_WSTRING:
        {
            auto wstring = StringToWString( stringValue );
            return SetParameter( param, ( TimeType )keyTime, wstring.GetVal() );
        }
        case ModelParamType::MPT_STRING:
            return SetParameter( param, ( TimeType )keyTime, stringValue );
        case ModelParamType::MPT_INT:
        {
            int intValue = SerializationHelper::String2T( stringValue, 0 );

            return SetParameter( param, ( TimeType )keyTime, intValue );
        }
        case ModelParamType::MPT_BOOL:
        {
            bool boolValue = SerializationHelper::String2T( stringValue, false );

            return SetParameter( param, ( TimeType )keyTime, boolValue );
        }
        case ModelParamType::MPT_ENUM:
        {
            int enumValue = SerializationHelper::String2T( stringValue, 0 );

            return SetParameter( param, ( TimeType )keyTime, static_cast< model::GenericEnumType >( enumValue ) );
        }
        case ModelParamType::MPT_MAT2:
        {
            glm::vec4 vec4Value = SerializationHelper::String2T( stringValue, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
            glm::mat2 mat2Value( vec4Value.x, vec4Value.y, vec4Value.z, vec4Value.w );

            return SetParameter( param, ( TimeType )keyTime, mat2Value );
        }
    }

    return false;
}


// ***********************
//
void PluginEventsHandlers::TimerHandler        ( IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != TimerEvent::Type() )
        return;

    TimerEventPtr evtTimer = std::static_pointer_cast<TimerEvent>( eventPtr );
        
    std::string & nodePath = evtTimer->NodeName;
    std::string& sceneName = evtTimer->SceneName;
    TimerEvent::Command command = evtTimer->TimerCommand;
    float hours = evtTimer->Hours;
    float minutes = evtTimer->Minutes;
    float seconds = evtTimer->Seconds;
    float millis = evtTimer->Milliseconds;

    TimeType time = ( hours * 3600.0f + minutes * 60.0f + seconds ) + millis * 0.001f;

    auto node = m_projectEditor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        SendSimpleErrorResponse( command, evtTimer->EventID, evtTimer->SocketID, "Node not found" );
        return;
    }
       
    bool result = true;
    auto timerplugin = node->GetPlugin("timer");
    if( timerplugin == nullptr )
    {
        SendSimpleErrorResponse( command, evtTimer->EventID, evtTimer->SocketID, "Timer plugin not found" );
        return;
    }

    if( command == TimerEvent::Command::Start )
        StartTimerPlugin( timerplugin );
    else if( command == TimerEvent::Command::Stop )
        StopTimerPlugin( timerplugin );
    else if( command == TimerEvent::Command::Reset )
        ResetTimerPlugin( timerplugin );
    else if( command == TimerEvent::Command::SetTime )
        SetTimeTimerPlugin( timerplugin, time );
    else if( command == TimerEvent::Command::SetTimeStart )
    {
        SetTimeTimerPlugin( timerplugin, time );
        StartTimerPlugin( timerplugin );
    }
    else if( command == TimerEvent::Command::SetTimeStop )
    {
        SetTimeTimerPlugin( timerplugin, time );
        StopTimerPlugin( timerplugin );
    }
    else if( command == TimerEvent::Command::SetTimePatern )
    {
        SetTimePaternTimerPlugin( timerplugin, StringToWString( evtTimer->TimePatern ) );
        StopTimerPlugin( timerplugin );
    }
    else
    {
        SendSimpleErrorResponse( command, evtTimer->EventID, evtTimer->SocketID, "Unknown command" );
        return;
    }

    SendSimpleResponse( command, evtTimer->EventID, evtTimer->SocketID, result );
}

// ***********************
//
bool        PluginEventsHandlers::AddTransformKey        ( ParameterPtr & param, const std::string & paramSubName, TimeType keyTime, const std::string & strValue )
{
    auto transformKind = SerializationHelper::String2T( paramSubName, TransformKind::invalid );

    switch ( transformKind )
    {
        case TransformKind::translation:
            return SetParameterTranslation( param, keyTime, SerializationHelper::String2T( strValue, glm::vec3( 0.f ) ) );
        case TransformKind::rotation:
            return SetParameterRotation( param, keyTime, SerializationHelper::String2T( strValue, glm::vec3( 0.f ) ) );
        case TransformKind::scale:
            return SetParameterScale( param, keyTime, SerializationHelper::String2T( strValue, glm::vec3( 1.f ) ) );
        case TransformKind::center:
            return SetParameterCenterMass( param, keyTime, SerializationHelper::String2T( strValue, glm::vec3( 1.f ) ) );
        default:
            return false;;
    }
}

// ***********************
//
bool        PluginEventsHandlers::RemoveTransformKey        ( ParameterPtr & param, const std::string & paramSubName, TimeType keyTime )
{
    auto transformKind = SerializationHelper::String2T( paramSubName, TransformKind::invalid );

    switch ( transformKind )
    {
        case TransformKind::translation:
            return RemoveTranslationKey( param, keyTime );
        case TransformKind::rotation:
            return RemoveRotationKey( param, keyTime );
        case TransformKind::scale:
            return RemoveScaleKey( param, keyTime );
        case TransformKind::center:
            return RemoveCenterMassKey( param, keyTime );
        default:
            return false;
    }
}

// ***********************
//
bool        PluginEventsHandlers::MoveTransformKey        ( ParameterPtr & param, const std::string & paramSubName, TimeType keyTime, TimeType newTime )
{
    auto transformKind = SerializationHelper::String2T( paramSubName, TransformKind::invalid );

    switch ( transformKind )
    {
        case TransformKind::translation:
            return MoveTranslationKey( param, keyTime, newTime );
        case TransformKind::rotation:
            return MoveRotationKey( param, keyTime, newTime );
        case TransformKind::scale:
            return MoveScaleKey( param, keyTime, newTime );
        case TransformKind::center:
            return MoveCenterMassKey( param, keyTime, newTime );
        default:
            return false;
    }
}

// ***********************
//
Expected< std::vector< ParameterPtr > >         PluginEventsHandlers::ListParameters        ( const ParameterAddress & address )
{
    auto scene = m_projectEditor->GetModelScene( address.SceneName );
    
    if( scene )
    {
        if( address.ParamTargetType == ParameterAddress::TargetType::CameraParam )
        {
            auto camera = scene->GetCamerasLogic().GetCamera( address.Index );
            if( camera )
                return camera->GetParameters();
        }
        else if( address.ParamTargetType == ParameterAddress::TargetType::LightParam )
        {
            auto light = scene->GetLight( address.Index );
            if( light )
                return light->GetParameters();
        }
        else
        {
            auto node = scene->GetModelSceneEditor()->GetNode( address.NodeName );
            if( node )
            {
                if( address.ParamTargetType == ParameterAddress::TargetType::GlobalEffectParam )
                {
                    auto effect = node->GetNodeEffect();
                    if( effect )
                        return effect->GetParameters();
                }
                else if( address.ParamTargetType == ParameterAddress::TargetType::NodeLogicParam )
                {
                    auto logic = node->GetLogic();
                    if( logic )
                        return logic->GetParameters();
                }
                else
                {
                    auto plugin = node->GetPlugin( address.PluginName );
                    if( plugin )
                    {
                        if( address.ParamTargetType == ParameterAddress::TargetType::PluginParam )
                            return plugin->GetParameters();
                        else if( address.ParamTargetType == ParameterAddress::TargetType::ResourceParam )
                        {
                            auto model = plugin->GetResourceStateModel( address.ParamSubName );
                            if( model )
                                return model->GetParameters();
                        }
                    }
                }
            }
        }
    }

    return Expected< std::vector< ParameterPtr > >();
}

// ***********************
//
void        PluginEventsHandlers::ParamDescHandler    ( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != ParamDescriptorEvent::Type() )
        return;

    ParamDescriptorEventPtr paramDescEvent = std::static_pointer_cast< ParamDescriptorEvent >( eventPtr );
    ParamDescriptorEvent::Command command  = paramDescEvent->ParamCommand;


    if( command == ParamDescriptorEvent::Command::Fail )
    {
        SendSimpleErrorResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, "Unknown command" );
        return;
    }

    assert( command == ParamDescriptorEvent::RemoveParamDescriptor || paramDescEvent->Request != nullptr );
    if( paramDescEvent->Request == nullptr && command != ParamDescriptorEvent::RemoveParamDescriptor )
    {
        SendSimpleErrorResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, "Not valid request." );
        return;
    }

    std::string & sceneName = paramDescEvent->ParamAddress.SceneName;

    auto scene = m_projectEditor->GetModelScene( sceneName );
    if( scene == nullptr )
    {
        SendSimpleErrorResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, "Scene not found" );
        return;
    }


    if( command == ParamDescriptorEvent::AddParamDescriptor )
    {
        model::IParameterPtr param = nullptr;
        param = GetParameter( m_projectEditor, paramDescEvent->ParamAddress );

        if( param )
        {
            EndUserParamDescriptor descriptor = EndUserParamDescriptor::Create( *paramDescEvent->Request );
            bool result = scene->GetEndUserParams().AddDescriptor( ParameterAddress( paramDescEvent->ParamAddress ), std::move( descriptor ) );
        
            SendSimpleResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, result );
        }
        else
        {
            SendSimpleErrorResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, "Parameter not found" );
        }
    }
    else if( command == ParamDescriptorEvent::RemoveParamDescriptor )
    {
        bool result = scene->GetEndUserParams().RemoveDescriptor( paramDescEvent->ParamAddress );
        SendSimpleResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, result );
    }
    else if( command == ParamDescriptorEvent::SetDescriptorParameters )
    {
        auto descriptor = scene->GetEndUserParams().GetDescriptor( paramDescEvent->ParamAddress );

        if( descriptor )
        {
            // This code deserializes only parameters and ignores array of time values.
            EndUserParamDescriptor newDesc = EndUserParamDescriptor::Create( *paramDescEvent->Request );

            descriptor->SetName( newDesc.GetName() );
            descriptor->SetDescription( newDesc.GetDescription() );
            descriptor->SetOrder( newDesc.GetOrder() );
            descriptor->Enable( newDesc.IsEnabled() );

            SendSimpleResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, true );
        }
        else
            SendSimpleErrorResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, "Descriptor not found" );
    }
    else if( command == ParamDescriptorEvent::AddAvaibleKeyTimes )
    {
        // This code deserializes only array of time values. Rest is ignored.
        EndUserParamDescriptor descriptor = EndUserParamDescriptor::Create( *paramDescEvent->Request );

        auto desc = scene->GetEndUserParams().GetDescriptor( paramDescEvent->ParamAddress );
        
        if( desc )
        {
            for( auto value : descriptor.GetAvaibleTimeValues() )
                desc->AddKeyTimeValue( value );

            SendSimpleResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, true );
        }
        else
            SendSimpleErrorResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, "Descriptor not found" );
    }
    else if( command == ParamDescriptorEvent::RemoveAvaibleKeyTimes )
    {
        // This code deserializes only array of time values. Rest is ignored.
        EndUserParamDescriptor descriptor = EndUserParamDescriptor::Create( *paramDescEvent->Request );

        auto desc = scene->GetEndUserParams().GetDescriptor( paramDescEvent->ParamAddress );
        
        if( desc )
        {
            for( auto value : descriptor.GetAvaibleTimeValues() )
                desc->RemoveKeyTime( value );

            SendSimpleResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, true );
        }
        else
            SendSimpleErrorResponse( command, paramDescEvent->EventID, paramDescEvent->SocketID, "Descriptor not found" );
    }
}

} //bv
