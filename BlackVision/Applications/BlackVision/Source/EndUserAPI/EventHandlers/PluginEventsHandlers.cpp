#include "PluginEventsHandlers.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/AssetManager.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"

#include "Engine/Models/BVProjectEditor.h"
#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"

#include "EventHandlerHelpers.h"
#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa
#include "Serialization/SerializationHelper.h"
#include "Engine/Events/EventManager.h"

namespace bv
{

namespace
{
    const int     DEFAULT_INT_VALUE = 0;
    const bool    DEFAULT_BOOL_VALUE = false;
    const int     DEFAULT_ENUM_VALUE = 0;         // This value should always exist as enum, but maybe there's other better choise.

} // annonymous


// *********************************
// constructor destructor
PluginEventsHandlers::PluginEventsHandlers(  BVAppLogic* logic )
    : m_appLogic( logic )
{}

PluginEventsHandlers::~PluginEventsHandlers()
{}

// *********************************
//
void PluginEventsHandlers::ParamHandler( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != bv::ParamKeyEvent::Type() )
        return;

    bv::ParamKeyEventPtr setParamEvent = std::static_pointer_cast<bv::ParamKeyEvent>( eventPtr );
        
    ParamKeyEvent::Command command  = setParamEvent->ParamCommand;
    ParamKeyEvent::TargetType targetType = setParamEvent->ParamTargetType;

    std::string& nodeName   = setParamEvent->NodeName;
    std::string& pluginName = setParamEvent->PluginName;
    std::string& paramName  = setParamEvent->ParamName;
    std::string& sceneName  = setParamEvent->SceneName;
    std::wstring& value     = setParamEvent->Value;

    float keyTime           = setParamEvent->Time;

    IParameterPtr param;
    if( pluginName == "transform" )     // Hack for transformations. Maybe it's eternal hack.
        param = GetPluginParameter( sceneName, nodeName, pluginName, "simple_transform" );
    else
    {
        if( targetType == ParamKeyEvent::TargetType::PluginParam )
            param = GetPluginParameter( sceneName, nodeName, pluginName, paramName );
        else if( targetType == ParamKeyEvent::TargetType::GlobalEffectParam )
            param = GetGlobalEffectParameter( sceneName, nodeName, paramName );
        else if( targetType == ParamKeyEvent::TargetType::ResourceParam )
            param = GetResourceParameter( sceneName, nodeName, pluginName, "Tex0", paramName ); // @todo Change tex0 to generic method of getting resources
        else
            param = GetPluginParameter( sceneName, nodeName, pluginName, paramName );           // Temporary for backward compatibility
    }

    if( param == nullptr )
    {
        SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Parameter not found" );
        return;
    }

    bool result = true;

