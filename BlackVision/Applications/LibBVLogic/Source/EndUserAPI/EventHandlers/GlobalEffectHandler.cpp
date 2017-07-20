#include "stdafxBVApp.h"

#include "GlobalEffectHandler.h"
#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Editors/BVProjectEditor.h"
#include "Serialization/SerializationHelper.h"
#include "BVAppLogic.h"

namespace bv
{

// ***********************
//
GlobalEffectHandler::GlobalEffectHandler( BVAppLogic* appLogic )
    :   m_appLogic( appLogic )
{}

// ***********************
//
GlobalEffectHandler::~GlobalEffectHandler()
{}

// ***********************
//
void        GlobalEffectHandler::GlobalEffectEventHandler			( bv::IEventPtr evt )
{
    if( evt->GetEventType() == GlobalEffectEvent::Type() )
    {
        GlobalEffectEventPtr effectEvent = std::static_pointer_cast<GlobalEffectEvent>( evt );
        std::string& sceneName      = effectEvent->SceneName;
        std::string& nodePath       = effectEvent->NodePath;
        std::string& effectName     = effectEvent->EffectName;
        std::string& timelinePath	= effectEvent->TimelinePath;

        GlobalEffectEvent::Command command = effectEvent->EffectCommand;
        auto eventID = effectEvent->EventID;
        auto socketID = effectEvent->SocketID;

        auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
        bool result = true;

        if( command == GlobalEffectEvent::Command::SetGlobalEffect )
        {
            result = editor->SetNodeEffect( sceneName, nodePath, timelinePath, effectName, true );
        }
        if( command == GlobalEffectEvent::Command::LoadGlobalEffectAsset )
        {
            auto assetData = effectEvent->AssetData;

            SizeType idx = 0;

            if( effectEvent->Request != nullptr )
            {
                auto req = effectEvent->Request;

                auto idxEx = SerializationHelper::String2T< SizeType >( req->GetAttribute( "assetIdx" ) );

                if( idxEx.isValid )
                {
                    idx = idxEx.ham;
                }
            }

            if( assetData != nullptr )
            {
                result = editor->LoadGlobalEffectAsset( sceneName, nodePath, timelinePath, effectName, *assetData, idx );
            }
            else
            {
                result = false;
            }
        }
        else
        {
            SendSimpleErrorResponse( command, eventID, socketID, "Unknown command" );
            return;
        }

        SendSimpleResponse( command, eventID, socketID, result );
    }
}



} //bv
