#include "GlobalEffectHandler.h"
#include "EventHandlerHelpers.h"
#include "Engine/Models/Builder/ModelNodeEffectFactory.h"
#include "Engine/Models/BVProjectEditor.h"
#include "../../BVAppLogic.h"

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

        bool result = true;

        if( command == GlobalEffectEvent::Command::SetGlobalEffect )
            result = CreateAndSetGlobalEffect( sceneName, nodePath, timelinePath, effectName );
        else
        {
            SendSimpleErrorResponse( command, eventID, socketID, "Unknown command" );
            return;
        }

        SendSimpleResponse( command, eventID, socketID, result );
    }
}


// ***********************
//
bool        GlobalEffectHandler::CreateAndSetGlobalEffect    ( const std::string& sceneName, const std::string& nodePath, const std::string& timelinePath, const std::string& effectName )
{
    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    auto timeEvaluator = editor->GetTimeline( timelinePath );
    auto node = editor->GetNode( sceneName, nodePath );

    if( !node || !timeEvaluator )
        return false;

    auto newEffect = model::ModelNodeEffectFactory::Create( effectName, timeEvaluator );

    if( !newEffect )
        return false;

    editor->SetNodeEffect( node, newEffect );
    return true;
}


} //bv
