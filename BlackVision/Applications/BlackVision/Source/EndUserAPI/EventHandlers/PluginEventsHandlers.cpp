#include "PluginEventsHandlers.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"

#include "Engine/Models/BVProjectEditor.h"
#include "BVAppLogic.h"
#include "UseLoggerBVAppModule.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Serialization/SerializationHelper.h"
#include "Engine/Events/EventManager.h"

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
    ParamKeyEvent::TargetType targetType = setParamEvent->ParamTargetType;

    std::string & nodeName     = setParamEvent->NodeName;
    std::string & pluginName   = setParamEvent->PluginName;
    std::string & paramName    = setParamEvent->ParamName;
    std::string & paramSubName = setParamEvent->ParamSubName;
    std::string & sceneName    = setParamEvent->SceneName;
    std::string & value        = setParamEvent->Value;
    UInt32        index        = setParamEvent->Index;
    
    
    TimeType keyTime           = setParamEvent->Time;

    IParameterPtr param = nullptr;

    //<------- preserve compatibility code - delete me later & uncomment code below -------
    if( paramSubName.empty() )
    {
        if( pluginName == "transform" )     // Hack for transformations. Maybe it's eternal hack.
        {
            param = GetPluginParameter( sceneName, nodeName, pluginName, "simple_transform" );
            paramSubName = paramName;
        }
        else if( pluginName == "texture" && ( paramName == "translation" || paramName == "scale" || paramName == "rotation" ) )     // Hack for transformations. Maybe it's eternal hack.
        {
            param = GetPluginParameter( sceneName, nodeName, pluginName, "txMat" );
            paramSubName = paramName;
        }
        else if( targetType == ParamKeyEvent::TargetType::ResourceParam )
            param = GetResourceParameter( sceneName, nodeName, pluginName, "Tex0", paramName );
    }

    if( !param )
    {
        if( targetType == ParamKeyEvent::TargetType::PluginParam )
            param = GetPluginParameter( sceneName, nodeName, pluginName, paramName );
        else if( targetType == ParamKeyEvent::TargetType::GlobalEffectParam )
            param = GetGlobalEffectParameter( sceneName, nodeName, paramName );
        else if( targetType == ParamKeyEvent::TargetType::ResourceParam )
            param = GetResourceParameter( sceneName, nodeName, pluginName, paramSubName, paramName );
        else if( targetType == ParamKeyEvent::TargetType::LightParam )
            param = GetLightParameter( sceneName, index, paramName );
        else if( targetType == ParamKeyEvent::TargetType::NodeLogicParam )
            param = GetNodeLogicParameter( sceneName, nodeName, paramName );
        else if( targetType == ParamKeyEvent::TargetType::CameraParam )
            param = GetCameraParameter( sceneName, index, paramName );
        else
            param = GetPluginParameter( sceneName, nodeName, pluginName, paramName ); // Temporary for backward compatibility
    }
    // ------- preserve compatibility code ------->

    /* uncomment me on 'preserve compatibility code' deletion
    if( targetType == ParamKeyEvent::TargetType::PluginParam )
        param = GetPluginParameter( sceneName, nodeName, pluginName, paramName );
    else if( targetType == ParamKeyEvent::TargetType::GlobalEffectParam )
        param = GetGlobalEffectParameter( sceneName, nodeName, paramName );
    else if( targetType == ParamKeyEvent::TargetType::ResourceParam )
        param = GetResourceParameter( sceneName, nodeName, pluginName, paramSubName, paramName );
    else if( targetType == ParamKeyEvent::TargetType::LightParam )
        param = GetLightParameter( sceneName, lightIndex, paramName );
    */

    if( param == nullptr )
    {
        SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Parameter not found" );
        return;
    }

    bool result = false;

    if( command == ParamKeyEvent::Command::AddKey )
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
    else if( command == ParamKeyEvent::Command::RemoveKey )
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
    else if( command == ParamKeyEvent::Command::MoveKey )
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
        auto timeEval = m_projectEditor->GetTimeEvaluator( value );
            
        //FIXME: logic below should be somewhere else - editor maybe
        //don't allow setting scene timeline or timeline from other scene
        if( !timeEval || timeEval->GetName() == sceneName 
            || TimelineHelper::GetSceneName( timeEval.get() ) != sceneName )
        {
            SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Timeline not found" );
            return;
        }

        param->SetTimeEvaluator( timeEval );

        result = true;
    }
    else if( command == ParamKeyEvent::Command::SampleCurve )
    {
        auto params = Split( value, "," );
        
        if( params.size() == 3 )
        {
            auto start = SerializationHelper::String2T< TimeType >( params[ 0 ] );
            auto end = SerializationHelper::String2T< TimeType >( params[ 1 ] );
            auto steps = SerializationHelper::String2T< UInt32 >( params[ 2 ] );

            if( start.isValid && end.isValid & steps.isValid )
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

        responseJSON.SetAttribute( SerializationHelper::SCENE_NAME_STRING, setParamEvent->SceneName );
        responseJSON.SetAttribute( SerializationHelper::NODE_NAME_STRING, setParamEvent->NodeName );
        responseJSON.SetAttribute( SerializationHelper::PLUGIN_NAME_STRING, setParamEvent->PluginName );
        responseJSON.SetAttribute( SerializationHelper::PARAM_NAME_STRING, setParamEvent->ParamName );
        responseJSON.SetAttribute( SerializationHelper::PARAM_SUB_NAME_STRING, setParamEvent->ParamSubName );
        responseJSON.SetAttribute( SerializationHelper::PARAM_TARGET_TYPE_STRING, SerializationHelper::T2String( setParamEvent->ParamTargetType ) );

        if( param->GetType() == ModelParamType::MPT_TRANSFORM ) //FIXME: special case for transform param
        {
            auto transformKind = SerializationHelper::String2T( paramSubName, TransformKind::invalid );
            QueryTypedParam< ParamTransformPtr >( param )->Serialize( responseJSON, transformKind );
        }
        else
        {
            param->Serialize( responseJSON );
        }

        SendResponse( responseJSON, setParamEvent->SocketID, setParamEvent->EventID );
        
        return;
    }

    if( result )
    {
        LOG_MESSAGE( SeverityLevel::info ) << SerializationHelper::T2String( command ) + " Node [" + nodeName + "] Plugin [" + pluginName + "] Param [" + paramName + " " + paramSubName + "] : (" + SerializationHelper::T2String( value ) + ") key: " + SerializationHelper::T2String( keyTime ) + " s";
    }

    SendSimpleResponse( command, setParamEvent->EventID, setParamEvent->SocketID, result );
}