    if( command == ParamKeyEvent::Command::AddKey )
    {
        if( pluginName == "transform" )     // Only transform parameter
        {
            std::string stringValue = toString( value );
            if( paramName == "translation" )
            {
                SetParameterTranslation( param, 0, (bv::TimeType)keyTime, SerializationHelper::String2Vec3( stringValue ) );
                LOG_MESSAGE( SeverityLevel::info ) << "AddParamKey() Node [" + nodeName + "] translation: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            }
            else if( paramName == "scale" )
            {
                SetParameterScale( param, 0, (bv::TimeType)keyTime, SerializationHelper::String2Vec3( stringValue ) );
                LOG_MESSAGE( SeverityLevel::info ) << "AddParamKey() Node [" + nodeName + "] scale: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            }
            else if( paramName == "rotation" )
            {
                glm::vec4 rotAxisAngle = SerializationHelper::String2Vec4( stringValue );
                glm::vec3 rotAxis = glm::vec3( rotAxisAngle );

                SetParameterRotation( param, 0, (bv::TimeType)keyTime, rotAxis, rotAxisAngle.w );
                LOG_MESSAGE( SeverityLevel::info ) << "AddParamKey() Node [" + nodeName + "] rotation: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            }
        }
        else
            AddParameter( param, value, (TimeType)keyTime );        // Regular parameter
    }
    else if( command == ParamKeyEvent::Command::SetInterpolatorType )
        result = BezierSetCurveType( param, SerializationHelper::String2T( toString( value ), CurveType::CT_BEZIER ) );
    else if( command == ParamKeyEvent::Command::SetInterpolatorPreWrapMethod )
        result = SetWrapPreMethod( param, SerializationHelper::String2T( toString( value ), WrapMethod::clamp ) );
    else if( command == ParamKeyEvent::Command::SetInterpolatorPostWrapMethod )
        result = SetWrapPostMethod( param, SerializationHelper::String2T( toString( value ), WrapMethod::clamp ) );
    else if( command == ParamKeyEvent::Command::AssignTimeline )
    {
        auto svalue = toString( value ); //timeline path (with scene timeline name)
        auto timeEval = m_appLogic->GetBVProject()->GetProjectEditor()->GetTimeEvaluator( svalue );

        if( !timeEval || TimelineHelper::GetSceneName( svalue ) != sceneName )
        {
            SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Timeline not found" );
            return;
        }
        param->SetTimeEvaluator( timeEval );
    }
    else if( command == ParamKeyEvent::Command::RemoveKey )
    {
        if( pluginName == "transform" )     // Only transform parameter
        {
            std::string stringValue = toString( value );
            if( paramName == "translation" )
            {
                RemoveTranslationKey( param, 0, (bv::TimeType)keyTime );
                LOG_MESSAGE( SeverityLevel::info ) << "RemoveParamKey() Node [" + nodeName + "] translation: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            }
            else if( paramName == "scale" )
            {
                RemoveScaleKey( param, 0, (bv::TimeType)keyTime );
                LOG_MESSAGE( SeverityLevel::info ) << "RemoveParamKey() Node [" + nodeName + "] scale: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            }
            else if( paramName == "rotation" )
            {
                RemoveRotationKey( param, 0, (bv::TimeType)keyTime );
                LOG_MESSAGE( SeverityLevel::info ) << "RemoveParamKey() Node [" + nodeName + "] rotation: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            }
        }
        else
            RemoveParameterKey( param, (TimeType)keyTime );
    }
    else
        result = false;

    SendSimpleResponse( command, setParamEvent->EventID, setParamEvent->SocketID, result );
}


// ***********************
//
ParameterPtr PluginEventsHandlers::GetPluginParameter  (    const std::string& sceneName,
                                                            const std::string& nodePath,
                                                            const std::string& pluginName,
                                                            const std::string& paramName )
{
    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
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
ParameterPtr PluginEventsHandlers::GetGlobalEffectParameter(    const std::string& sceneName,
                                                                const std::string& nodePath,
                                                                const std::string& paramName )
{
    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
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
ParameterPtr PluginEventsHandlers::GetResourceParameter    (    const std::string& sceneName,
                                                                const std::string& nodePath,
                                                                const std::string& pluginName,
                                                                const std::string& textureName,
                                                                const std::string& paramName )
{
    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
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
void PluginEventsHandlers::AddParameter        ( std::shared_ptr<model::IParameter>& param, const std::wstring& value, TimeType keyTime )
{
    std::string stringValue = toString( value );
    auto paramType = param->GetType();

    switch( paramType )
    {
        case ModelParamType::MPT_FLOAT:
        {
            float floatValue = stof( stringValue );
            SetParameter( param, (bv::TimeType)keyTime, floatValue );
            break;
        }
        case ModelParamType::MPT_VEC2:
        {
            glm::vec2 vec2Value = SerializationHelper::String2Vec2( stringValue );
            SetParameter( param, (bv::TimeType)keyTime, vec2Value );
            break;
        }
        case ModelParamType::MPT_VEC3:
        {
            glm::vec3 vec3Value = SerializationHelper::String2Vec3( stringValue );
            SetParameter( param, (bv::TimeType)keyTime, vec3Value );
            break;
        }
        case ModelParamType::MPT_VEC4:
        {
            glm::vec4 vec4Value = SerializationHelper::String2Vec4( stringValue );
            SetParameter( param, (bv::TimeType)keyTime, vec4Value );
            break;
        }
        case ModelParamType::MPT_WSTRING:
        {
            SetParameter( param, (bv::TimeType)keyTime, value );
            break;
        }
        case ModelParamType::MPT_STRING:
        {
            SetParameter( param, (bv::TimeType)keyTime, stringValue );
            break;
        }
        case ModelParamType::MPT_INT:
        {
            int intValue = SerializationHelper::String2T<int>( stringValue, DEFAULT_INT_VALUE );
            SetParameter( param, (bv::TimeType)keyTime, intValue );
            break;
        }
        case ModelParamType::MPT_BOOL:
        {
            bool boolValue = SerializationHelper::String2T<bool>( stringValue, DEFAULT_BOOL_VALUE );
            SetParameter( param, (bv::TimeType)keyTime, boolValue );
            break;
        }
        case ModelParamType::MPT_ENUM:
        {
            int enumValue = SerializationHelper::String2T<int>( stringValue, DEFAULT_ENUM_VALUE );
            SetParameter( param, (bv::TimeType)keyTime, static_cast<GenericEnumType>( enumValue ) );
            break;
        }
        case ModelParamType::MPT_MAT2:
        {
            glm::vec4 vec4Value = SerializationHelper::String2Vec4( stringValue );
            glm::mat2 mat2Value( vec4Value.x, vec4Value.y, vec4Value.z, vec4Value.w );
            SetParameter( param, (bv::TimeType)keyTime, mat2Value );
            break;
        }
    }
}


// *********************************
//
void PluginEventsHandlers::LoadAsset( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() == bv::LoadAssetEvent::Type() )
    {
        bv::LoadAssetEventPtr eventLoadAsset = std::static_pointer_cast<bv::LoadAssetEvent>( eventPtr );
        
        std::string& nodeName = eventLoadAsset->NodeName;
        std::string& pluginName = eventLoadAsset->PluginName;
        std::string& sceneName = eventLoadAsset->SceneName;
        std::string& assetData = eventLoadAsset->AssetData;

        auto projectEditor = m_appLogic->GetBVProject()->GetProjectEditor();
        bool result = projectEditor->LoadAsset( sceneName, nodeName, pluginName, assetData );

        if( result )
            LOG_MESSAGE( SeverityLevel::info ) << "Asset loaded succesfully. Node: [" + eventLoadAsset->NodeName + "] plugin [" + eventLoadAsset->PluginName + "]";
        else
            LOG_MESSAGE( SeverityLevel::error ) << "Failed to load asset. Node [" + eventLoadAsset->NodeName + "] plugin [" + eventLoadAsset->PluginName + "]\n" << assetData;

        SendSimpleResponse( LoadAssetEvent::Command::LoadAsset, eventLoadAsset->EventID, eventLoadAsset->SocketID, result );
    }
}

// ***********************
//
void PluginEventsHandlers::TimerHandler        ( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != bv::TimerEvent::Type() )
        return;

    bv::TimerEventPtr evtTimer = std::static_pointer_cast<bv::TimerEvent>( eventPtr );
	BVProjectPtr modelScene = m_appLogic->GetBVProject();
    auto root = modelScene->GetModelSceneRoot();
        
    std::string & nodePath = evtTimer->NodeName;
    std::string& sceneName = evtTimer->SceneName;
    TimerEvent::Command command = evtTimer->TimerCommand;
    float hours = evtTimer->Hours;
    float minutes = evtTimer->Minutes;
    float seconds = evtTimer->Seconds;
    float millis = evtTimer->Milliseconds;

    TimeType time = ( hours * 3600.0f + minutes * 60.0f + seconds ) + millis * 0.001f;

    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
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


} //bv
