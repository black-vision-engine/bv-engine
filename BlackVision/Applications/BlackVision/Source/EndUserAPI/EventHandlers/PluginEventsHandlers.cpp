#include "PluginEventsHandlers.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/AssetManager.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"


#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"

#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa
#include "Serialization/SerializationHelper.h"      // Conversions from string to glm types
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
void PluginEventsHandlers::AddParamKey( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != bv::ParamKeyEvent::Type() )
        return;

    bv::ParamKeyEventPtr setParamEvent = std::static_pointer_cast<bv::ParamKeyEvent>( eventPtr );
        
    ParamKeyEvent::Command command  = setParamEvent->KeyCommand;
    if( command != ParamKeyEvent::Command::AddKey )
        return;

    std::string& nodeName   = setParamEvent->NodeName;
    std::string& pluginName = setParamEvent->PluginName;
    std::string& paramName  = setParamEvent->ParamName;
    std::string& sceneName  = setParamEvent->SceneName;
    std::wstring& value     = setParamEvent->Value;

    float keyTime           = setParamEvent->Time;
        
    auto scene = m_appLogic->GetBVProject()->GetScene( sceneName );
    if( scene == nullptr )
        return;

	auto root = scene->GetRootNode();
    auto node = root->GetNode( nodeName );
    if( node == nullptr )
    {
        if( root->GetName() == nodeName )
            node = root;
        else
        {
            LOG_MESSAGE( SeverityLevel::error ) << "AddParamKey() node ["+ nodeName+"] not found";
            return;
        }
    }
    

    auto plugin = node->GetPlugin( pluginName );
    if( plugin == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "AddParamKey() node ["+ nodeName+"], plugin [" + pluginName + "] not found";
        return;
    }

    if( pluginName == "transform" )
    {
        auto param = plugin->GetParameter("simple_transform");
        if( param == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "AddParamKey() node ["+ nodeName+"], plugin [" + pluginName + "], param [simple_transform] not found";
            return;
        }
            
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
    else if( pluginName == "alpha" )
    {
        if( paramName == "alpha" )
        {
            auto sceneNodeEffect = node->GetNodeEffect();

            ParamFloatPtr alphaParam;

            if ( !sceneNodeEffect || sceneNodeEffect->GetType() != NodeEffectType::NET_ALPHA_MASK )
            {
                auto typeEffect = std::static_pointer_cast< model::ModelNodeEffectAlphaMask >( sceneNodeEffect );
                typeEffect->GetParamAlpha();
            }

            if( !alphaParam )
            {
                assert( false );
                return;
            }

			float floatValue = stof( value );

            SetParameter( alphaParam, (bv::TimeType)keyTime, floatValue );
        }
    }
    else
    {
        auto param = plugin->GetParameter( paramName );
        if( param == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "AddParamKey() node ["+ nodeName+"], plugin [" + pluginName + "], param [" + paramName + "] not found";
            return;
        }

        AddParameter( param, value, (TimeType)keyTime );
    }
}


// *********************************
//
void PluginEventsHandlers::UpdateParamKey      ( bv::IEventPtr /*eventPtr*/ )
{
    assert( !"Implement meeee" );
}

// *********************************
//
void PluginEventsHandlers::RemoveParamKey      ( bv::IEventPtr /*eventPtr*/ )
{
    assert( !"Implement meeee" );
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
            SetParameter( param, (bv::TimeType)keyTime, enumValue );
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
        std::string& asssetData = eventLoadAsset->AssetData;

        auto root = m_appLogic->GetBVProject()->GetModelSceneRoot();
        auto node = root->GetNode( nodeName );
        if( node == nullptr )
            return;

        auto plugin = node->GetPlugin( pluginName );
        if( plugin == nullptr )
            return;

        JsonDeserializeObject deserializer;
        deserializer.Load( asssetData );

        bool result = true;
        auto assetDesc = AssetManager::GetInstance().CreateDesc( deserializer );

        if( assetDesc != nullptr )
            result = plugin->LoadResource( assetDesc );
        else
            result = false;

        if( result )
            LOG_MESSAGE( SeverityLevel::info ) << "Asset loaded succesfully. Node: [" + eventLoadAsset->NodeName + "] plugin [" + eventLoadAsset->PluginName + "]";
        else
            LOG_MESSAGE( SeverityLevel::error ) << "Failed to load asset. Node [" + eventLoadAsset->NodeName + "] plugin [" + eventLoadAsset->PluginName + "]\n" << asssetData;
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
        
    std::string& nodeName = evtTimer->NodeName;
    TimerEvent::Command command = evtTimer->TimerCommand;
    float hours = evtTimer->Hours;
    float minutes = evtTimer->Minutes;
    float seconds = evtTimer->Seconds;
    float millis = evtTimer->Milliseconds;

    TimeType time = ( hours * 3600.0f + minutes * 60.0f + seconds ) + millis * 0.001f;

    auto node = root->GetNode( nodeName );

    if( node == nullptr && root->GetName() == nodeName )
    {
        LOG_MESSAGE( SeverityLevel::info ) << "root node is node you're looking for [" + nodeName + "] Applying jedi fix now.";
        node = root;
    }
    if( node == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Error OnSetParam() node [" + nodeName + "] not found";
        return;
    }
       
    auto timerplugin = node->GetPlugin("timer");

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
}


} //bv
