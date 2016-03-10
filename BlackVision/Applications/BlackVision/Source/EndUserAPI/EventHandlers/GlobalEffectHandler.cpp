#include "GlobalEffectHandler.h"
#include "EventHandlerHelpers.h"
#include "Engine/Models/BVProjectEditor.h"
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
            result = editor->SetNodeEffect( sceneName, nodePath, timelinePath, effectName );
        }
        if( command == GlobalEffectEvent::Command::LoadGlobalEffectAsset )
        {
            auto assetData = effectEvent->AssetData;
            if( assetData != nullptr )
            {
                result = editor->LoadGlobalEffectAsset( sceneName, nodePath, timelinePath, effectName, *assetData );
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
