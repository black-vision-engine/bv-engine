#include "EventHandlerHelpers.h"
#include "../../BVAppLogic.h"


#include "../../UseLoggerBVAppModule.h"

namespace bv
{

// ***********************
//
bv::model::IModelNodePtr GetRootNode( BVAppLogic* appLogic, const std::string& sceneName )
{
    auto scene = appLogic->GetBVProject()->GetScene( sceneName );
    if( scene == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Scene [" + sceneName +" ] not found";
        return nullptr;
    }

    auto root = scene->GetRootNode();
    return root;
}

// ***********************
//
bv::model::IModelNodePtr GetNode( BVAppLogic* appLogic, const std::string& sceneName, const std::string& nodeName )
{
    auto root = GetRootNode( appLogic, sceneName );
    if( root == nullptr ) return nullptr;

    auto node = root->GetNode( nodeName );
    if( node == nullptr )
    {
        if( root->GetName() == nodeName )
            node = root;
        else
        {
            LOG_MESSAGE( SeverityLevel::error ) << "Node [" + nodeName + "] not found";
            return nullptr;
        }
    }
    return node;
}

} //bv
