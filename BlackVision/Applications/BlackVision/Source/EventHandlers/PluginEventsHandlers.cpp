#include "PluginEventsHandlers.h"

#include "EventHelpers.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/AssetManager.h"

namespace bv
{

// *********************************
// constructor destructor
PluginEventsHandlers::PluginEventsHandlers()
{}

PluginEventsHandlers::~PluginEventsHandlers()
{}

// *********************************
//
void PluginEventsHandlers::AddParamKey( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() == bv::SetParamEvent::m_sEventType)
    {
        bv::SetParamEventPtr setParamEvent = std::static_pointer_cast<bv::SetParamEvent>( eventPtr );

        std::string nodeName    = toString( setParamEvent->NodeName );
        std::string pluginName  = toString( setParamEvent->PluginName );
        std::string paramName   = toString( setParamEvent->ParamName );
        std::string value       = toString( setParamEvent->Value );
        
    }
}


  //  if( evt->GetEventType() == bv::SetParamEvent::m_sEventType)
  //  {
  //      bv::SetParamEventPtr evtSetParam = std::static_pointer_cast<bv::SetParamEvent>( evt );
  //      
  //      auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
  //      

  //      //todo: //fixme: wstring -> string
  //      wstring NodeName = evtSetParam->NodeName;
  //      string NodeNameStr( NodeName.begin(), NodeName.end() );
  //      //todo: //fixme: wstring -> string
  //      wstring TexturePath = evtSetParam->Value;
  //      string TexturePathStr( TexturePath.begin(), TexturePath.end() );

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
  //      }
  //      if(evtSetParam->PluginName == L"texture")
  //      {
  //          auto plugin = node->GetPlugin( "texture" );

  //          if(plugin == nullptr)
  //          {
  //               Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] , plugin [texture] not found");
  //               return;
  //          }
		//		ifstream f(BB::AssetManager::GetTexture(TexturePathStr ).c_str());
  //              if (f.good()) {
  //                  f.close();
  //                   bool result = model::LoadTexture( plugin, BB::AssetManager::GetTexture(TexturePathStr ) );
		//			 {result;}
  //              } else {
  //                  f.close();
		//			 bool result = model::LoadTexture( plugin, BB::AssetManager::GetBlankTexture() );
		//			 {result;}
  //                  Log::A("error", "Error OnSetParam() texture ["+  BB::AssetManager::GetTexture(TexturePathStr )+"] not found");
  //                  return;
  //              }   

  //         
  //          Log::A("OK", "OK OnSetParam() node ["+ NodeNameStr+"] has new image, yo!");

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

  //      }else if(evtSetParam->PluginName == L"visibility"){
  //          
  //          if(evtSetParam->ParamName == L"visible")
  //          {
  //              if(evtSetParam->Value == L"true")
  //              {
  //                  node->SetVisible(true);
  //              }else{
  //                  node->SetVisible(false);
  //              }
  //          }

  //      
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

}

// *********************************
//
void PluginEventsHandlers::RemoveParamKey      ( bv::IEventPtr /*eventPtr*/ )
{

}

// *********************************
//
void PluginEventsHandlers::LoadAsset( bv::IEventPtr /*eventPtr*/ )
{
    //if( eventPtr->GetEventType() == bv::LoadAssetEvent::m_sEventType )
    //{
    //    bv::LoadAssetEventPtr eventLoadAsset = std::static_pointer_cast<bv::LoadAssetEvent>( eventPtr );
    //    
    //    std::string nodeName = toString( eventLoadAsset->NodeName );
    //    std::string pluginName = toString( eventLoadAsset->PluginName );
    //    std::string asssetData = toString( eventLoadAsset->AssetData );

    //    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
    //    auto node = root->GetNode( nodeName );
    //    auto plugin = node->GetPlugin( pluginName );

    //    JsonDeserializeObject deserializer;
    //    deserializer.Load( asssetData );

    //    bool result = true;
    //    auto assetDesc = AssetManager::GetInstance().CreateDesc( deserializer );

    //    if( assetDesc != nullptr )
    //        result = plugin->LoadResource( assetDesc );
    //    else
    //        result = false;

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
    //}
}

} //bv
