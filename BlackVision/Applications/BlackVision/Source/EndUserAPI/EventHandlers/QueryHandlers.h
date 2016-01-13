#pragma once

#include "Engine/Events/Events.h"

namespace bv
{

class BVAppLogic;

class QueryHandlers
{
private:
    BVAppLogic*         m_appLogic;
public:
    QueryHandlers( BVAppLogic* appLogic );
    ~QueryHandlers();

    void        Info        ( bv::IEventPtr evt );

private:
    std::string     ListSceneAssets     ( const std::string& request, unsigned int requestID );
    std::wstring    ListProjectNames    ( const std::string& request, unsigned int requestID );
    std::wstring    ListScenes          ( const std::string& request, unsigned int requestID );
    std::wstring    ListAssetsPaths     ( const std::string& request, unsigned int requestID );
    std::wstring    ListCategoriesNames ( const std::string& request, unsigned int requestID );
    std::wstring    ListProjects        ( const std::string& request, unsigned int requestID );

    std::wstring    ListAllScenes           ( const std::string& request, unsigned int requestID );
    std::wstring    ListAllFolders          ( const std::string& request, unsigned int requestID );
    std::wstring    ListResourcesInFolders  ( const std::string& request, unsigned int requestID );
    std::wstring    ListAllResources        ( const std::string& request, unsigned int requestID );

    std::string     VideoCardsInfo      ( const std::string& request, unsigned int requestID );
    std::string     GetNodeInfo         ( const std::string& request, unsigned int requestID );
    std::string     GetMinimalSceneInfo ( const std::string& request, unsigned int requestID );
    std::string     GetTimeLinesInfo    ( const std::string& request, unsigned int requestID );
    std::string     PerformanceInfo     ( const std::string& request, unsigned int requestID );
    std::string     TreeStructureInfo   ( const std::string& request, unsigned int requestID );
    std::string     CheckTimelineTime   ( const std::string& request, unsigned int requestID );
    std::string     PluginInfo          ( const std::string& request, unsigned int requestID );
    
    std::string     ListTimelineKeyframes           ( const std::string& request, unsigned int requestID );
    std::string     MinimalTreeStructureInfo        ( const std::string& request, unsigned int requestID );
};

} //bv
