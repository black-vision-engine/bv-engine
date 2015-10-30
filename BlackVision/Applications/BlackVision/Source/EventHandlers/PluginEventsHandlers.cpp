#include "PluginEventsHandlers.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/AssetManager.h"
#include "../BVAppLogic.h"
#include "../UseLogger.h"

#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa
#include "Serialization/SerializationHelper.h"      // Conversions from string to glm types


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
    if( eventPtr->GetEventType() == bv::ParamKeyEvent::Type() )
    {
        bv::ParamKeyEventPtr setParamEvent = std::static_pointer_cast<bv::ParamKeyEvent>( eventPtr );
        
        ParamKeyEvent::Command command  = setParamEvent->KeyCommand;
        if( command != ParamKeyEvent::Command::AddKey )
            return;

        std::string nodeName    = toString( setParamEvent->NodeName );
        std::string pluginName  = toString( setParamEvent->PluginName );
        std::string paramName   = toString( setParamEvent->ParamName );
        std::wstring value      = setParamEvent->Value;

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

        if( pluginName == "transform" )
        {
            auto param = node->GetPlugin( pluginName )->GetParameter("simple_transform");
            if( param == nullptr )
            {
                LOG_MESSAGE( SeverityLevel::error ) << "AddParamKey() node ["+ nodeName+"], plugin [" + pluginName + "], param [simple_transform] not found";
                return;
            }
            
            std::string stringValue = toString( value );
            if( paramName == "translation" )
            {
                 SetParameterTranslation( param, 0, (bv::TimeType)keyTime, SerializationHelper::String2Vec3( stringValue ) );
                 LOG_MESSAGE( SeverityLevel::info ) << "Node [" + nodeName + "] translation: " + stringValue + " key time: " + std::to_string( keyTime );
                 // @todo Realy do we want to send messages all the time, when this param changes??
            }
            else if( paramName == "scale" )
            {
                 SetParameterScale ( param, 0, (bv::TimeType)keyTime, SerializationHelper::String2Vec3( stringValue ) );
                 LOG_MESSAGE( SeverityLevel::info ) << "Node [" + nodeName + "] scale: " + stringValue + " key time: " + std::to_string( keyTime );
                 // @todo Realy do we want to send messages all the time, when this param changes??
            }
        }
    }
}


  //  if( evt->GetEventType() == bv::SetParamEvent::m_sEventType)
  //  {
  //      bv::SetParamEventPtr evtSetParam = std::static_pointer_cast<bv::SetParamEvent>( evt );
  //      
  //      auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
  //      
  //      auto node = root->GetNode(NodeNameStr);
  //      if(node==nullptr &&root->GetName()==NodeNameStr)
  //      {
  //          Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
  //          node = root;
  //      }
  //      if(node==nullptr)
  //      {
  //          Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] not found");
  //          return;
  //      }else if(evtSetParam->PluginName == L"transform"){
  //          
  //          if(evtSetParam->ParamName == L"translation")
  //          {
  //               auto param = node->GetPlugin("transform")->GetParameter("simple_transform");
  //               assert( param );

  //               wstring value = evtSetParam->Value;
  //               
  //             
		//		 Log::A("OK", "node ["+ NodeNameStr+"] translation: ("+to_string(evtSetParam->x)+", "+to_string(evtSetParam->y)+", "+to_string(evtSetParam->z)+") time: "+to_string(evtSetParam->time));

  //               SetParameterTranslation ( param, 0, (bv::TimeType)evtSetParam->time, glm::vec3( evtSetParam->x,evtSetParam->y,evtSetParam->z));
  //          }else  if(evtSetParam->ParamName == L"scale")
  //          {
  //               auto param = node->GetPlugin("transform")->GetParameter("simple_transform");
  //               assert( param );

  //               wstring value = evtSetParam->Value;
		//		 Log::A("OK", "node ["+ NodeNameStr+"] scale: ("+to_string(evtSetParam->x)+", "+to_string(evtSetParam->y)+", "+to_string(evtSetParam->z)+") time: "+to_string(evtSetParam->time));
  //               
  //               SetParameterScale ( param, 0, (bv::TimeType)evtSetParam->time, glm::vec3( evtSetParam->x,evtSetParam->y,evtSetParam->z));
  //          }

		//}else if(evtSetParam->PluginName == L"alpha"){
  //          
  //          if(evtSetParam->ParamName == L"alpha")
  //          {
		//		 auto state = node->GetOverrideState();
		//		 auto alpha_param = state->GetAlphaParam();

		//		 wstring value = evtSetParam->Value;
		//		 
		//		 float float_value = 1.0f;

		//		 try{
		//			float_value =  boost::lexical_cast<float>(value);
		//		 }catch(boost::bad_lexical_cast&)
		//		 {
		//			float_value = 0.0f;
		//		 }

		//		 SetParameter( alpha_param, (bv::TimeType)evtSetParam->time, float_value);
  //          }
  //      }else if(evtSetParam->PluginName == L"text"){
  //          
  //          if(evtSetParam->ParamName == L"text")
  //          {
  //              DefaultTextPlugin* txt = dynamic_cast< DefaultTextPlugin* >( node->GetPlugin("text").get() );
  //              wstring text( evtSetParam->Value.begin(), evtSetParam->Value.end() );

  //              if( txt != nullptr )
  //              {
  //                  model::DefaultTextPlugin::SetText( node->GetPlugin("text"), text );
  //              }
  //              else
  //              {
  //                  Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] , plugin [text] not found");
  //              }
  //          }
  //      } else {
		//	string plugin_name_cast= string(evtSetParam->PluginName.begin(), evtSetParam->PluginName.end());
		//	string param_name_cast= string(evtSetParam->ParamName.begin(), evtSetParam->ParamName.end());
  //          auto plugin = node->GetPlugin( plugin_name_cast ).get();
		//	if(plugin==nullptr)
		//	{
		//		Log::A("error", "Error OnSetParam() plugin ["+ plugin_name_cast+"] not found");

		//	}else{
		//		auto param = plugin->GetParameter( param_name_cast );
		//		if(param==nullptr)
		//		{
		//			Log::A("error", "Error OnSetParam() plugin ["+ plugin_name_cast+"] param ["+param_name_cast+"] not found");

		//		}else{

		//			 wstring value = evtSetParam->Value;
		//		 
		//			 float float_value = 1.0f;

		//			 try{
		//				float_value =  boost::lexical_cast<float>(value);
		//			 }catch(boost::bad_lexical_cast&)
		//			 {
		//				float_value = 0.0f;
		//			 }

		//			 SetParameter( param, (bv::TimeType)evtSetParam->time, float_value);
		//		}

		//	}

  //      }   
  //  }

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
        auto plugin = node->GetPlugin( pluginName );
        if( node == nullptr || plugin == nullptr )
            return;

        JsonDeserializeObject deserializer;
        deserializer.Load( asssetData );

        bool result = true;
        auto assetDesc = AssetManager::GetInstance().CreateDesc( deserializer );

        if( assetDesc != nullptr )
            result = plugin->LoadResource( assetDesc );
        else
            result = false;

        //std::wstring response;
        //if( result )
        //    response = L"Asset loaded succesfully. node: [" + eventLoadAsset->NodeName + L"] plugin [" + eventLoadAsset->PluginName + L"]";
        //else
        //    response = L"Failed to load asset. node [" + eventLoadAsset->NodeName + L"] plugin [" + eventLoadAsset->PluginName + L"]";

        //
        //ResponseMsg msg;
        //msg.msg     = response;
        //msg.sock_id = eventLoadAsset->SockID;
        //SocketWrapper::AddMsg( msg );
    }
}

} //bv
