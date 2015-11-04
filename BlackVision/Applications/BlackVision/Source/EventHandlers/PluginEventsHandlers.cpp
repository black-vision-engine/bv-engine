#include "PluginEventsHandlers.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/AssetManager.h"
#include "Engine/Models/Interfaces/IOverrideState.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"


#include "../BVAppLogic.h"
#include "../UseLogger.h"

#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa
#include "Serialization/SerializationHelper.h"      // Conversions from string to glm types
#include "Engine/Events/EventManager.h"


namespace bv
{

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

    std::string nodeName    = toString( setParamEvent->NodeName );
    std::string pluginName  = toString( setParamEvent->PluginName );
    std::string paramName   = toString( setParamEvent->ParamName );
    std::wstring& value     = setParamEvent->Value;

    float keyTime           = setParamEvent->Time;
        
    auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
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
            // @todo Realy do we want to send messages all the time, when this param changes??
        }
        else if( paramName == "scale" )
        {
            SetParameterScale( param, 0, (bv::TimeType)keyTime, SerializationHelper::String2Vec3( stringValue ) );
            LOG_MESSAGE( SeverityLevel::info ) << "AddParamKey() Node [" + nodeName + "] scale: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            // @todo Realy do we want to send messages all the time, when this param changes??
        }
        else if( paramName == "rotation" )
        {
            glm::vec4 rotAxisAngle = SerializationHelper::String2Vec4( stringValue );
            glm::vec3 rotAxis = glm::vec3( rotAxisAngle );

            SetParameterRotation( param, 0, (bv::TimeType)keyTime, rotAxis, rotAxisAngle.w );
            LOG_MESSAGE( SeverityLevel::info ) << "AddParamKey() Node [" + nodeName + "] rotation: (" + stringValue + ") key: " + std::to_string( keyTime ) + " s";
            // @todo Realy do we want to send messages all the time, when this param changes??
        }
    }
    else if( pluginName == "text")
    {
        if( paramName == "text" )
        {
            DefaultTextPlugin* textPlugin = dynamic_cast< DefaultTextPlugin* >( node->GetPlugin("text").get() );

            if( textPlugin != nullptr )
                model::DefaultTextPlugin::SetText( plugin, value );
            else
                LOG_MESSAGE( SeverityLevel::error ) << "AddParamKey() nNode ["+ nodeName+"], plugin [text] not found";
        }
    }
    else if( setParamEvent->PluginName == "alpha" )
    {
        if( setParamEvent->ParamName == "alpha" )
        {
			auto state = node->GetOverrideState();
			auto alphaParam = state->GetAlphaParam();
				 
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
        }
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

// *********************************
//
void PluginEventsHandlers::LoadAsset( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() == bv::LoadAssetEvent::Type() )
    {
        bv::LoadAssetEventPtr eventLoadAsset = std::static_pointer_cast<bv::LoadAssetEvent>( eventPtr );
        
        std::string nodeName = eventLoadAsset->NodeName;
        std::string pluginName = eventLoadAsset->PluginName;
        std::string asssetData = eventLoadAsset->AssetData;

        auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
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


} //bv