// ***********************
//
ParameterPtr PluginEventsHandlers::GetPluginParameter  (    const std::string & sceneName,
                                                            const std::string & nodePath,
                                                            const std::string & pluginName,
                                                            const std::string & paramName )
{
    auto node = m_projectEditor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto plugin = node->GetPlugin( pluginName );
    if( plugin == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "] not found";
        return nullptr;
    }

    auto param = plugin->GetParameter( paramName );
    if( param == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "], param [" + paramName + "] not found";
        return nullptr;
    }
    return param;
}

// ***********************
//
ParameterPtr PluginEventsHandlers::GetGlobalEffectParameter(    const std::string & sceneName,
                                                                const std::string & nodePath,
                                                                const std::string & paramName )
{
    auto node = m_projectEditor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto effect = node->GetNodeEffect();
    if( effect == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], effect not found";
        return nullptr;
    }

    effect->GetParameter( paramName );
    auto param = effect->GetParameter( paramName );
    if( param == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], param [" + paramName + "] not found";
        return nullptr;
    }
    return param;
}


// ***********************
//
ParameterPtr PluginEventsHandlers::GetResourceParameter    (    const std::string & sceneName,
                                                                const std::string & nodePath,
                                                                const std::string & pluginName,
                                                                const std::string & textureName,
                                                                const std::string & paramName )
{
    auto node = m_projectEditor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto plugin = node->GetPlugin( pluginName );
    if( plugin == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "] not found";
        return nullptr;
    }

    auto resourceModel = plugin->GetResourceStateModel( textureName );
    if( resourceModel == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "], texture [" + textureName + " not found";
        return nullptr;
    }
    
    return resourceModel->GetParameter( paramName );
}

// ***********************
//
ParameterPtr PluginEventsHandlers::GetLightParameter        ( const std::string & sceneName,
                                                              UInt32 lightIndex,
                                                              const std::string & paramName )
{
    auto scene = m_projectEditor->GetModelScene( sceneName );
    if( scene )
    {
        auto light = scene->GetLight( lightIndex );
        if( light )
        {
            return light->GetParameter( paramName );
        }
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: not found";
    return nullptr;
}

// ***********************
//
ParameterPtr    PluginEventsHandlers::GetNodeLogicParameter   ( const std::string & sceneName, const std::string & nodePath, const std::string & paramName )
{
    auto node = m_projectEditor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto logic = node->GetLogic();
    if( logic )
    {
        logic->GetParameter( paramName );
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Node logic parameter not found";
    return nullptr;
}

// ***********************
//
ParameterPtr    PluginEventsHandlers::GetCameraParameter      ( const std::string & sceneName, UInt32 cameraIndex, const std::string & paramName )
{
    auto scene = m_projectEditor->GetModelScene( sceneName );
    if( scene )
    {
        auto camera = scene->GetCamerasLogic().GetCamera( cameraIndex );
        if( camera )
        {
            return camera->GetParameter( paramName );
        }
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Camera parameter not found";
    return nullptr;
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
            return SetParameter( param, ( TimeType )keyTime, wstring.ham );
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

            return SetParameter( param, ( TimeType )keyTime, static_cast< GenericEnumType >( enumValue ) );
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

} //bv
