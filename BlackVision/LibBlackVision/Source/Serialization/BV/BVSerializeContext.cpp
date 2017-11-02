#include "stdafx.h"

#include "BVSerializeContext.h"

#include "Assets/AssetDescsWithUIDs.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// ***********************
//
AssetDescsWithUIDsPtr               BVSerializeContext::GetAssets               ()
{
    return m_assets;
}

// ***********************
//
void                                BVSerializeContext::SetAssets               ( const AssetDescsWithUIDsPtr & assets )
{
    m_assets = assets;
}

// ***********************
//
model::TimelineManager *            BVSerializeContext::GetTimelineManager      ()
{
    return m_timelineManager;
}

// ***********************
//
BVSerializeContext *                BVSerializeContext::CreateContextFromEmptiness  ()
{
    return new BVSerializeContext( model::TimelineManager::GetInstance() );
}

}
