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
    std::string     ListAssets          ( const std::string& request );
    std::wstring    ListProjectNames    ( const std::string& request );
    std::wstring    ListScenes          ( const std::string& request );
    std::wstring    ListAssetsPaths     ( const std::string& request );
    std::wstring    ListCategoriesNames ( const std::string& request );
    std::wstring    ListProjects        ( const std::string& request );

    std::string     VideoCardsInfo      ( const std::string& request );
    std::string     GetNodeInfo         ( const std::string& request );
    std::string     GetMinimalSceneInfo ( const std::string& request );
    std::string     GetTimeLinesInfo    ( const std::string& request );
    std::string     PerformanceInfo     ( const std::string& request );
    std::string     TreeStructureInfo   ( const std::string& request );
    std::string     CheckTimelineTime   ( const std::string& request );
};

} //bv
