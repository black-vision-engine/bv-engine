#pragma once

#include "Serialization/SerializeContext.h"
#include "Assets/AssetDescsWithUIDs.h"

#include "CoreDEF.h"

namespace bv {

namespace model { class TimelineManager; }

class BVSerializeContext;

BVSerializeContext *      Context             ( const ISerializer & deser );

// ***********************
//
class BVSerializeContext : public SerializeContext
{
private:

    AssetDescsWithUIDsPtr               m_assets;
    model::TimelineManager *            m_timelineManager;

public:
    BVSerializeContext( model::TimelineManager * timelineManager )
        : m_assets( std::make_shared< AssetDescsWithUIDs >() )
        , m_timelineManager( timelineManager )
    {
        recursive = true;
        detailedInfo = true;
        pluginsInfo = true;
        extendedAssetData = false;
        sceneNameInTimeline = true;
        inludeUIDs = false;
    }

    bool                                recursive;
    bool                                detailedInfo;
    bool                                pluginsInfo;
    bool                                extendedAssetData;
    bool                                sceneNameInTimeline;
    bool                                inludeUIDs;

    AssetDescsWithUIDsPtr               GetAssets               ();
    void                                SetAssets               ( const AssetDescsWithUIDsPtr & assets );

    model::TimelineManager *            GetTimelineManager      ();

public:

    static BVSerializeContext *         CreateContextFromEmptiness  (); // in future this should have some parameters instead of static singletons
};

}
